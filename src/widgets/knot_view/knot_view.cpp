/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2013  Mattia Basaglia

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

#include "knot_view.hpp"
#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>
#include "commands.hpp"
#include <QApplication>
#include "resource_manager.hpp"

Knot_View::Knot_View(QString file)
    : mouse_mode(EDIT_GRAPH), last_node(nullptr)
{

    setWindowFilePath(file);
    setFrameStyle(StyledPanel|Plain);
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect ( -width(), -height(), width()*2, height()*2);
    setScene(scene);
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(NoAnchor);
    setResizeAnchor(AnchorViewCenter);

    scene->addItem(&graph);

    connect(horizontalScrollBar(),SIGNAL(valueChanged(int)),
            this,SLOT(update_scrollbars()));
    connect(verticalScrollBar(),SIGNAL(valueChanged(int)),
            this,SLOT(update_scrollbars()));

    guide.setPen(QPen(Qt::gray,1,Qt::DashLine));


    QColor bg = QApplication::palette().color(QPalette::Highlight);
    rubberband.setPen(QPen(bg));
    bg.setAlpha(128);
    rubberband.setBrush(bg);
    rubberband.setZValue(100);


    //setCacheMode(CacheNone);
    connect(&m_grid,SIGNAL(grid_changed()),scene,SLOT(invalidate()));
    connect(&bg_img,SIGNAL(changed()),scene,SLOT(invalidate()));

    connect(&graph,SIGNAL(style_changed()),scene,SLOT(invalidate()));

}



void Knot_View::translate_view(QPointF delta)
{
    translate(delta);
    expand_scene_rect(10);
}

Node* Knot_View::add_node(QPointF pos)
{
    Node* node = new Node(pos);
    push_command(new Create_Node(node,this));
    return node;
}

Node *Knot_View::add_breaking_node(QPointF pos)
{
    Edge* e = edge_at(pos);
    if ( !e )
        return add_node(pos);

    Node* v1 = e->vertex1();
    Node* v2 = e->vertex2();


    begin_macro(tr("Break Edge"));
    Node* new_node = add_node(e->snap(pos));
    add_edge(v1,new_node);
    add_edge(new_node,v2);
    remove_edge(e);
    end_macro();

    return new_node;
}

Edge *Knot_View::add_edge(Node *n1, Node *n2)
{
    Edge* e = new Edge(n1,n2,Resource_Manager::default_edge_style());
    push_command(new Create_Edge(e,this));
    return e;
}

void Knot_View::remove_edge(Edge *edge)
{
    push_command(new Remove_Edge(edge,this));
}

void Knot_View::begin_macro(QString name)
{
    macro_stack.push( new Knot_Macro(name,this,nullptr) );
}

void Knot_View::end_macro()
{
    if ( macro_stack.empty() )
        return;
    Knot_Macro* macro = macro_stack.pop();
    push_command(macro);
}

void Knot_View::push_command(Knot_Command *cmd)
{
    if ( !macro_stack.isEmpty() )
        cmd->set_parent(macro_stack.top());
    else
        undo_stack.push(cmd);
}

QList<Node *> Knot_View::selected_nodes() const
{
    QList<Node*> nl;
    foreach ( QGraphicsItem* it, scene()->selectedItems() )
    {
        Node* n = dynamic_cast<Node*>(it);
        if(n)
            nl.push_back(n);
    }
    return nl;
}


void Knot_View::zoom_view(double factor)
{
    QPoint mp = mapFromGlobal(QCursor::pos());
    QPointF sp1 = mapToScene(mp);
    scale(factor,factor);
    // Anchor on mouse
    if ( rect().contains(mp) )
    {
        QPointF sp2 = mapToScene(mp);
        translate(sp2-sp1);
    }
    expand_scene_rect();

    emit zoomed(100*get_zoom_factor());
}

void Knot_View::set_zoom(double factor)
{
    QTransform t ( factor,           transform().m12(), transform().m13(),
                   transform().m21(),factor,            transform().m23(),
                   transform().m31(),transform().m32(), transform().m33()
                   );
    setTransform(t);
    emit zoomed(100*factor);
}

void Knot_View::set_mode_edit_graph()
{
    mouse_mode = EDIT_GRAPH;
}

void Knot_View::set_mode_edge_chain()
{
    mouse_mode = EDGE_CHAIN;
}

