/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012  Mattia Basaglia

Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include "knotview.hpp"
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QApplication>
#include "commands.hpp"
#include "knotgraph.hpp"
#include <QXmlStreamWriter>
#include <QDomDocument>
#include <QSvgGenerator>

/**
    \todo snap to grid
    \todo insert fixed shapes
*/

KnotView::KnotView( QWidget* parent )
    : QGraphicsView(parent), mode(EDIT_NODE),
    last_node(NULL),
    mouse_status(DEFAULT),
    guide(NULL), rubberband(NULL)
{
    setScene ( new QGraphicsScene );
    setSceneRect ( 0, 0, width(), height());
    mode_edit_node();
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor( QGraphicsView::NoAnchor );
    scene()->addItem(&knot);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void KnotView::do_add_node(Node *node, node_list adjl)
{
    last_node = node;
    scene()->addItem ( last_node );
    foreach(Node* n, adjl)
    {
        link(n,last_node);
    }
    knot.add_node(node);
    redraw(false);
}

void KnotView::do_remove_node(Node *node)
{
    if ( mode == INSERT_EDGE_CHAIN && guide )
    {
        if ( moved_nodes.empty() )
        {
            last_node = NULL;
        }
        else
        {
            last_node = moved_nodes.back(); // why can this make last_node = NULL?
            moved_nodes.pop_back();
        }
        if ( last_node && last_node->scene() )
            set_guide(last_node->pos(),guide->line().p2());
        else
            unset_guide();
    }
    else if ( node == last_node && guide )
    {
        last_node = NULL;
        int i = undo_stack.index();
        --i;
        while ( i >= 0 )
        {
            const LastNodeCommand* lnc = dynamic_cast<const LastNodeCommand*>(undo_stack.command(i));
            if ( lnc )
            {
                last_node = lnc->get_node();
                if ( last_node != node )
                    break;
            }
            --i;
        }
        if ( node == last_node || !last_node || !last_node->scene() )
        {
            last_node = NULL;
            unset_guide();
        }
        else
        {
            set_guide(last_node->pos(),guide->line().p2());
        }

    }


    node->setSelected(false);
    scene()->removeItem(node);
    foreach ( Edge* e, node->links() )
    {
        scene()->removeItem(e);
        node->remove_link(e);
        e->other(node)->remove_link(e);
        knot.remove_edge(e);
        delete e;
    }
    knot.remove_node(node);
    redraw(true);
}

void KnotView::do_move_node(Node* node, QPointF pos)
{
    node->setPos(pos);
    redraw(true);
}



QPointF KnotView::get_mouse_point(QMouseEvent *event)
{
    /// \todo fix this to work with dragging

    QPointF p = mapToScene(event->pos());

    if ( last_node && ( event->modifiers() & Qt::ControlModifier ) )
    {
        // snap to closest 15deg angle from last node
        QLineF line(last_node->pos(),p);
        int angle = line.angle() / 15;
        angle *= 15;
        line.setAngle(angle);
        p = line.p2();
    }

    return p;
}

void KnotView::mousePressEvent(QMouseEvent *event)
{
    if ( !isInteractive() ) return;

    QPointF p = get_mouse_point ( event );
    Node* itm = node_at(p);

    if ( event->button() == Qt::RightButton ) // select nodes
    {
        if ( mode == INSERT_EDGE_CHAIN )
        {
            mode_change();
        }

        if ( ! ( event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier) ) )
            scene()->clearSelection();
        if ( itm )
            itm->setSelected(!itm->isSelected());
        QColor bg = QApplication::palette().color(QPalette::Highlight);
        bg.setAlpha(128);
        rubberband = scene()->addRect ( 0,0,0,0,
            QApplication::palette().color(QPalette::Highlight), bg );
        rubberband->setPos(p);
        rubberband->setZValue(100);
        return;
    }
    else if ( event->button() == Qt::MiddleButton ) // drag view
    {
        oldpos = event->pos();
        mouse_status = DRAGGING;
        return;
    }

    if ( mode == EDIT_NODE )
    {
        if ( !itm )
        {
            Edge* edg = edge_at(p);
            if ( edg )
            {
                Node* v1 = edg->vertex1();
                Node* v2 = edg->vertex2();
                QLineF oldline ( v1->pos(), v2->pos() );
                QLineF newline ( v1->pos(), p );
                newline.setAngle( oldline.angle() );

                undo_stack.beginMacro("Break Edge");
                add_node(newline.p2(),node_list());
                remove_edge(v1,v2);
                add_edge(last_node,v1);
                add_edge(last_node,v2);
                undo_stack.endMacro();
            }
            else
                add_node(p,node_list());

            scene()->clearSelection();
            last_node->setSelected(true);
        }
        else
        {
            last_node = itm;
            if ( !last_node->isSelected() )
            {
                scene()->clearSelection();
                last_node->setSelected(true);
            }
        }

        moved_nodes.clear();
        foreach ( QGraphicsItem* itm, scene()->selectedItems() )
        {
            Node* n = dynamic_cast<Node*>(itm);
            if ( n )
                moved_nodes.push_back(n);
        }
        if ( !moved_nodes.empty() )
        {
            mouse_status = MOVING;
            oldpos = p;
            QGraphicsView::mousePressEvent(event);
        }
    }
    else if ( mode == INSERT_EDGE )
    {
        if ( itm )
        {
            last_node = itm;
            set_guide(last_node->pos(),p);
        }
        else
            toggle_edge(edge_at(p));
    }
    else if ( mode == INSERT_EDGE_CHAIN )
    {
        Node* next_node = itm;
        if ( !next_node )
        {
            Node* prev_node = last_node;
            node_list pl;
            if ( last_node )
                pl.push_back(last_node);
            add_node(p,pl);
            next_node = last_node;
            last_node = prev_node;
        }
        else if ( last_node )
        {
            add_edge(last_node,next_node);
        }


        moved_nodes.push_back ( last_node );

        last_node = next_node;
        set_guide(last_node->pos(),p);
        scene()->clearSelection();
        last_node->setSelected(true);
    }
}
#include <QScrollBar>
void KnotView::mouseMoveEvent(QMouseEvent *event)
{
    if ( !isInteractive() ) return;

    QPointF p = get_mouse_point ( event );

    emit mouse_moved ( p );


    if ( mouse_status == DRAGGING ) // drag view
    {
        QPointF delta = oldpos-event->pos();
        delta /= matrix().m11(); // take scaling into account
        QRectF r = sceneRect();
        r.translate(delta);
        setSceneRect ( r );
        oldpos = event->pos();

        return;
    }

    foreach ( QGraphicsItem* gi, scene()->items() )
    {
        CustomItem* ci = dynamic_cast<CustomItem*>(gi);
        if ( ci )
            ci->highlight = false;
    }
    CustomItem* ci = dynamic_cast<CustomItem*>(scene()->itemAt(p));
    if ( ci )
        ci->highlight = true;


    if ( rubberband )
    {
        rubberband->setRect(QRectF(QPointF(0,0),p-rubberband->pos()));
    }
    else if ( mode == EDIT_NODE )
    {
        QGraphicsView::mouseMoveEvent(event);
         knot.build();/// \todo option to turn on/off fluid knot refresh
    }
    else if ( mode == INSERT_EDGE_CHAIN || mode == INSERT_EDGE )
    {
        if ( guide && last_node )
        {
            set_guide ( last_node->pos(), p );
        }
    }
    redraw(false);
}


