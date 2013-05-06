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


Node_Mover::Node_Mover(QList<Node *> nodes, QPointF pivot)
    : nodes(nodes), pivot(pivot), start_pos(pivot),
      scale_factor(1), scale_count(0), rotate_angle(0)
{
    offset.reserve(nodes.size());

    QPointF min, max;
    if ( ! nodes.empty() )
        min = max = nodes[0]->pos();

    foreach(Node* n,nodes)
    {
        offset.push_back(n->pos() - pivot);

        if ( n->x() < min.x() )
            min.setX(n->x());
        else if ( n->x() > max.x() )
            max.setX(n->x());
        if ( n->y() < min.y() )
            min.setY(n->y());
        else if ( n->y() > max.y() )
            max.setY(n->y());
    }

    m_initial_size.setWidth(max.x()-min.x());
    m_initial_size.setHeight(max.y()-min.y());
}

void Node_Mover::move(QPointF delta)
{
    pivot += delta;
    foreach(Node* n,nodes)
    {
        n->setPos(n->pos()+delta);
    }
}

void Node_Mover::rotate(double angle)
{
    rotate_angle += angle;
    foreach(Node* n,nodes)
    {
        QLineF ray(pivot,n->pos());
        ray.setAngle(ray.angle()+angle);
        n->setPos(ray.p2());
    }

}

void Node_Mover::scale(double factor)
{
    scale_factor *= factor;
    for ( int i = 0; i < nodes.size(); i++ )
    {
        QLineF ray(QPointF(0,0),offset[i]);
        ray.setLength(ray.length()*scale_factor);
        ray.setAngle(ray.angle()+rotate_angle);
        nodes[i]->setPos(pivot+ray.p2());
    }
}

void Node_Mover::fixed_scale(bool increase, double step_size)
{
    scale_count += increase ? +1 : -1;

    double factor = ( m_initial_size.width() + scale_count*step_size ) /
                                m_initial_size.width();
    scale_factor = factor;
    for ( int i = 0; i < nodes.size(); i++ )
    {
        QLineF ray(QPointF(0,0),offset[i]);
        ray.setLength(ray.length()*scale_factor);
        ray.setAngle(ray.angle()+rotate_angle);
        nodes[i]->setPos(pivot+ray.p2());
    }
}

void Node_Mover::deploy(Knot_View *view)
{
    if ( !qFuzzyCompare(pivot,start_pos) )
    {
        if ( nodes.size() > 1 )
        {
            view->begin_macro(QObject::tr("Move Nodes"));
            for ( int i = 0; i < nodes.size(); i++ )
            {
                Node* n = nodes[i];
                view->push_command(new Move_Node(n,offset[i]+start_pos,n->pos(),view));
            }
            view->end_macro();
        }
        else if ( nodes.size() == 1 )
        {
            Node* n = nodes[0];
            view->push_command(new Move_Node(n,offset[0]+start_pos,n->pos(),view));
        }
    }

    nodes.clear();
    offset.clear();
}
