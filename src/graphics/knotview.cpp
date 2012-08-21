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
#include "grid_scene.hpp"

KnotView::KnotView( QWidget* parent )
    : QGraphicsView(parent), mode(EDIT_NODE_EDGE),
    last_node(NULL),
    mouse_status(DEFAULT),
    guide(NULL), rubberband(NULL)
{
    setScene ( new GridScene(&grid) );
    setSceneRect ( 0, 0, width(), height());
    mode_edit_node_edge();
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor( QGraphicsView::NoAnchor );
    scene()->addItem(&knot);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setCacheMode(CacheBackground);
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
        if ( node_chain.size() < 2 )
        {
            node_chain.clear();
            last_node = NULL;
        }
        else
        {
            node_chain.pop_back();
            last_node = node_chain.back(); // why can this make last_node = NULL?
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
        //scene()->removeItem(e);
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
    (void)node;(void)pos;
    node->setPos(pos);
    redraw(true);
}



QPointF KnotView::get_mouse_point(QMouseEvent *event)
{
    return mapToScene(event->pos());
}

void KnotView::snap(QPointF& p,QMouseEvent *event)
{
    if ( last_node && ( event->modifiers() & Qt::ControlModifier ) )
    {
        // snap to closest 15deg angle from last node
        QLineF line(last_node->pos(),p);
        int angle = line.angle() / 15;
        angle *= 15;
        line.setAngle(angle);
        p = line.p2();
    }

    grid.snap(p);
}

void KnotView::mouseDoubleClickEvent(QMouseEvent * event)
{
    if ( !isInteractive() ) return;

    if ( event->button() == Qt::LeftButton )
    {
        QPointF p = get_mouse_point ( event );
        Node *node=node_at(p);

        snap ( p, event );

        if ( node )
            node->setPos(p);
        else
            node=node_at(p);

        if ( !node )
            add_node_or_break_edge(p);

        redraw(true);
    }

}

void KnotView::mousePressEvent(QMouseEvent *event)
{
    if ( !isInteractive() ) return;

    QPointF p = get_mouse_point ( event );
    Node* node = node_at(p);
    Edge* edge = edge_at(p);

    if ( mode == MOVE_GRID )
    {
        mouse_status = MOVING;
        return;
    }


    if ( event->button() == Qt::LeftButton )
    {
        if ( mode == INSERT_EDGE_CHAIN )
        {
            snap(p,event);
            if ( !node )
                node = node_at(p);

            Node* next_node = node;

            if ( last_node && last_node == next_node )
                return;

            if ( !next_node )
            {
                node_list pl;
                if ( last_node )
                    pl.push_back(last_node);
                next_node = add_node_or_break_edge(p,pl);
            }
            else if ( last_node )
            {
                add_edge(last_node,next_node);
            }

            last_node = next_node;

            node_chain.push_back ( last_node );

            set_guide(last_node->pos(),p);
            scene()->clearSelection();
            last_node->setSelected(true);

            return;
        }

        if ( node )
        {
            if ( !node->isSelected() )
            {
                if ( !( event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier) ) )
                    scene()->clearSelection();
                // not selected, select
                node->setSelected(true);
            }
            else if ( event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier) )
            {
                // selected, toggling selection
                node->setSelected(false);
            }

            if ( node->isSelected() )
            {
                mouse_status = MOVING;
                snap(p,event);
                startpos = oldpos = node->pos();
                initialize_movement(node);
            }
        }
        else if ( edge )
        {
            if ( ! (  edge->vertex1()->isSelected() && edge->vertex2()->isSelected() ) )
            {
                if ( !( event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier) ) )
                    scene()->clearSelection();
                // not selected, select
                edge->vertex1()->setSelected(true);
                edge->vertex2()->setSelected(true);
            }
            else if ( event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier) )
            {
                // selected, toggling selection
                edge->vertex1()->setSelected(false);
                edge->vertex2()->setSelected(false);
            }

            if ( edge->vertex1()->isSelected() )
            {
                mouse_status = MOVING;
                snap(p,event);
                startpos = oldpos = p;
                initialize_movement(edge->vertex1());
            }
        }
        else if ( !( event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier) ) )
            scene()->clearSelection();
    }
    else if ( event->button() == Qt::MidButton )
    {
        oldpos = event->pos();
        mouse_status = DRAGGING;
    }
    else if ( event->button() == Qt::RightButton )
    {
        if ( mode == INSERT_EDGE_CHAIN )
        {
            mode_change();
        }

        if ( node )
        {
            emit context_menu(node);
            return;
        }
        else if ( edge )
        {
            emit context_menu(edge);
            return;
        }

        /*if ( node )
        {
            last_node = node;
            set_guide(last_node->pos(),p);
            mouse_status = EDGING;
        }
        else if ( edge )
            cycle_edge(edge);*/
    }

    if ( mouse_status == DEFAULT )
    {
        mouse_status = SELECTING;
        QColor bg = QApplication::palette().color(QPalette::Highlight);
        bg.setAlpha(128);
        rubberband = scene()->addRect ( 0,0,0,0,
            QApplication::palette().color(QPalette::Highlight), bg );
        rubberband->setPos(p);
        rubberband->setZValue(100);
    }
}