void KnotView::mouseReleaseEvent(QMouseEvent *event)
{
    if ( !isInteractive() ) return;

    QPointF p = get_mouse_point ( event );


    if ( event->button() == Qt::RightButton )
    {
        QList<QGraphicsItem*> sel = scene()->selectedItems();
        QPainterPath pp;
        pp.addRect(rubberband->rect());
        pp.translate(rubberband->pos());
        scene()->setSelectionArea(pp);
        foreach(QGraphicsItem* itm, sel)
            itm->setSelected(true);

        scene()->removeItem(rubberband);
        delete rubberband;
        rubberband = NULL;
    }
    else if ( event->button() == Qt::MiddleButton ) // drag view
    {
        mouse_status = DEFAULT;
        return;
    }
    else if ( mode == EDIT_NODE )
    {
        if ( mouse_status == MOVING )
        {
            if ( p != oldpos )
                move_nodes(p);
            mouse_status = DEFAULT;
            QGraphicsView::mouseReleaseEvent(event);

        }
    }
    else if ( mode == INSERT_EDGE )
    {
        unset_guide();

        Node* itm = node_at(p);
        if ( last_node && itm && itm != last_node )
        {
            if ( last_node->get_link ( itm ) )
                remove_edge ( last_node, itm );
            else
                add_edge ( last_node, itm );
        }
    }
    redraw(false);
}

