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

KnotView::KnotView( QWidget* parent )
    : QGraphicsView(parent), mode(EDIT_NODE_EDGE),
    last_node(NULL),
    mouse_status(DEFAULT),
    guide(NULL), rubberband(NULL),
    fluid_redraw(true),
    h_tl(TH::TOP|TH::LEFT),
    h_bl(TH::BOTTOM|TH::LEFT),
    h_tr(TH::TOP|TH::RIGHT),
    h_br(TH::BOTTOM|TH::RIGHT),
    dragged(NULL),
    saved(false)
{
    setScene ( new QGraphicsScene );
    setSceneRect ( 0, 0, width(), height());
    mode_edit_node_edge();
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor( QGraphicsView::NoAnchor );
    scene()->addItem(&knot);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setCacheMode(CacheBackground);
    connect(scene(),SIGNAL(selectionChanged()),SLOT(update_transform_handles()));
    connect(scene(),SIGNAL(selectionChanged()),SIGNAL(selectionChanged()));

    grid.enable(false);
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
    node->setPos(pos);
    if ( node->isSelected() )
        update_transform_handles();
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

        if ( (dragged=th_at(p)) )
        {
            mouse_status = TRANSFORMING;
            QPointF midpoint;
            QRectF boundbox = bound_box();

            if ( dragged->get_mode() == TH::ROTATE )
            {
                node_list sel = selected_nodes();
                foreach(Node*n,sel)
                    midpoint += n->pos();
                midpoint /= sel.size();
            }
            else if ( event->modifiers() & Qt::ShiftModifier )
                midpoint = boundbox.center();
            else if ( dragged->get_position() == (TH::TOP|TH::LEFT) )
                midpoint = boundbox.topLeft();
            else if ( dragged->get_position() == (TH::TOP|TH::RIGHT) )
                midpoint = boundbox.topRight();
            else if ( dragged->get_position() == (TH::BOTTOM|TH::LEFT) )
                midpoint = boundbox.bottomLeft();
            else if ( dragged->get_position() == (TH::BOTTOM|TH::RIGHT) )
                midpoint = boundbox.bottomRight();

            initialize_movement(midpoint);
        }
        else if ( node )
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
                initialize_movement(node->pos());
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
                startpos  = p;
                initialize_movement(edge->vertex1()->pos());
            }
        }
        else if ( !( event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier) ) )
            scene()->clearSelection();
    }
    else if ( event->button() == Qt::MidButton )
    {
        move_center = event->pos();
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

    redraw(false);
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

    if ( mouse_status == MOVING || mouse_status == PLACING )
    {
        snap(p,event);
        foreach(Node* n,selected_nodes())
        {
            n->setPos(n->pos()+p-move_center);
        }

        move_center = p;

        if (fluid_redraw)
        {
            update_transform_handles();
            knot.build();
        }
    }
    else if ( mouse_status == SELECTING )
    {
        rubberband->setRect(QRectF(QPointF(0,0),p-rubberband->pos()));
    }
    else if ( mouse_status == DRAGGING ) // drag view
    {
        QPointF delta = move_center-event->pos();
        delta /= matrix().m11(); // take scaling into account
        QRectF r = sceneRect();
        r.translate(delta);
        setSceneRect ( r );
        move_center = event->pos();
        setCursor(Qt::SizeAllCursor);
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
    else if ( mouse_status == TRANSFORMING && dragged )
    {
        if ( dragged->get_mode() == TH::ROTATE )
        {
            double angle = QLineF(move_center,p).angle() - QLineF(move_center,dragged->pos()).angle();
            if ( event->modifiers() & Qt::ControlModifier )
                angle = qRound64 ( angle / 15 ) * 15;
            foreach ( Node* selnode, selected_nodes() )
            {

                QLineF vectorbase ( QPointF(0,0), sel_offset[selnode] );

                QLineF vector ( move_center, selnode->pos() );
                vector.setAngle(vectorbase.angle()+angle);
                selnode->setPos(vector.p2());
            }

            if (fluid_redraw)
            {
                knot.build();
            }
        }
        else
        {
            QLineF newl(move_center,p);
            QLineF oldl(move_center,dragged->pos());

            /// \todo fix flipping

            //bool flipped = false;

            if ( grid.is_enabled() )
            {
                if ( abs(newl.length()-oldl.length()) < grid.get_size() )
                    return;
                else
                     /*flipped =*/ fixed_scale ( newl.length() > oldl.length() );
            }
            else
            {

                double scale = newl.length() / oldl.length();

                if ( newl.length() < 32 )
                {
                    /*flipped = true;
                    scale = -1;*/return;
                }

                foreach ( Node* selnode, selected_nodes() )
                {
                    QLineF oldln(move_center,selnode->pos());
                    oldln.setLength(oldln.length()*scale);
                    selnode->setPos(oldln.p2());
                }
            }


            /*if ( flipped )
            {
                if ( dragged->get_position() == (TH::TOP|TH::LEFT) )
                    dragged = &h_br;
                else if ( dragged->get_position() == (TH::TOP|TH::RIGHT) )
                    dragged = &h_bl;
                else if ( dragged->get_position() == (TH::BOTTOM|TH::LEFT) )
                    dragged = &h_tr;
                else if ( dragged->get_position() == (TH::BOTTOM|TH::RIGHT) )
                    dragged = &h_tl;
            }*/

            if (fluid_redraw)
            {
                update_transform_handles();
                knot.build();
            }
        }

        setCursor(dragged->cursor());
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

    if ( dragged )
        dragged->highlight = true;

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
    }
    else if ( mouse_status == MOVING )
    {
        snap(p,event);
        Node *node=node_at(p);
        if ( node )
            p = node->pos();

        if ( p != startpos )
            move_nodes();
        sel_offset.clear();

        // ugly fix for a weird bug
        foreach(Node*n,selected_nodes())
            foreach(Edge*e,n->links())
            {
                scene()->removeItem(e);
                scene()->addItem(e);
            }
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
    else if ( mouse_status == TRANSFORMING )
    {
        dragged = NULL;
        if ( h_tl.get_mode() == TH::ROTATE )
            undo_stack.beginMacro(tr("Rotate"));
        else
            undo_stack.beginMacro(tr("Scale"));
        move_nodes();
        undo_stack.endMacro();
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

    if ( mode == INSERT_EDGE_CHAIN )
        setCursor(Qt::CrossCursor);
    else
        setCursor(Qt::ArrowCursor);

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
    else if ( mouse_status == MOVING || mouse_status == PLACING )
    {

        node_list sel = selected_nodes();
        if ( sel.size() > 1 )
        {

            if ( event->modifiers() & Qt::ShiftModifier )
            {
                double angle = ( event->delta() < 0 ? -15 : +15 );
                //QPointF origin = sel[0]->pos();
                foreach ( Node* selnode, sel )
                {
                    QLineF vector ( move_center, selnode->pos() );
                    vector.setAngle(vector.angle()+angle);
                    selnode->setPos(vector.p2());
                }
            }
            else
            {
                fixed_scale ( event->delta() >  0 );
            }
            update_transform_handles();

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

/**
    \brief Scale selection up or down by 1 grid unit (based on sel_size)

    \return whether it has been flipped
*/
bool KnotView::fixed_scale(bool grow)
{
    bool flipped = false;

    int delta = grow ? 1 : -1;
    sel_size += delta;
    if ( qFuzzyCompare(sel_size+1,1) )
    {
        sel_size = delta;
        flipped = true;
    }
    foreach ( Node* selnode, selected_nodes() )
    {
        QLineF vector ( QPointF(0,0), sel_offset[selnode] );
        vector.setLength(vector.length()*sel_size/initial_sel_size);
        vector.translate(move_center);
        selnode->setPos(vector.p2());
    }

    return flipped;
}

QRectF KnotView::bound_box() const
{
    QRectF boundbox;
    boundbox.setTop(INT_MIN);
    boundbox.setLeft(INT_MIN);
    boundbox.setBottom(INT_MAX);
    boundbox.setRight(INT_MAX);
    foreach ( Node* n, selected_nodes() )
    {
        if ( n->x() > boundbox.left() )
            boundbox.setLeft(n->x());
        if ( n->x() < boundbox.right() )
            boundbox.setRight(n->x());
        if ( n->y() > boundbox.top() )
            boundbox.setTop(n->y());
        if ( n->y() < boundbox.bottom() )
            boundbox.setBottom(n->y());
    }
    return boundbox.normalized();
}

void KnotView::initialize_movement(QPointF center)
{

    node_list selection = selected_nodes();
    sel_offset.clear();
    startpos  = move_center = center;
    foreach(Node* n, selection)
    {
        QPointF off = n->pos() - move_center;
        sel_offset[n] = off;
    }

    if ( grid.is_enabled() )
        sel_size = bound_box().width() / grid.get_size();
    else
        sel_size = 4;

    initial_sel_size = sel_size;
}

Node *KnotView::node_at(QPointF p)
{
    return dynamic_cast<Node*>(scene()->itemAt(p));
}

Edge *KnotView::edge_at(QPointF p)
{
    return dynamic_cast<Edge*>(scene()->itemAt(p));
}

Transform_Handle *KnotView::th_at(QPointF p)
{
    return dynamic_cast<Transform_Handle*>(scene()->itemAt(p));
}

void KnotView::link(Node *a, Node *b )
{
    Edge* e = knot.add_edge(a,b);
    if ( e )
        scene()->addItem ( e );
    redraw(true);
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
        //scene()->removeItem(already_there);
        knot.remove_edge(already_there);
        delete already_there;
        redraw(true);
    }
}

void KnotView::do_toggle_edge(Node *a, Node *b, Edge::type_type type)
{
    knot.set_edge_type(a,b,type);
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

void KnotView::reload_graph()
{
    undo_stack.beginMacro(tr("Load Knot"));

    load_graph(knot);

    undo_stack.endMacro();
    undo_stack.setClean();
}

void KnotView::load_graph(const KnotGraph &knot_graph)
{
    if ( &knot_graph != &knot )
        knot = knot_graph;

    foreach(Node* n,knot.get_nodes())
    {
        if ( n->scene() != scene() )
        {
            add_node(n);
            styleinfo si = n->get_custom_style().default_to(get_default_style());
            si.enabled_style = n->get_custom_style().enabled_style;
            n->set_custom_style(si);
        }
    }
    foreach(Edge* e,knot.get_edges())
    {
        if ( e->scene() != scene() )
        {
            scene()->addItem(e);
            add_edge(e->vertex1(),e->vertex2());
        }
        set_edge_type(e,e->type);
    }

}

void KnotView::same_mode(const KnotView &other)
{
    mode = other.mode;
    h_tl.set_mode(other.h_tl.get_mode());
    h_tr.set_mode(other.h_tr.get_mode());
    h_bl.set_mode(other.h_bl.get_mode());
    h_br.set_mode(other.h_br.get_mode());
    grid.enable(other.grid.is_enabled());
    set_cache_mode(other.get_cache_mode());
    fluid_redraw = other.fluid_redraw;

    toggle_knotline ( other.knot.scene() != NULL );
}


void KnotView::do_set_width(double w)
{
    knot.set_width(w);
    redraw(true);
}

void KnotView::do_set_pen(QPen p)
{
    knot.setPen(p);
    redraw(false);
}
void KnotView::do_set_brush(QBrush b)
{
    knot.setBrush(b);
    redraw(false);
}

void KnotView::do_set_default_style(styleinfo si)
{
    knot.set_style_info ( si );
    redraw(true);
}


void KnotView::do_set_join_style(Qt::PenJoinStyle pjs)
{
    knot.set_join_style(pjs);
    redraw(true);
}

void KnotView::set_cache_mode(QGraphicsItem::CacheMode cm)
{
    knot.setCacheMode(cm);
}

double KnotView::get_width() const
{
    return knot.get_width();
}

QPen KnotView::get_pen() const
{
    return knot.pen();
}


QBrush KnotView::get_brush() const
{
    return knot.brush();
}

styleinfo KnotView::get_default_style() const
{
    return knot.get_style_info();
}

Qt::PenJoinStyle KnotView::get_join_style() const
{
    return knot.get_join_style();
}

QGraphicsItem::CacheMode KnotView::get_cache_mode() const
{
    return knot.cacheMode();
}

void KnotView::disable_custom_style(Node *n)
{
    undo_stack.beginMacro( tr("Remove custom node style") );
    undo_stack.push(new ChangeCustomNodeStyle(n,styleinfo(),this) );
    undo_stack.endMacro();
}

void KnotView::set_custom_style(node_list n, styleinfo sty)
{
    undo_stack.push(new ChangeMultiNodeStyle(n,sty,this) );
}

void KnotView::set_custom_style(Node *n, styleinfo sty)
{
    undo_stack.push(new ChangeCustomNodeStyle(n,sty,this) );
}

void KnotView::set_join_style(Qt::PenJoinStyle pjs)
{
    undo_stack.push(new ChangeKnotJoinStyle(get_join_style(),pjs,this));
}

void KnotView::set_default_style(styleinfo si)
{
    undo_stack.push(new ChangeDefaultNodeStyle(get_default_style(),si,this));
}

void KnotView::set_brush(QBrush b)
{
    undo_stack.push(new ChangeKnotBrush(get_brush(),b,this));
}

void KnotView::set_brush_color(QColor c)
{
    set_brush(c);
}

void KnotView::set_pen(QPen p)
{
    undo_stack.push(new ChangeKnotPen(get_pen(),p,this));
}

void KnotView::set_width(double w)
{
    undo_stack.push(new ChangeKnotWidth(get_width(),w,this));
}

void KnotView::accept_brush()
{
    undo_stack.push(new ChangeKnotBrush(get_brush(),get_brush(),true,this));
}

void KnotView::accept_width()
{
    undo_stack.push(new ChangeKnotWidth(get_width(),get_width(),true,this));
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

void KnotView::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect,backgroundBrush());
    backimg.render(painter);
    grid.render(painter,rect);
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

void KnotView::break_edge_intersections(Edge *e)
{
    if ( !e ) return;

    QList<QGraphicsItem *> intersected_raw = e->collidingItems();
    QList<Edge*> intersected;

    Node* v1 = e->vertex1();
    Node* v2 = e->vertex2();

    foreach ( QGraphicsItem* itm, intersected_raw )
    {
        Edge* edg = dynamic_cast<Edge*>(itm);
        if ( edg && !edg->is_vertex(v1) && !edg->is_vertex(v2) )
            intersected.push_back(edg);
    }

    if ( intersected.empty() )
        return;

    undo_stack.beginMacro("Break edge");

    Node* last = v1;

    QLineF start_line ( v1->pos(), v2->pos() );
    remove_edge ( v1, v2 );

    foreach ( Edge*o, intersected )
    {
        Node* o1 = o->vertex1();
        Node* o2 = o->vertex2();
        QLineF intersect_line ( o1->pos(), o2->pos() );
        QPointF intersect_point;
        start_line.intersect(intersect_line,&intersect_point);
        remove_edge(o1,o2);
        node_list nadj = node_list() << last << o1 << o2;
        last = add_node(intersect_point,nadj);
    }
    add_edge(last,v2);


    undo_stack.endMacro();

}

void KnotView::break_edge_equal(Edge *e, int segments)
{
    if ( !e || segments < 2 ) return;

    undo_stack.beginMacro("Break edge");

    Node* v1 = e->vertex1();
    Node* v2 = e->vertex2();

    QLineF line ( v1->pos(), v2->pos() );

    remove_edge ( v1, v2 );

    line.setLength(line.length()/segments);
    Node* last = v1;
    for ( int i = 1; i < segments; i++ )
    {
        last = add_node(line.p2(),node_list()<<last);
        line.translate(line.p2()-line.p1());
    }
    add_edge(last,v2);

    undo_stack.endMacro();

}

void KnotView::move_nodes ( )
{
    if ( selected_nodes().size() > 1 )
    {
        undo_stack.beginMacro(tr("Move Nodes"));
        foreach ( Node* n, selected_nodes() )
        {
            undo_stack.push(new MoveNode(n,sel_offset[n]+startpos,n->pos(),this));
        }
        undo_stack.endMacro();
    }
    else if ( selected_nodes().size() == 1 )
    {
        Node* n = selected_nodes()[0];
        undo_stack.push(new MoveNode(n,sel_offset[n]+startpos,n->pos(),this));
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

bool KnotView::has_selection()
{
    return !scene()->selectedItems().isEmpty();
}

snapping_grid &KnotView::get_grid()
{
    return grid;
}

background_image &KnotView::background()
{
    return backimg;
}

void KnotView::mode_edit_node_edge()
{
    mode = EDIT_NODE_EDGE;
    mode_change();
    setCursor(Qt::ArrowCursor);
}

void KnotView::mode_edge_chain()
{
    mode = INSERT_EDGE_CHAIN;
    mode_change();
    setCursor(Qt::CrossCursor);
}

void KnotView::mode_move_grid()
{
    mode = MOVE_GRID;
    mode_change();
    setCursor(Qt::CrossCursor);
}

void KnotView::mode_moving_new(QPointF center)
{
    mode_edit_node_edge();

    node_list sn = selected_nodes();

    if ( sn.size() == 0 )
        return;

    mouse_status = PLACING;

    move_center = mapToScene ( mapFromGlobal ( QCursor::pos() ) );

    initialize_movement ( center  );

    setCursor(Qt::DragCopyCursor);
}

void KnotView::transform_mode_scale()
{
    h_tl.set_mode(TH::SCALE);
    h_tr.set_mode(TH::SCALE);
    h_bl.set_mode(TH::SCALE);
    h_br.set_mode(TH::SCALE);
    redraw(false);
}

void KnotView::transform_mode_rotate()
{
    h_tl.set_mode(TH::ROTATE);
    h_tr.set_mode(TH::ROTATE);
    h_bl.set_mode(TH::ROTATE);
    h_br.set_mode(TH::ROTATE);
    redraw(false);
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
    QMap<Node*,Edge::type_type> types; // link types

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
                if ( !o->isSelected() )
                {
                    links.push_back(o);
                    types[o] = e->type;
                }
            }
            remove_node(n,adjacency_list);
        }
    }

    Node* mid = add_node(pos/selection.count(),links);
    foreach ( Node* o, links )
        set_edge_type(mid,o,types[o]);

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
    if ( recalculate_node && knot.scene() )
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
    emit zoom_changed(matrix().m11()*100);
}

void KnotView::reset_zoom()
{
    QMatrix trans = matrix();
    trans.setMatrix(1,trans.m12(),trans.m21(),1,trans.dx(),trans.dy());
    setMatrix(trans);
}

void KnotView::set_zoom(double percent)
{
    QMatrix trans = matrix();
    trans.setMatrix(percent/100,trans.m12(),trans.m21(),percent/100,trans.dx(),trans.dy());
    setMatrix(trans);
}

void KnotView::flip_horizontal()
{
    node_list sel = selected_nodes();

    if ( sel.size() < 2 )
        return;

    undo_stack.beginMacro(tr("Horizontal Flip"));

    QPointF origin = bound_box().center();
    foreach ( Node* selnode, sel )
    {
        QLineF vector ( origin, selnode->pos() );
        vector.translate(-origin);
        vector.setP2(QPointF(-vector.p2().x(),vector.p2().y()));
        vector.translate(origin);
        undo_stack.push ( new MoveNode(selnode, selnode->pos(),vector.p2(),this) );
    }

    undo_stack.endMacro();

    redraw(true);
}

void KnotView::flip_vertical()
{
    node_list sel = selected_nodes();

    if ( sel.size() < 2 )
        return;

    undo_stack.beginMacro(tr("Vertical Flip"));

    QPointF origin = bound_box().center();
    foreach ( Node* selnode, sel )
    {
        QLineF vector ( origin, selnode->pos() );
        vector.translate(-origin);
        vector.setP2(QPointF(vector.p2().x(),-vector.p2().y()));
        vector.translate(origin);
        undo_stack.push ( new MoveNode(selnode, selnode->pos(),vector.p2(),this) );
    }

    undo_stack.endMacro();

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
    CustomItem::show_graph = visible;
    redraw(false);
}

void KnotView::update_transform_handles()
{
    if ( scene()->selectedItems().count() < 3 )
    {
        if ( h_tl.scene() )
        {
            scene()->removeItem(&h_tl);
            scene()->removeItem(&h_bl);
            scene()->removeItem(&h_tr);
            scene()->removeItem(&h_br);
        }
        return;
    }

    if ( !h_tl.scene() )
    {
        scene()->addItem(&h_tl);
        scene()->addItem(&h_bl);
        scene()->addItem(&h_tr);
        scene()->addItem(&h_br);
    }
    QRectF boundbox = bound_box();
    boundbox.setTop(boundbox.top()-16);
    boundbox.setLeft(boundbox.left()-16);
    boundbox.setBottom(boundbox.bottom()+16);
    boundbox.setRight(boundbox.right()+16);
    h_tl.setPos(boundbox.bottomRight());
    h_tr.setPos(boundbox.bottomLeft());
    h_bl.setPos(boundbox.topRight());
    h_br.setPos(boundbox.topLeft());
}

