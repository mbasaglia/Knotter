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

#ifndef POINT_MATH_HPP
#define POINT_MATH_HPP

#include <QPointF>
#include <QLineF>
#include <qmath.h>
#include "c++.hpp"

inline bool qFuzzyCompare ( QPointF p1, QPointF p2 )
{
    return qFuzzyCompare(p1.x(),p2.x()) && qFuzzyCompare(p1.y(),p2.y());
}

inline double point_distance_squared(const QPointF &p1, const QPointF &p2 )
{
    QPointF d = p1-p2;
    return d.x()*d.x() + d.y()*d.y();
}


inline double point_distance(const QPointF &p1, const QPointF &p2 )
{
    return qSqrt(point_distance_squared(p1,p2));
}

/*inline double line_point_distance_squared(const QPointF& p1, const QPointF &p2,
                                          const QPointF &to)
{
    double a = qSqrt(point_distance_squared(p1,p2));
    double b2 = point_distance_squared(p1,to);
    double b = qSqrt(b2);
    double c = qSqrt(point_distance_squared(to,p2));
    double s = (a+b+c) / 2;         // Semi-perimeter
    double T = s*(s-a)*(s-b)*(s-c); // Area^2
    return T / b2; // ( Area / Base ) ^ 2

}*/

/**
 * Project point to line
 */
inline QPointF project(QPointF point, QLineF line)
{
    QPointF intersection;
    QLineF normalLine = line.normalVector().translated(point - line.p1());
    line.intersect(normalLine, &intersection);
    return intersection;
}

constexpr inline long double pi() { return 3.1415926535897932384626433832L; }
constexpr inline long double deg2rad(long double deg ) { return deg/180*pi(); }
constexpr inline long double rad2deg(long double rad ) { return rad*180/pi(); }

#endif // POINT_MATH_HPP