void KnotView::wheelEvent(QWheelEvent *event)
{
    if ( !isInteractive() ) return;

    QPointF p = mapToScene(event->pos());

    if ( event->modifiers() & Qt::ControlModifier )
    {
        if ( event->delta() < 0 )
            zoom(0.8);
        else
            zoom(1.25);
    }
    else // toggle egde type
    {
        if ( event->delta() < 0 )
            toggle_edge(edge_at(p));
        else
            toggle_edge_inverted(edge_at(p));
    }
}


Node *KnotView::node_at(QPointF p)
{
    return dynamic_cast<Node*>(scene()->itemAt(p));
}

Edge *KnotView::edge_at(QPointF p)
{
    return dynamic_cast<Edge*>(scene()->itemAt(p));
}

void KnotView::link(Node *a, Node *b )
{
    if ( !a || !b )
        return;

    Edge* already_there = a->get_link ( b );
    if ( !already_there )
    {
        Edge *brand_new = new Edge(a,b);
        scene()->addItem ( brand_new );
        a->add_link(brand_new);
        b->add_link(brand_new);
        knot.add_edge(brand_new);
        redraw(true);
    }
}

void KnotView::unlink(Node *a, Node *b)
{
    if ( !a || !b )
        return;

    Edge* already_there = a->get_link ( b );
    if ( already_there )
    {
        if ( mode == INSERT_EDGE_CHAIN && guide)
        {
            if ( moved_nodes.empty() )
            {
                last_node = NULL;
                unset_guide();
            }
            else
            {
                last_node = moved_nodes.back();
                moved_nodes.pop_back();
                set_guide(last_node->pos(),guide->line().p2());
            }
        }
        a->remove_link(already_there);
        b->remove_link(already_there);
        scene()->removeItem(already_there);
        knot.remove_edge(already_there);
        delete already_there;
        redraw(true);
    }
}

void KnotView::do_toggle_edge(Node *a, Node *b, Edge::type_type type)
{
    Edge* edge = a->get_link ( b );
    if ( edge )
        edge->type = type;
    redraw(true);
}

void KnotView::clear()
{
    last_node = 0;
    undo_stack.clear();
    scene()->removeItem(&knot);
    unset_guide();
    scene()->clear();
    scene()->addItem(&knot);
    knot.clear();
    /*foreach ( QGraphicsItem* itm, scene()->items() )
    {
        Node* nod = dynamic_cast<Node*>(itm);
        if ( nod )
            do_remove_node(nod);
    }*/
    mode_change();
}

