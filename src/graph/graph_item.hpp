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

#ifndef GRAPH_ITEM_HPP
#define GRAPH_ITEM_HPP
#include <QObject>
#include <qmath.h>
#include <QPainter>
class Graph;

inline double point_distance_squared(const QPointF &p1, const QPointF &p2 )
{
    QPointF d = p1-p2;
    return d.x()*d.x() + d.y()*d.y();
}

inline double line_point_distance_squared(const QPointF& p1, const QPointF &p2,
                                          const QPointF &to)
{
    double a = qSqrt(point_distance_squared(p1,p2));
    double b2 = point_distance_squared(p1,to);
    double b = qSqrt(b2);
    double c = qSqrt(point_distance_squared(to,p2));
    double s = (a+b+c) / 2;         // Semi-perimeter
    double T = s*(s-a)*(s-b)*(s-c); // Area^2
    return T / b2; // ( Area / Base ) ^ 2

}

class Graph_Item : public QObject
{
    Q_OBJECT

public:
    Graph_Item(Graph* parent);

    virtual double distance_squared(QPointF to) const = 0;
    virtual void paint_regular(QPainter *painter) const = 0;
    virtual void paint_highlighted(QPainter *painter) const = 0;
};

#endif // GRAPH_ITEM_HPP
