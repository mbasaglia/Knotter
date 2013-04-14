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
#include "edge.hpp"
#include "graph.hpp"
#include <QVector2D>

Edge::Edge(Node *v1, Node *v2, Edge_Style* e_style, Graph *parent) :
    Graph_Item(parent), v1(v1), v2(v2), m_style(e_style)
{
    attach();
}

void Edge::detach()
{
    v1->remove_edge(this);
    v2->remove_edge(this);
}

void Edge::attach()
{
    v1->add_edge(this);
    v2->add_edge(this);
}


void Edge::set_style(Edge_Style *st)
{
    m_style = st;
}

Edge_Style *Edge::style() const
{
    return m_style;
}

void Edge::paint_regular(QPainter *painter) const
{
    m_style->paint_regular(painter,*this);
}

void Edge::paint_highlighted(QPainter *painter) const
{
    m_style->paint_highlighted(painter,*this);
}

double Edge::distance_squared(QPointF to) const
{
     return line_point_distance_squared(v1->position(),v2->position(),to);
}