void KnotView::writeXML(QIODevice *device) const
{
    QXmlStreamWriter xml(device);
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(4);
    xml.writeStartDocument("1.0");

    xml.writeStartElement("knot");
        xml.writeAttribute("version","1"); // note: version MUST be an integer

        xml.writeStartElement("style");
            xml.writeStartElement("stroke");
                xml.writeAttribute("width",QString::number(get_width()));
                xml.writeAttribute("color",get_brush().color().name());
            xml.writeEndElement();
            xml.writeStartElement("outline");
                QPen pen = get_pen();
                xml.writeAttribute("width",QString::number(pen.widthF()));
                xml.writeAttribute("color",pen.color().name());
            xml.writeEndElement();
            xml.writeStartElement("cusp");
                xml.writeTextElement("style",knot_curve_styler::name(knot.get_curve_style()));
                xml.writeTextElement("min-angle",QString::number(knot.get_cusp_angle()));
                QString point;
                switch ( knot.get_join_style() )
                {
                    default:
                    case Qt::BevelJoin: point = "bevel"; break;
                    case Qt::MiterJoin: point = "miter"; break;
                    case Qt::RoundJoin: point = "round"; break;
                }
                xml.writeTextElement("point",point);
            xml.writeEndElement();
            xml.writeStartElement("crossing");
                xml.writeTextElement("gap",QString::number(knot.get_crossing_distance()));
                xml.writeTextElement("handle-length",QString::number(knot.get_handle_length()));
            xml.writeEndElement();
        xml.writeEndElement();

        xml.writeStartElement("graph");
        QMap<Node*,int> node_ids;
        foreach(QGraphicsItem* it, scene()->items())
        {
            Node* n = dynamic_cast<Node*>(it);
            if ( !n )
                continue;

            int id;
            if ( node_ids.contains(n) )
                id = node_ids[n];
            else
            {
                id = node_ids.size();
                node_ids[n] = id;
            }

            xml.writeStartElement("node");
                xml.writeAttribute("id",QString("node_%1").arg(id));
                xml.writeAttribute("x",QString::number(n->pos().x()));
                xml.writeAttribute("y",QString::number(n->pos().y()));
                foreach(Edge* e, n->links())
                {
                    Node* o = e->other(n);
                    int oid;
                    if ( node_ids.contains(o) )
                        oid = node_ids[o];
                    else
                    {
                        oid = node_ids.size();
                        node_ids[o] = oid;
                    }
                    xml.writeEmptyElement("edge");
                        QString edge_type;
                        switch ( e->type )
                        {
                            default:
                            case Edge::CROSSING: edge_type = "regular"; break;
                            case Edge::WALL:     edge_type = "wall"; break;
                            case Edge::HOLE:     edge_type = "hole"; break;
                            case Edge::INVERTED: edge_type = "inverted"; break;
                        }
                        xml.writeAttribute("type",edge_type);
                        xml.writeAttribute("target",QString("node_%1").arg(oid));

                }
            xml.writeEndElement();
        }
        xml.writeEndElement(); // graph
    xml.writeEndElement();// knot

    xml.writeEndDocument();
}