void Knot_View::set_mode_move_grid()
{
    mouse_mode |= MOVE_GRID;
    setCursor(Qt::SizeAllCursor);
}

void Knot_View::set_mode_move_background()
{
    mouse_mode |= MOVE_BG_IMG;
    setCursor(Qt::SizeAllCursor);
}

void Knot_View::update_knot()
{
    graph.render_knot();
    scene()->invalidate();
}

void Knot_View::set_knot_colors(const QList<QColor> &l)
{
    push_command(new Change_Colors(graph.colors(),l,this));
}

void Knot_View::set_stroke_width(double w)
{
    push_command(new Knot_Width(graph.width(),w,this));
}

void Knot_View::expand_scene_rect(int margin)
{
    QRectF vp ( mapToScene(-margin,-margin),
                mapToScene(width()+2*margin,height()+2*margin));
    QRectF sr = sceneRect();
    if ( ! sr.contains(vp) )
    {
        setSceneRect(sr.united(vp));
        /*QPoint p1 ( qMin(vp.left(),sr.left()),qMin(vp.top(),sr.top()) );
        QPoint p2 ( qMax(vp.right(),sr.right()),qMax(vp.bottom(),sr.bottom()) );
        setSceneRect(QRect(p1,p2));*/
    }
}

bool Knot_View::mouse_select(QList<Node *> nodes, bool modifier, bool clear)
{
    bool select = true;
    if ( !modifier )
    {
        if ( clear )
            scene()->clearSelection();
    }
    else
    {
        select = false;
        foreach(Node* itm, nodes)
        {
            if ( !itm->isSelected() )
            {
                select = true;
                break;
            }
        }
    }


    foreach(Node* itm, nodes)
        itm->setSelected(select);

    return select;
}

Node *Knot_View::node_at(QPointF p) const
{
    return dynamic_cast<Node*>(scene()->itemAt(p,QTransform()));
}


Edge *Knot_View::edge_at(QPointF p) const
{
    return dynamic_cast<Edge*>(scene()->itemAt(p,QTransform()));
}

void Knot_View::mousePressEvent(QMouseEvent *event)
{
    QPoint mpos = event->pos();
    QPointF scene_pos = mapToScene(mpos);
    QPointF snapped_scene_pos = m_grid.nearest(scene_pos);

    if ( guide.scene() )
        scene()->removeItem(&guide);


    if ( event->buttons() & Qt::MiddleButton )
    {
        // drag view
        setCursor(Qt::ClosedHandCursor);
    }
    else if ( mouse_mode & MOVE_BACK)
    {
        mouse_mode &= ~MOVE_BACK;
    }
    else
    {
        if ( mouse_mode & EDIT_GRAPH && event->buttons() & Qt::LeftButton )
        {
            Node* n = node_at(scene_pos);
            Edge* e = edge_at(scene_pos);
            QList<Node*> nodes;
            if ( n )
            {
                last_node = n;
                nodes.push_back(n);
            }
            else if ( e )
            {
                last_node = e->vertex1();
                nodes.push_back(e->vertex1());
                nodes.push_back(e->vertex2());

            }
            else
                mouse_mode |= RUBBERBAND;

            if ( !(mouse_mode & RUBBERBAND) )
            {
                /// \todo there's something wrong with the selection
                // not directly in condition because has side-effect
                bool b = mouse_select(nodes,event->modifiers() &
                                      (Qt::ControlModifier|Qt::ShiftModifier),
                                      false );
                // move only if has selected, not if has deselected
                if ( b )
                {
                    mouse_mode |= MOVE_NODES;
                    node_mover = Node_Mover(selected_nodes(),last_node->pos());
                }
            }
        }
        else if ( mouse_mode & EDIT_GRAPH || event->buttons() & Qt::RightButton )
        {
            mouse_mode |= RUBBERBAND;
        }
        else if ( mouse_mode & EDGE_CHAIN  && event->button() == Qt::LeftButton )
        {
            Node* next_node = node_at(snapped_scene_pos);
            Edge* next_edge = last_node && next_node ? last_node->edge_to(next_node) : nullptr;

            QString title;

            if ( next_edge )
                title = tr("Move Ahead");
            else if ( !last_node )
                title = tr("Create Node");
            else
                title = tr("Add Edge");

            begin_macro(title);

            if ( !next_node )
                next_node = add_breaking_node(snapped_scene_pos);

            if ( last_node && !next_edge )
                add_edge(last_node,next_node);

            push_command(new Last_Node(last_node,next_node,this));

            end_macro();

            update_knot();

            //if ( !guide.scene() )
            {
                guide.setLine(QLineF(last_node->pos(),snapped_scene_pos));
                scene()->addItem(&guide);
            }
        }

        if ( mouse_mode & RUBBERBAND )
        {
            // drag rubberband selection
            last_node = nullptr;
            rubberband.setPos(mapToScene(mpos));
            rubberband.setRect(0,0,0,0);
            scene()->addItem(&rubberband);
        }

    }
    move_center = mpos;
}

