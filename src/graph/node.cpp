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
#include "node.hpp"
#include "edge.hpp"
#include "graph.hpp"
#include "resource_manager.hpp"

void Node::set_position(QPointF p)
{
    pos = p;
    emit moved(p);
}


void Node::add_edge(Edge *e)
{
    edges.append(e);
}

void Node::remove_edge(Edge *e)
{
    edges.removeOne(e);
}

bool Node::has_edge_to(const Node *n) const
{
    foreach(Edge* e, edges)
    {
        if ( e->other(this) == n )
            return true;
    }
    return false;
}

double Node::distance_squared(QPointF to) const
{
    return point_distance_squared(pos,to);
}

void Node::paint(QPainter *painter, bool hidden, bool selected, bool active) const
{

    if ( selected )
    {
        painter->setPen(QPen(Qt::darkGray,2));
        painter->setBrush(Qt::white);
        painter->drawRect(bounding_box());
    }


    painter->setPen(Qt::black);

    if ( !hidden && active )
    {
        painter->setBrush(QColor("#ffcc00"));
        painter->drawEllipse(pos,xradius,xradius);
    }
    else if ( !hidden || active )
    {
        painter->setBrush(QColor("#ff4400"));
        painter->drawEllipse(pos,radius,radius);
    }

}



Node::Node(QPointF pos, Graph *parent)
 : Graph_Item(parent),pos(pos)
{
}

QRectF Node::bounding_box() const
{
    return QRectF(pos.x()-xradius,pos.y()-xradius,2*xradius,2*xradius);
}