bool KnotView::readXML(QIODevice *device)
{
    QDomDocument xml;
    if ( !xml.setContent(device) ) // could retrieve error details
        return false; // xml error

    QDomElement knot = xml.firstChildElement("knot");
    if ( knot.isNull() )
        return false; // XML does not contain a knot
    if ( knot.attribute("version").toInt() != 1 )
        return false; // unknown version

    QDomElement graph = knot.firstChildElement("graph");
    if ( graph.isNull() )
        return false; // XML does not contain a graph description

    undo_stack.beginMacro("Load Knot");

    QMap<QString,Node*> node_ids;
    QDomElement node = graph.firstChildElement("node");
    while ( !node.isNull() )
    {
        QString id = node.attribute("id");
        Node* cur_node = 0;
        if ( !node_ids.contains(id) )
        {
            cur_node = node_ids[id] = new Node(QPointF());
        }
        else
            cur_node = node_ids[id];
        add_node(cur_node);
        cur_node->setPos(node.attribute("x").toDouble(),node.attribute("y").toDouble());

        QDomElement edge = node.firstChildElement("edge");
        while ( !edge.isNull() )
        {
            QString type_name = edge.attribute("type");
            QString target = edge.attribute("target");
            Edge::type_type type = Edge::CROSSING;
            if ( type_name == "regular" )
                type = Edge::CROSSING;
            else if ( type_name == "wall" )
                type = Edge::WALL;
            else if ( type_name == "hole" )
                type = Edge::HOLE;
            else if ( type_name == "inverted" )
                type = Edge::INVERTED;
            Node* target_node = 0;
            if ( !node_ids.contains(target) )
            {
                target_node = node_ids[target] = new Node(QPointF());
            }
            else
                target_node = node_ids[target];
            add_edge(cur_node,target_node);
            set_edge_type( cur_node->get_link(target_node), type );
            edge = edge.nextSiblingElement("edge");
        }
        node = node.nextSiblingElement("node");
    }

    undo_stack.endMacro();
    undo_stack.setClean();

    QDomElement style = knot.firstChildElement("style");
    if ( !style.isNull() )
    {
        QDomElement stroke = style.firstChildElement("stroke");
        set_width(stroke.attribute("width","5").toDouble());
        set_brush(QColor(stroke.attribute("color","black")));

        QDomElement outline = style.firstChildElement("outline");
        QPen p;
        p.setColor(QColor(outline.attribute("color","gray")));
        p.setWidth(outline.attribute("width","1").toDouble());
        set_pen(p);

        QDomElement cusp = style.firstChildElement("cusp");
            QDomElement cusp_style = cusp.firstChildElement("style");
            QString stylename =  cusp_style.isNull() ? "pointed" : cusp_style.text();
            set_curve_style(knot_curve_styler::idof(stylename));
            QDomElement cusp_angle = cusp.firstChildElement("min-angle");
            double angle = cusp_angle.isNull() ? 225 : cusp_angle.text().toDouble();
            set_cusp_angle(angle);
            QDomElement cusp_point = cusp.firstChildElement("point");
            QString point_name =  cusp_point.isNull() ? "miter" : cusp_point.text();
            if ( point_name == "bevel" )
                set_join_style(Qt::BevelJoin);
            else if ( point_name == "round" )
                set_join_style ( Qt::RoundJoin );
            else
                set_join_style ( Qt::MiterJoin );

        QDomElement crossing = style.firstChildElement("crossing");
            QDomElement egap = crossing.firstChildElement("gap");
            double dgap = egap.isNull() ? 10 : egap.text().toDouble();
            set_crossing_distance(dgap);
            QDomElement handle_length = crossing.firstChildElement("handle-length");
            double hl = handle_length.isNull() ? 10 : handle_length.text().toDouble();
            set_handle_length(hl);

    }

    return true;
}

#include <QStyleOptionGraphicsItem>
void KnotView::writeSVG(QIODevice *device)
{
    /// \todo export only path ( output of knot.build() )
    QSvgGenerator gen;
    gen.setOutputDevice(device);
    //gen.setSize(knot.boundingRect().size().toSize());
    /// \todo gen.setViewBox (?)
    QPainter painter;
    painter.begin(&gen);
        QStyleOptionGraphicsItem opt;
        QPointF off = knot.boundingRect().topLeft();
        painter.translate(-off.x(),-off.y()); // remove offset
        knot.paint(&painter,&opt);
    painter.end();
}

void KnotView::set_width(double w)
{
    knot.set_width(w);
    redraw(true);
}

double KnotView::get_width() const
{
    return knot.get_width();
}

void KnotView::set_pen(QPen p)
{
    bool redraw_knot = false;
    if ( p.joinStyle() != get_pen().joinStyle() )
        redraw_knot = true; // alters path

    knot.setPen(p);
    redraw(redraw_knot);
}

QPen KnotView::get_pen() const
{
    return knot.pen();
}

void KnotView::set_brush(QBrush b)
{
    knot.setBrush(b);
    redraw(false);
}

QBrush KnotView::get_brush() const
{
    return knot.brush();
}

knot_curve_styler::style_id KnotView::get_curve_style() const
{
    return knot.get_curve_style();
}

void KnotView::set_curve_style(knot_curve_styler::style_id si)
{
    knot.set_curve_style ( si );
    redraw(true);
}

