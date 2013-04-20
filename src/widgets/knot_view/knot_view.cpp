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

Knot_View::Knot_View(QString file)
    : mouse_mode(EDIT_GRAPH), last_node(nullptr)
{
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
}

void Knot_View::translate_view(QPointF delta)
{
    translate(delta);
    expand_scene_rect(10);
}

Node* Knot_View::add_node(QPointF pos)
{
    Node* node = new Node(pos);
    undo_stack.push(new Create_Node(node,this));
    return node;
}

Edge *Knot_View::add_edge(Node *n1, Node *n2)
{
    /// \todo centralized edge style management (in Resource_Manager)
    Edge* e = new Edge(n1,n2,new Edge_Normal);
    undo_stack.push(new Create_Edge(e,this));
    return e;
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

void Knot_View::expand_scene_rect(int margin)
{
    QRectF vp ( mapToScene(-margin,-margin),
                mapToScene(width()+2*margin,height()+2*margin));
    QRectF sr = sceneRect();
    if ( ! sr.contains(vp) )
    {
        QPoint p1 ( qMin(vp.left(),sr.left()),qMin(vp.top(),sr.top()) );
        QPoint p2 ( qMax(vp.right(),sr.right()),qMax(vp.bottom(),sr.bottom()) );
        setSceneRect(QRect(p1,p2));
    }
}

Node *Knot_View::node_at(QPointF p) const
{
    return dynamic_cast<Node*>(scene()->itemAt(p,QTransform()));
}



void Knot_View::mousePressEvent(QMouseEvent *event)
{
    QPoint mpos = event->pos();
    QPointF snapped_scene_pos = grid.nearest(mapToScene(mpos));

    if ( guide.scene() )
        scene()->removeItem(&guide);

    if ( mouse_mode & MOVE_BACK )
    {
        /// \todo
    }
    else
    {
        if ( event->buttons() & Qt::MiddleButton )
        {
            // drag view
            setCursor(Qt::ClosedHandCursor);
        }
        else if ( mouse_mode & EDIT_GRAPH && event->buttons() & Qt::LeftButton )
        {
            /// \todo drag selection
        }
        else if ( mouse_mode & EDIT_GRAPH || event->buttons() & Qt::RightButton )
        {
            scene()->clearSelection();
            last_node = nullptr;
            /// \todo begin rubberband
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

            undo_stack.beginMacro(title);

            if ( !next_node )/// \todo break existing edges
                next_node = add_node(snapped_scene_pos);

            if ( last_node && !next_edge )
                add_edge(last_node,next_node);

            undo_stack.push(new Last_Node(last_node,next_node,this));

            undo_stack.endMacro();

            if ( !guide.scene() )
            {
                guide.setLine(QLineF(last_node->pos(),snapped_scene_pos));
                scene()->addItem(&guide);
            }
        }

    }
    move_center = mpos;
}

void Knot_View::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mpos = event->pos();
    QPointF snapped_scene_pos = grid.nearest(mapToScene(mpos));

    /// \todo move grid/bg

    if ( event->buttons() & Qt::MiddleButton  )
    {
        QPointF delta = mpos-move_center;
        delta /= get_zoom_factor(); // take scaling into account
        translate_view(delta);
    }
    else if ( mouse_mode & EDIT_GRAPH && event->buttons() & Qt::LeftButton )
    {
        /// \todo drag selection
    }
    else if ( mouse_mode & EDIT_GRAPH || event->buttons() & Qt::RightButton )
    {
        /// \todo update rubberband
    }
    else if ( mouse_mode & EDGE_CHAIN )
    {
        if ( last_node )
            guide.setLine(QLineF(last_node->pos(),snapped_scene_pos));
    }

    move_center = mpos;
}

void Knot_View::mouseReleaseEvent(QMouseEvent *)
{
    setCursor(Qt::ArrowCursor);
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

void Knot_View::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect,backgroundBrush());
    /// \todo background image
    grid.render(painter,rect);
}
