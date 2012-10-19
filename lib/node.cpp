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
#include "node.hpp"
#include <QPainter>
#include "edge.hpp"
#include <algorithm>

bool CustomItem::show_graph = true;

Node::Node(QPointF position)
    : CustomItem(true)
{
    setPos(position);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    setZValue(1);
}

Node::~Node()
{}


QRectF Node::boundingRect() const
{
    return QRectF ( -(radius+1), -(radius+1), 2*(radius+1), 2*(radius+1) );
}

QRectF Node::actualRect() const
{
    return QRectF ( -radius, -radius, 2*radius, 2*radius );
}

void Node::paint(QPainter *painter,
                const QStyleOptionGraphicsItem *,
                QWidget *)
{

    if ( isSelected() )
    {
        painter->setPen(QPen(Qt::darkGray,2));
        painter->setBrush(Qt::white);
        painter->drawRect(boundingRect());
    }

    if ( !is_visible() && !highlight)
        return;

    painter->setPen(Qt::black);

    if ( highlight && is_visible() )
    {
        painter->setBrush(QColor("#ffcc00"));
        painter->drawEllipse(boundingRect());
    }
    else
    {
        painter->setBrush(QColor("#ff4400"));
        painter->drawEllipse(actualRect());
    }
}

Edge *Node::get_link(Node *other) const
{
    foreach ( Edge* e, edges )
    {
        if ( e->is_vertex(other) )
            return e;
    }
    return NULL;
}

void Node::add_link(Edge *edge)
{
    edges.push_back(edge);
}

void Node::remove_link(Edge *which)
{
    edges.erase ( std::find ( edges.begin(), edges.end(), which ) );
}

void Node::clear_links()
{
    edges.clear();
}

QList<Node *> Node::linked_to() const
{
    QList<Node *> r;
    foreach ( Edge* e, edges )
        r.push_back(e->other(this));
    return r;
}


void Node::set_custom_style(styleinfo si)
{
    custom_style = si;
}

void Node::disable_custom_style()
{
    custom_style.enabled_style = styleinfo::NOTHING;
}