double KnotView::get_cusp_angle() const
{
    return knot.get_cusp_angle();
}

void KnotView::set_cusp_angle(double ca)
{
    knot.set_cusp_angle ( ca );
    redraw(true);
}

double KnotView::get_handle_length() const
{
    return knot.get_handle_length();
}

void KnotView::set_handle_length(double hl)
{
    knot.set_handle_length(hl);
    redraw(true);
}

double KnotView::get_crossing_distance() const
{
    return knot.get_crossing_distance();
}

void KnotView::set_crossing_distance(double cd)
{
    knot.set_crossing_distance(cd);
    redraw(true);
}

Qt::PenJoinStyle KnotView::get_join_style() const
{
    return knot.get_join_style();
}

void KnotView::set_join_style(Qt::PenJoinStyle pjs)
{
    knot.set_join_style(pjs);
    redraw(true);
}



void KnotView::set_guide(QPointF from, QPointF to)
{
    if ( guide )
    {
        guide->setLine(QLineF(from,to));
    }
    else
        guide = scene()->addLine(QLineF(from,to),QPen(Qt::gray,1,Qt::DashLine));
}

void KnotView::unset_guide()
{
    if ( guide )
    {
        scene()->removeItem(guide);
        delete guide;
        guide = NULL;
    }
}

void KnotView::mode_change()
{
    last_node = NULL;
    unset_guide();
    //scene()->clearSelection();
}


Node *KnotView::add_node(QPointF pos, node_list adjl)
{
    Node* nn = new Node(pos);
    undo_stack.push(new AddNode(nn,adjl,this));
    return nn;
}

/**
    add existing node, undo command will takes ownership
*/
void KnotView::add_node(Node *n)
{
    undo_stack.push(new AddNode(n,n->linked_to(),this));
}


void KnotView::remove_node(Node *node)
{
    if ( node )
    {
        node_list adjl;
        foreach ( Edge* e, node->links() )
        {
            adjl.push_back(e->other(node));
        }
        remove_node(node,adjl);
    }
}

void KnotView::remove_node(Node *node,node_list adjl)
{
    if ( node )
    {
        undo_stack.push(new RemoveNode(node,adjl,this));
    }
}

void KnotView::add_edge(Node *p1, Node *p2)
{
    if ( p1 && p2 && !p1->get_link(p2) )
        undo_stack.push(new AddEdge(p1,p2,this));
}

void KnotView::remove_edge(Node *p1, Node *p2)
{
    if ( p1 && p2 && p1->get_link(p2) )
        undo_stack.push(new RemoveEdge(p1,p2,this));
}

void KnotView::move_nodes ( QPointF dest )
{
        QPointF delta = dest-oldpos;
    if ( moved_nodes.size() > 1 )
    {
        undo_stack.beginMacro("Move Nodes");
        foreach ( Node* n, moved_nodes )
        {
            undo_stack.push(new MoveNodes(n,n->pos()-delta,n->pos(),this));
        }
        undo_stack.endMacro();
    }
    else if ( moved_nodes.size() == 1 )
    {
        undo_stack.push(new MoveNodes(moved_nodes[0],moved_nodes[0]->pos()-delta,
                                     moved_nodes[0]->pos(),this));
    }
    moved_nodes.clear();
}

void KnotView::toggle_edge(Edge* e)
{
    if ( e )
    {
        Edge::type_type tt = e->type;
        switch ( tt ) // cycle type
        {
            case Edge::CROSSING:  tt = Edge::WALL; break;
            case Edge::WALL:      tt = Edge::HOLE; break;
            case Edge::HOLE:      tt = Edge::INVERTED; break;
            case Edge::INVERTED:  tt = Edge::CROSSING; break;
        }
        undo_stack.push(new ToggleEdge(e->vertex1(),e->vertex2(),e->type,tt,this));
    }
}

