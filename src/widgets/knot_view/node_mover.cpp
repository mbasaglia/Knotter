/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#include "node_mover.hpp"
#include "knot_view.hpp"
#include "commands.hpp"
#include "point_math.hpp"


Node_Mover::Node_Mover()
    : scale_factor(1), scale_count(0), rotate_angle(0), dragged_handle(nullptr)
{
    for ( int i = 0; i < n_handles; i++ )
    {
        transform_handles[i].set_image_angle(90*i);
        transform_handles[i].setVisible(false);
    }
}


void Node_Mover::set_nodes(QList<Node *> nodes)
{
    m_nodes = nodes;


    QPointF min, max;
    if ( ! m_nodes.empty() )
        min = max = m_nodes[0]->pos();

    foreach(Node* n,m_nodes)
    {

        if ( n->x() < min.x() )
            min.setX(n->x());
        else if ( n->x() > max.x() )
            max.setX(n->x());
        if ( n->y() < min.y() )
            min.setY(n->y());
        else if ( n->y() > max.y() )
            max.setY(n->y());
    }
    m_initial_box.setTopLeft(min);
    m_initial_box.setBottomRight(max);

    if ( ! m_nodes.empty() )
        initialize_movement(pivot);//m_nodes[0]->pos());
    update_transform_handles();
}

void Node_Mover::update_transform_handles()
{


    transform_handles[0].setPos(m_initial_box.topLeft());
    transform_handles[1].setPos(m_initial_box.bottomLeft());
    transform_handles[2].setPos(m_initial_box.bottomRight());
    transform_handles[3].setPos(m_initial_box.topRight());

    for ( int i = 0; i < n_handles; i++ )
    {
        transform_handles[i].setVisible(nodes().size() >= 3);
        if ( scale_factor < 0 )
            transform_handles[i].set_angle(rotate_angle+180);
        else
            transform_handles[i].set_angle(rotate_angle);

        QPointF off = transform_handles[i].pos()-start_pos;
        QLineF ray(QPointF(0,0),off);
        ray.setLength(ray.length()*scale_factor);
        ray.setAngle(ray.angle()+rotate_angle);

        transform_handles[i].setPos(pivot+ray.p2());
    }
}

void Node_Mover::add_handles_to_scene(QGraphicsScene *scene)
{
    for ( int i = 0; i < n_handles; i++ )
        scene->addItem(&transform_handles[i]);
}

void Node_Mover::set_mode(Transform_Handle::Mode mode)
{
    for ( int i = 0; i < n_handles; i++ )
    {
        transform_handles[i].set_mode(mode);
        transform_handles[i].update();
    }
}

void Node_Mover::set_dragged_handle(Transform_Handle *handle, bool anchor_angle)
{
    dragged_handle = handle;

    QPointF sp = m_initial_box.center();

    if ( anchor_angle )
    {
        for ( int i = 0; i < n_handles; i++ )
            if ( &transform_handles[i] == dragged_handle )
            {
                switch(i)
                {
                    case 0: sp = m_initial_box.bottomRight(); break;
                    case 1: sp = m_initial_box.topRight(); break;
                    case 2: sp = m_initial_box.topLeft(); break;
                    case 3: sp = m_initial_box.bottomLeft(); break;
                }
                break;
            }
    }

    initialize_movement_internal(sp);
}

void Node_Mover::drag_handle(QPointF p, bool fixed, double step_size )
{
    if ( !dragged_handle )
        return;

    QLineF l1 (pivot,dragged_handle->pos());
    QLineF l2 (pivot,p);

    if ( mode() == Transform_Handle::ROTATE )
    {
        double angle = l2.angle()-l1.angle();
        if ( fixed )
            angle = int(angle) / 15 * 15;
        rotate(angle);
    }
    else if ( qAbs( l2.angle() - l1.angle()) > 90 )
    {
        scale(-1);
    }
    else if ( fixed )
    {
        if ( qAbs(l2.length() - l1.length()) > step_size )
        {
            fixed_scale(l2.length() > l1.length(),step_size);
        }
    }
    else
    {
        scale(l2.length()/l1.length());
    }

}

QCursor Node_Mover::current_handle_cursor()
{
    if ( dragged_handle && mode() == Transform_Handle::SCALE )
    {
        if ( dragged_handle == transform_handles+0 ||
             dragged_handle == transform_handles+2 )
            return Qt::SizeFDiagCursor;
        else
            return Qt::SizeBDiagCursor;
    }
    return Qt::ArrowCursor;
}

void Node_Mover::initialize_movement_internal(QPointF pivot)
{
    start_pos = this->pivot = pivot;

    scale_factor = 1;
    scale_count = 0;
    rotate_angle = 0;

    offset.clear();
    offset.reserve(m_nodes.size());

    foreach(Node* n,m_nodes)
        offset.push_back(n->pos() - pivot);
}

void Node_Mover::initialize_movement(QPointF pivot)
{
    initialize_movement_internal(pivot);
    update_transform_handles();
}

void Node_Mover::move(QPointF delta)
{
    pivot += delta;
    foreach(Node* n,m_nodes)
    {
        n->setPos(n->pos()+delta);
    }


    update_transform_handles();
    //for ( int i = 0; i < n_handles; i++ )
    //    transform_handles[i].setPos(transform_handles[i].pos()+delta);
}

void Node_Mover::set_pos(QPointF p)
{
    move(p-pivot);
}

void Node_Mover::rotate(double angle)
{
    rotate_angle += angle;
    foreach(Node* n,m_nodes)
    {
        QLineF ray(pivot,n->pos());
        ray.setAngle(ray.angle()+angle);
        n->setPos(ray.p2());
    }
    update_transform_handles();
}

void Node_Mover::scale(double factor)
{
    scale_factor *= factor;
    for ( int i = 0; i < m_nodes.size(); i++ )
    {
        QLineF ray(QPointF(0,0),offset[i]);
        ray.setLength(ray.length()*scale_factor);
        ray.setAngle(ray.angle()+rotate_angle);
        m_nodes[i]->setPos(pivot+ray.p2());
    }
    update_transform_handles();
}

void Node_Mover::fixed_scale(bool increase, double step_size)
{
    scale_count += increase ? +1 : -1;

    double factor = ( m_initial_box.width() + scale_count*step_size ) /
                                m_initial_box.width();
    scale_factor = factor;
    for ( int i = 0; i < m_nodes.size(); i++ )
    {
        QLineF ray(QPointF(0,0),offset[i]);
        ray.setLength(ray.length()*scale_factor);
        ray.setAngle(ray.angle()+rotate_angle);
        m_nodes[i]->setPos(pivot+ray.p2());
    }
    update_transform_handles();
}

void Node_Mover::deploy(Knot_View *view,QString message)
{
    if ( !qFuzzyCompare(pivot,start_pos) || !qFuzzyCompare(rotate_angle+1,1)
         || !qFuzzyCompare(scale_factor,1) )
    {
        if ( m_nodes.size() > 1 )
        {
            view->begin_macro(message);
            for ( int i = 0; i < m_nodes.size(); i++ )
            {
                Node* n = m_nodes[i];
                view->push_command(new Move_Node(n,offset[i]+start_pos,n->pos(),view));
            }
            view->end_macro();
        }
        else if ( m_nodes.size() == 1 )
        {
            Node* n = m_nodes[0];
            view->push_command(new Move_Node(n,offset[0]+start_pos,n->pos(),view));
        }
    }
    //initialize_movement(pivot);

    /*m_nodes.clear();
    offset.clear();


    start_pos = pivot;
    scale_factor = 1;
    scale_count = 0;
    rotate_angle = 0;*/
}