void Knot_View::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mpos = event->pos();
    QPointF scene_pos = mapToScene(mpos);
    QPointF snapped_scene_pos = m_grid.nearest(scene_pos);

    if ( event->buttons() & Qt::MiddleButton  )
    {
        // drag view
        QPointF delta = mpos-move_center;
        delta /= get_zoom_factor(); // take scaling into account
        translate_view(delta);
    }
    else if ( mouse_mode & MOVE_BACK )
    {
        Node* n = node_at(scene_pos);
        Edge* e = edge_at(scene_pos);
        if ( mouse_mode & MOVE_GRID )
        {
            if ( n )
                m_grid.set_origin(n->pos());
            else if ( e )
                m_grid.set_origin(e->snap(scene_pos));
            else
                m_grid.set_origin(scene_pos);
        }
        else if ( mouse_mode & MOVE_BG_IMG )
        {
            if ( n )
                bg_img.set_position(n->pos());
            else if ( e )
                bg_img.set_position(e->snap(scene_pos));
            else
                bg_img.set_position(scene_pos);
        }

    }
    else if ( mouse_mode & RUBBERBAND )
    {
        // update rubberband
        rubberband.setRect(QRectF(QPointF(0,0),mapToScene(mpos)-rubberband.pos())
                            .normalized());
    }
    else if ( mouse_mode & MOVE_NODES )
    {
        // move selected nodes

        if ( last_node )
        {
            QPointF delta = snapped_scene_pos-last_node->pos();
            node_mover.move(delta);
            /// \todo if fluid refresh
            update_knot();
        }
    }
    else if ( mouse_mode & EDGE_CHAIN )
    {
        if ( last_node )
            guide.setLine(QLineF(last_node->pos(),snapped_scene_pos));
    }

    move_center = mpos;
    scene()->invalidate();
}


void Knot_View::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);

    if ( mouse_mode & RUBBERBAND )
    {
        // select from rubberband;
        scene()->removeItem(&rubberband);

        QPainterPath pp;

        mouse_select(nodes_in_rubberband(),
                     event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier) );

        mouse_mode &= ~RUBBERBAND;
    }
    else if ( mouse_mode & MOVE_NODES )
    {
        last_node = nullptr;
        mouse_mode &=~ MOVE_NODES;
        node_mover.deploy(this);
        update_knot();
    }
    else if ( mouse_mode & MOVE_BACK )
    {
        if ( event->button() == Qt::MiddleButton )
            setCursor(Qt::SizeAllCursor);
    }

}

void Knot_View::mouseDoubleClickEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        QPoint mpos = event->pos();
        QPointF scene_pos = mapToScene(mpos);
        QPointF snapped_scene_pos = m_grid.nearest(scene_pos);
        Node *node=node_at(scene_pos);

        if ( node )
            node->setPos(snapped_scene_pos);
        else
            add_breaking_node(snapped_scene_pos);

        update_knot();
    }
}

void Knot_View::wheelEvent(QWheelEvent *event)
{
    if ( event->modifiers() & Qt::ControlModifier )
    {
        if ( event->delta() < 0 )
            zoom_view(0.8);
        else
            zoom_view(1.25);
    }
}



QList<Node *> Knot_View::nodes_in_rubberband() const
{
    QList<QGraphicsItem*> items = scene()->items(QRectF(
                            rubberband.rect().translated(rubberband.pos())));
    QList<Node*> nodes;
    foreach ( QGraphicsItem* item, items)
    {
        Node* n = dynamic_cast<Node*>(item);
        if ( n )
            nodes.push_back(n);
    }
    return nodes;
}

void Knot_View::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect,backgroundBrush());
    bg_img.render(painter);
    m_grid.render(painter,rect);
}