void KnotView::toggle_edge_inverted(Edge *e)
{
    if ( e )
    {
        Edge::type_type tt = e->type;
        switch ( tt ) // cycle type
        {
            case Edge::HOLE:        tt = Edge::WALL; break;
            case Edge::INVERTED:    tt = Edge::HOLE; break;
            case Edge::CROSSING:    tt = Edge::INVERTED; break;
            case Edge::WALL:        tt = Edge::CROSSING; break;
        }
        undo_stack.push(new ToggleEdge(e->vertex1(),e->vertex2(),e->type,tt,this));
    }
}

void KnotView::set_edge_type(Edge *e, Edge::type_type type)
{
    if ( e )
    {
        undo_stack.push(new ToggleEdge(e->vertex1(),e->vertex2(),e->type,type,this));
    }
}

node_list KnotView::selected_nodes() const
{
    node_list nl;
    foreach ( QGraphicsItem* it, scene()->selectedItems() )
    {
        Node* n = dynamic_cast<Node*>(it);
        if(n)
            nl.push_back(n);
    }
    return nl;
}

void KnotView::mode_edit_node()
{
    mode = EDIT_NODE;
    mode_change();
}

void KnotView::mode_insert_edge()
{
    mode = INSERT_EDGE;
    mode_change();
}

void KnotView::mode_edge_chain()
{
    mode = INSERT_EDGE_CHAIN;
    mode_change();
}

void KnotView::erase_selected()
{
    //if ( mode == EDIT_NODE )
    {
        undo_stack.beginMacro("Remove Nodes");
        foreach( QGraphicsItem* node, scene()->selectedItems() )
        {
            remove_node ( dynamic_cast<Node*>(node) );
        }
        undo_stack.endMacro();
    }
}

void KnotView::link_selected()
{
    if ( mode == EDIT_NODE )
    {
        undo_stack.beginMacro("Link Nodes");
        typedef QList<QGraphicsItem*>::iterator iter;
        QList<QGraphicsItem*> sel = scene()->selectedItems();
        for ( iter i = sel.begin(); i != sel.end(); ++i )
        {
            iter j = i;
            for ( ++j; j != sel.end(); ++j )
                add_edge(dynamic_cast<Node*>(*i),dynamic_cast<Node*>(*j));
        }
        undo_stack.endMacro();
    }
}


void KnotView::merge_selected()
{

    if ( mode == EDIT_NODE )
    {

        QPointF pos; // average position

        node_list links; // nodes linked to selection

        QList<QGraphicsItem*> selection = scene()->selectedItems();

        if ( selection.count() < 2 )
            return;

        undo_stack.beginMacro("Merge Nodes");

        foreach( QGraphicsItem* node, selection )
        {
            Node* n = dynamic_cast<Node*>(node);
            if ( n )
            {
                pos += node->pos();
                node_list adjacency_list;
                foreach ( Edge* e, n->links() )
                {
                    Node *o = e->other(n);
                    adjacency_list.push_back(o);
                    if ( !scene()->selectedItems().contains(o) )
                        links.push_back(o);
                }
                remove_node(n,adjacency_list);
            }
        }

        add_node(pos/selection.count(),links);
        last_node->setSelected(true);

        undo_stack.endMacro();

    }
}

void KnotView::select_all()
{
    //if ( mode == EDIT_NODE )
    {
        foreach( QGraphicsItem* node, scene()->items() )
        {
            if ( dynamic_cast<Node*>(node) )
                node->setSelected(true);
        }
    }
}


void KnotView::redraw(bool recalculate_node)
{
    if ( recalculate_node )
        knot.build();
    scene()->invalidate();
}

void KnotView::reset_view()
{
    reset_zoom();
    setSceneRect ( -width()/2, -height()/2, width(), height());
}

void KnotView::zoom(double factor)
{
    scale(factor,factor);
}

void KnotView::reset_zoom()
{
    QMatrix trans = matrix();
    trans.setMatrix(1,trans.m12(),trans.m21(),1,trans.dx(),trans.dy());
    setMatrix(trans);
}

