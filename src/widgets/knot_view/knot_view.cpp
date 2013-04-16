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

    connect(horizontalScrollBar(),SIGNAL(valueChanged(int)),
            this,SLOT(update_scrollbars()));
    connect(verticalScrollBar(),SIGNAL(valueChanged(int)),
            this,SLOT(update_scrollbars()));
}

void Knot_View::translate_view(QPointF delta)
{
    translate(delta);
    expand_scene_rect(10);
}

void Knot_View::add_node(QPointF pos)
{
    undo_stack.push(new Create_Node(new Node(pos,&graph),this));
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



void Knot_View::mousePressEvent(QMouseEvent *event)
{
    if ( event->button() == Qt::LeftButton )
    {
        add_node(mapToScene(event->pos()));
    }
    else if ( event->buttons() & Qt::MiddleButton )
    {
        setCursor(Qt::ClosedHandCursor);
    }
    move_center = event->pos();
}

void Knot_View::mouseMoveEvent(QMouseEvent *event)
{
    if ( event->buttons() & Qt::MiddleButton  )
    {
        QPointF delta = event->pos()-move_center;
        delta /= matrix().m11(); // take scaling into account
        translate_view(delta);
        move_center = event->pos();
    }
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
