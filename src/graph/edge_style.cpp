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

#include "edge_style.hpp"
#include "edge.hpp"

Edge_Style::Edge_Style()
{
}

void Edge_Style::paint_regular(QPainter *painter, const Edge &edge)
{
    QPen pen(QColor("#0088ff"),2);
    pen.setCosmetic(true);
    painter->setPen(pen);

    paint(painter,edge);
}

void Edge_Style::paint_highlighted(QPainter *painter, const Edge &edge)
{
    QPen pen(QColor("#00ccff"),4);
    pen.setCosmetic(true);
    painter->setPen(pen);

    paint(painter,edge);
}

void Edge_Style::paint(QPainter *painter, const Edge &edge)
{
    painter->drawLine(edge.to_line());
}


void Edge_Inverted::paint(QPainter *painter, const Edge &edge)
{
    QPen pen = painter->pen();
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);

    painter->drawLine(edge.to_line());
}


void Edge_Wall::paint(QPainter *painter, const Edge &edge)
{
    QPen pen = painter->pen();
    pen.setWidth(pen.width()*3);
    painter->setPen(pen);

    painter->drawLine(edge.to_line());


    QPen p(Qt::white,pen.width()/3);
    p.setCosmetic(true);
    painter->setPen(p);

    painter->drawLine(edge.to_line());
}


void Edge_Hole::paint(QPainter *painter, const Edge &edge)
{
    QLineF l = edge.to_line().normalVector();
    l.setLength(5);
    l.translate((edge.vertex1()->pos()-edge.vertex2()->pos())/2);
    painter->drawLine(l);
    l.setLength(-5);
    painter->drawLine(l);
}