void KnotView::mouseMoveEvent(QMouseEvent *event)
{
    if ( !isInteractive() ) return;

    QPointF p = get_mouse_point ( event );

    emit mouse_moved ( p );


    if ( mode == MOVE_GRID )
    {
        Node *node=node_at(p);
        if ( node )
            p = node->pos();
        grid.set_origin(p);
        redraw(false);
        return;
    }

    if ( mouse_status == MOVING )
    {
        snap(p,event);
        foreach(Node* n,selected_nodes())
        {
            n->setPos(n->pos()+p-oldpos);
        }
        oldpos = p;

        knot.build();/// \todo option to turn on/off fluid knot refresh
    }
    else if ( mouse_status == SELECTING )
    {
        rubberband->setRect(QRectF(QPointF(0,0),p-rubberband->pos()));
    }
    else if ( mouse_status == DRAGGING ) // drag view
    {
        QPointF delta = oldpos-event->pos();
        delta /= matrix().m11(); // take scaling into account
        QRectF r = sceneRect();
        r.translate(delta);
        setSceneRect ( r );
        oldpos = event->pos();
        return;
    }
    /*else if ( mouse_status == EDGING )
    {
        if ( guide && last_node )
        {
            set_guide ( last_node->pos(), p );
        }
    }*/
    else if ( mode == INSERT_EDGE_CHAIN )
    {
        QPointF q = p;
        snap(q,event);

        if ( guide && last_node )
        {
            set_guide ( last_node->pos(), q );
        }
    }



    // highlight item under cursor
    foreach ( QGraphicsItem* gi, scene()->items() )
    {
        CustomItem* ci = dynamic_cast<CustomItem*>(gi);
        if ( ci )
            ci->highlight = false;
    }
    CustomItem* ci = dynamic_cast<CustomItem*>(scene()->itemAt(p));
    if ( ci )
        ci->highlight = true;


    redraw(false);
}

