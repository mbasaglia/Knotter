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

#include "node_mover.hpp"
#include "knot_view.hpp"
#include "commands.hpp"
#include "point_math.hpp"


Node_Mover::Node_Mover()
    : scale_factor(1), scale_count(0), rotate_angle(0)
{
    for ( int i = 0; i < n_handles; i++ )
        transform_handles[i].set_image_angle(90*i);
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
    initial_box.setTopLeft(min);
    initial_box.setBottomRight(max);

    update_transform_handles();
}

void Node_Mover::update_transform_handles()
{
    for ( int i = 0; i < n_handles; i++ )
        transform_handles[i].setVisible(nodes().size() >= 3);
    const QPointF p;// = pivot-initial_box.center();
    transform_handles[0].setPos(p+initial_box.topLeft()*scale_factor);
    transform_handles[1].setPos(p+initial_box.bottomLeft()*scale_factor);
    transform_handles[2].setPos(p+initial_box.bottomRight()*scale_factor);
    transform_handles[3].setPos(p+initial_box.topRight()*scale_factor);
}

void Node_Mover::add_handles_to_scene(QGraphicsScene *scene)
{
    for ( int i = 0; i < n_handles; i++ )
        scene->addItem(&transform_handles[i]);
}

void Node_Mover::initialize_movement(QPointF pivot)
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

void Node_Mover::move(QPointF delta)
{
    pivot += delta;
    foreach(Node* n,m_nodes)
    {
        n->setPos(n->pos()+delta);
    }

    for ( int i = 0; i < n_handles; i++ )
        transform_handles[i].setPos(transform_handles[i].pos()+delta);
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

    double factor = ( initial_box.width() + scale_count*step_size ) /
                                initial_box.width();
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

void Node_Mover::deploy(Knot_View *view)
{
    if ( !qFuzzyCompare(pivot,start_pos) )
    {
        if ( m_nodes.size() > 1 )
        {
            view->begin_macro(QObject::tr("Move Nodes"));
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

    m_nodes.clear();
    offset.clear();
}