void KnotView::mouseReleaseEvent(QMouseEvent *event)
{
    if ( !isInteractive() ) return;

    QPointF p = get_mouse_point ( event );


    if ( mode == MOVE_GRID )
    {
        Node *node=node_at(p);
        if ( node )
            p = node->pos();
        grid.set_origin(p);
        redraw(false);
        mode_edit_node_edge();
        mouse_status = DEFAULT;
        return;
    }

    if ( mouse_status == MOVING )
    {
        snap(p,event);
        if ( p != startpos )
            move_nodes(p);
        sel_offset.clear();
    }
    else if ( mouse_status == SELECTING )
    {

        if ( !( event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier) ) )
            scene()->clearSelection();

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
    /*else if ( mouse_status == EDGING )
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
    }*/

    mouse_status = DEFAULT;
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
    else if ( mouse_status == MOVING )
    {

        node_list sel = selected_nodes();
        if ( sel.size() > 1 )
        {

            if ( event->modifiers() & Qt::ShiftModifier )
            {
                double angle = ( event->delta() < 0 ? -15 : +15 );
                QPointF origin = sel[0]->pos();
                foreach ( Node* selnode, sel )
                {
                    QLineF vector ( origin, selnode->pos() );
                    vector.setAngle(vector.angle()+angle);
                    selnode->setPos(vector.p2());
                }
            }
            else
            {
                /*double factor = ( event->delta() < 0 ? 0.5 : 2 );
                QPointF origin = sel[0]->pos();
                foreach ( Node* selnode, sel )
                {
                    QLineF vector ( origin, selnode->pos() );
                    vector.setLength(vector.length()*factor);
                    selnode->setPos(vector.p2());
                }*/
                QPointF origin = sel[0]->pos();
                sel_size += event->delta() < 0 ? -1 : 1;
                foreach ( Node* selnode, sel )
                {
                    QLineF vector ( QPointF(0,0), sel_offset[selnode] );
                    vector.setLength(vector.length()*sel_size);
                    vector.translate(origin);
                    selnode->setPos(vector.p2());
                }
            }
            redraw(true);
        }
    }
    else // toggle egde type
    {
        if ( event->delta() < 0 )
            cycle_edge(edge_at(p));
        else
            cycle_edge_inverted(edge_at(p));
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
            if ( node_chain.empty() )
            {
                last_node = NULL;
                unset_guide();
            }
            else
            {
                last_node = node_chain.back();
                node_chain.pop_back();
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

    mode_change();
}


void KnotView::writeXML(QIODevice *device) const
{
    QXmlStreamWriter xml(device);
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(4);
    xml.writeStartDocument("1.0");

    xml.writeStartElement("knot");
        xml.writeAttribute("version","2"); // note: version MUST be an integer

        xml.writeStartElement("style");
            xml.writeStartElement("stroke");
                xml.writeAttribute("width",QString::number(get_width()));
                xml.writeAttribute("color",get_brush().color().name());
                xml.writeAttribute("alpha",QString::number(get_brush().color().alpha()));
            xml.writeEndElement();
            xml.writeStartElement("outline");
                QPen pen = get_pen();
                xml.writeAttribute("width",QString::number(pen.widthF()));
                xml.writeAttribute("color",pen.color().name());
                xml.writeAttribute("alpha",QString::number(pen.color().alpha()));
                QString penstyle;
                switch ( pen.style() )
                {
                    default:
                    case Qt::NoPen:         penstyle = "nothing";       break;
                    case Qt::SolidLine:     penstyle = "solid";         break;
                    case Qt::DotLine:       penstyle = "dot";           break;
                    case Qt::DashLine:      penstyle = "dash";          break;
                    case Qt::DashDotLine:   penstyle = "dash_dot";      break;
                    case Qt::DashDotDotLine:penstyle = "dash_dot_dot";  break;
                }
                xml.writeAttribute("style",penstyle);
            xml.writeEndElement();
            xml.writeStartElement("cusp");
                styleinfo si = knot.get_style_info();
                xml.writeTextElement("style",knot_curve_styler::name(si.curve_style));
                xml.writeTextElement("min-angle",QString::number(si.cusp_angle));
                xml.writeTextElement("distance",QString::number(si.cusp_distance));
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
                xml.writeTextElement("gap",QString::number(si.crossing_distance));
                xml.writeTextElement("handle-length",QString::number(si.handle_length));
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
    if ( knot.attribute("version").toInt() > 2 )
        return false; // unknown version

    QDomElement graph = knot.firstChildElement("graph");
    if ( graph.isNull() )
        return false; // XML does not contain a graph description

    undo_stack.beginMacro(tr("Load Knot"));

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
        QColor stroke_col(stroke.attribute("color","black"));
        stroke_col.setAlpha(stroke.attribute("alpha","255").toInt());
        set_brush(stroke_col);

        QDomElement outline = style.firstChildElement("outline");
        QPen p;
        QColor pen_col(outline.attribute("color","gray"));
        pen_col.setAlpha(outline.attribute("alpha","255").toInt());
        p.setColor(pen_col);
        p.setWidth(outline.attribute("width","1").toDouble());

        QString pstyle_name = outline.attribute("style","solid");
        if ( pstyle_name == "solid" )
            p.setStyle ( Qt::SolidLine );
        else if ( pstyle_name == "dot" )
            p.setStyle ( Qt::DotLine );
        else if ( pstyle_name == "dash" )
            p.setStyle ( Qt::DashLine );
        else if ( pstyle_name == "dash_dot" )
            p.setStyle ( Qt::DashDotLine );
        else if ( pstyle_name == "dash_dot_dot" )
            p.setStyle ( Qt::DashDotDotLine );
        else
            p.setStyle ( Qt::NoPen );

        set_pen(p);

        QDomElement cusp = style.firstChildElement("cusp");
            styleinfo cusp_style_info;
            QDomElement cusp_style = cusp.firstChildElement("style");
            QString stylename =  cusp_style.isNull() ? "pointed" : cusp_style.text();
            cusp_style_info.curve_style = knot_curve_styler::idof(stylename);
            QDomElement cusp_angle = cusp.firstChildElement("min-angle");
            cusp_style_info.cusp_angle = cusp_angle.isNull() ? 225 : cusp_angle.text().toDouble();
            QDomElement cusp_dist = cusp.firstChildElement("distance");
            cusp_style_info.cusp_distance = cusp_angle.isNull() ? 24 : cusp_dist.text().toDouble();
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
            cusp_style_info.crossing_distance = egap.isNull() ? 10 : egap.text().toDouble();
            QDomElement handle_length = crossing.firstChildElement("handle-length");
            cusp_style_info.handle_length = handle_length.isNull() ? 10 : handle_length.text().toDouble();

            set_default_style(cusp_style_info);

    }

    return true;
}

#include <QStyleOptionGraphicsItem>
void KnotView::paint_knot(QPaintDevice *device, QRectF area, bool minimal )
{
    QPainter painter;
    painter.begin(device);

    paint_knot ( &painter, area, minimal );

    painter.end();
}

void KnotView::paint_knot(QPainter *painter, QRectF area, bool minimal)
{
    /// \todo find out how to draw only area
    QStyleOptionGraphicsItem opt;
    //opt.exposedRect = area; NOOP?
    //painter->setClipRect(area); Uh?
    //opt.rect = area.toRect(); NOOP?
    Q_UNUSED(area)

    QPointF off = knot.boundingRect().topLeft();
    painter->translate(-off.x(),-off.y()); // remove offset


    if ( minimal )
        painter->drawPath(knot.build());
    else
        knot.paint(painter,&opt);
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
    /// \bug pen width = 0 still renders outline (caused by Qt, width = 0 => 1px cosmetic )
    knot.setPen(p);
    redraw(false);
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

void KnotView::set_brush_color(QColor c)
{
    knot.setBrush(c);
    redraw(false);
}

QBrush KnotView::get_brush() const
{
    return knot.brush();
}

styleinfo KnotView::get_default_style() const
{
    return knot.get_style_info();
}

void KnotView::set_default_style(styleinfo si)
{
    knot.set_style_info ( si );
    redraw(true);
}
/*
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
*/
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

void KnotView::initialize_movement(Node *main)
{
    node_list selection = selected_nodes();
    if ( selection.size() < 2 )
        return;
    sel_offset.clear();
    last_node = main;
    QRectF boundbox;
    foreach(Node* n, selection)
    {
        QPointF off = n->pos()-main->pos();
        sel_offset[n] = off;

        if ( off.x() < boundbox.left() )
            boundbox.setLeft(off.x());
        else if ( off.x() > boundbox.right() )
            boundbox.setRight(off.x());

        if ( off.y() < boundbox.top() )
            boundbox.setTop(off.y());
        else if ( off.y() > boundbox.bottom() )
            boundbox.setBottom(off.y());
    }

    if ( grid.is_enabled() )
        sel_size = boundbox.width() / grid.get_size();
    else
        sel_size = 4;

    for ( QMap<Node*,QPointF>::iterator it = sel_offset.begin();
            it != sel_offset.end(); ++it )
        it.value() /= sel_size;
}


Node *KnotView::add_node(QPointF pos, node_list adjl)
{
    Node* nn = new Node(pos);
    undo_stack.push(new AddNode(nn,adjl,this));
    return nn;
}

Node* KnotView::add_node_or_break_edge(QPointF p,node_list adjl)
{
    Edge* edg = edge_at(p);
    Node* retnode;
    if ( edg )
    {
        Node* v1 = edg->vertex1();
        Node* v2 = edg->vertex2();
        QLineF oldline ( v1->pos(), v2->pos() );
        QLineF newline ( v1->pos(), p );
        newline.setAngle( oldline.angle() );

        undo_stack.beginMacro(tr("Break Edge"));
        adjl.push_back(v1);
        adjl.push_back(v2);
        retnode = add_node(newline.p2(),adjl);
        remove_edge(v1,v2);
        undo_stack.endMacro();
    }
    else
        retnode = add_node(p,adjl);

    scene()->clearSelection();
    retnode->setSelected(true);
    return retnode;
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

void KnotView::move_node(Node *n, QPointF dest)
{
    undo_stack.push(new MoveNode(n,n->pos(),dest,this));
}

void KnotView::move_nodes ( QPointF dest )
{
    QPointF delta = dest-startpos;
    if ( selected_nodes().size() > 1 )
    {
        undo_stack.beginMacro(tr("Move Nodes"));
        foreach ( Node* n, selected_nodes() )
        {
            undo_stack.push(new MoveNode(n,n->pos()-delta,n->pos(),this));
        }
        undo_stack.endMacro();
    }
    else if ( selected_nodes().size() == 1 )
    {
        undo_stack.push(new MoveNode(selected_nodes()[0],selected_nodes()[0]->pos()-delta,
                                     selected_nodes()[0]->pos(),this));
    }
}

void KnotView::cycle_edge(Edge* e)
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

void KnotView::cycle_edge_inverted(Edge *e)
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

void KnotView::set_edge_type(Node *n1, Node *n2, Edge::type_type type)
{
    Edge* e = n1->get_link(n2);
    if ( n1 && n2 && e )
        undo_stack.push(new ToggleEdge(n1,n2,e->type,type,this));
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

snapping_grid &KnotView::get_grid()
{
    return grid;
}

void KnotView::mode_edit_node_edge()
{
    mode = EDIT_NODE_EDGE;
    mode_change();
}

void KnotView::mode_edge_chain()
{
    mode = INSERT_EDGE_CHAIN;
    mode_change();
}

void KnotView::mode_move_grid()
{
    mode = MOVE_GRID;
    mode_change();
}

void KnotView::mode_moving_new()
{
    mode_edit_node_edge();

    node_list sn = selected_nodes();

    if ( sn.size() == 0 )
        return;

    mouse_status = MOVING;

    if ( grid.is_enabled() )
        QCursor::setPos( mapToGlobal( mapFromScene( sn[0]->pos() ) ) );

    oldpos = mapToScene ( mapFromGlobal ( QCursor::pos() ) );

    initialize_movement ( sn[0] );
}

void KnotView::erase_selected()
{
    //if ( mode == EDIT_NODE )
    {
        undo_stack.beginMacro(tr("Remove Nodes"));
        foreach( QGraphicsItem* node, scene()->selectedItems() )
        {
            remove_node ( dynamic_cast<Node*>(node) );
        }
        undo_stack.endMacro();
    }
}

void KnotView::link_selected()
{
    undo_stack.beginMacro(tr("Link Nodes"));
    typedef node_list::iterator iter;
    node_list sel = selected_nodes();
    for ( iter i = sel.begin(); i != sel.end(); ++i )
    {
        iter j = i;
        for ( ++j; j != sel.end(); ++j )
            add_edge(*i,*j);
    }
    undo_stack.endMacro();
}

void KnotView::unlink_selected()
{
    undo_stack.beginMacro(tr("Unlink Nodes"));
    typedef node_list::iterator iter;
    node_list sel = selected_nodes();
    for ( iter i = sel.begin(); i != sel.end(); ++i )
    {
        iter j = i;
        for ( ++j; j != sel.end(); ++j )
            remove_edge(*i,*j);
    }
    undo_stack.endMacro();
}


void KnotView::merge_selected()
{
    QPointF pos; // average position

    node_list links; // nodes linked to selection

    QList<QGraphicsItem*> selection = scene()->selectedItems();

    if ( selection.count() < 2 )
        return;

    undo_stack.beginMacro(tr("Merge Nodes"));

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

void KnotView::flip_horizontal()
{
    node_list sel = selected_nodes();

    if ( sel.size() < 2 )
        return;

    QPointF origin = sel[0]->pos();
    foreach ( Node* selnode, sel )
    {
        QLineF vector ( origin, selnode->pos() );
        vector.translate(-origin);
        vector.setP2(QPointF(-vector.p2().x(),vector.p2().y()));
        vector.translate(origin);
        selnode->setPos(vector.p2());
    }

    redraw(true);
}

void KnotView::flip_vertical()
{
    node_list sel = selected_nodes();

    if ( sel.size() < 2 )
        return;

    QPointF origin = sel[0]->pos();
    foreach ( Node* selnode, sel )
    {
        QLineF vector ( origin, selnode->pos() );
        vector.translate(-origin);
        vector.setP2(QPointF(vector.p2().x(),-vector.p2().y()));
        vector.translate(origin);
        selnode->setPos(vector.p2());
    }

    redraw(true);
}

void KnotView::toggle_knotline(bool visible)
{
    if ( visible && !knot.scene() )
        scene()->addItem(&knot);
    else if ( !visible && knot.scene() )
        scene()->removeItem(&knot);

}

void KnotView::toggle_graph(bool visible)
{
    dynamic_cast<GridScene*>(scene())->show_graph = visible;
}

