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
#include "snapping_grid.hpp"
#include <cmath>
#include <QPainter>
#include <QVarLengthArray>

snapping_grid::snapping_grid(double size, grid_shape type, QPointF origin, bool enabled)
    : size ( size ), shape ( type ), origin ( origin ), enabled ( enabled )
{}

void snapping_grid::snap(QPointF &p) const
{
    if ( !enabled )
        return;

    if ( shape == SQUARE )
    {
        /**
            Square grid, simple enough
        */
        p -= origin;
        p /= size;
        p.setX(qRound64(p.x()));
        p.setY(qRound64(p.y()));
        p *= size;
        p += origin;
    }
    else if ( shape == TRIANGLE1 )
    {
        /**
            Triangular grid, find intersection of line1 and line2
            line2 is horizontal: y = n2 * y_factor + origin.y
            y_factor is the height of the triangle sqrt(3)/2 * size.
            line1 has a 30deg slope, and x offset of n1*size + origin.x
            y = sqrt(3) * ( x + n1*size + origin.x )
            They intersect in the point where y is constrained by line2,
            x = size/2 * n2 + size * n1

            if you think of p as (x,y)+offset, you can get the expressions used
            below to get (x,y) from (p.x,p.y).
            The rounding p/size removes the offset as |offset.x| < size

            the p -= origin, p += origin is to transform the coordinates
            relative to the grid origin
        */
        p -= origin;
        double y_factor = size * sqrt(3.0)/2.0;
        qint64 n2 = qRound64(p.y()/y_factor);
        p.setY(n2*y_factor);
        qint64 n1 = qRound64 ( p.x()/size - n2/2.0 );
        p.setX(size*(n2/2.0+n1));
        p+=origin;
    }
    else if ( shape == TRIANGLE2 )
    {
        p -= origin;
        double x_factor = size * sqrt(3.0)/2.0;
        qint64 n2 = qRound64(p.x()/x_factor);
        p.setX(n2*x_factor);
        qint64 n1 = qRound64 ( p.y()/size - n2/2.0 );
        p.setY(size*(n2/2.0+n1));
        p+=origin;
    }
}

void snapping_grid::render(QPainter *painter, const QRectF &rect) const
{
    if ( !enabled )
        return;
    painter->setPen(QPen(Qt::lightGray,0));
    painter->drawEllipse(origin,5,5);
    QPointF topleft = nearest(rect.left()-size,rect.top()-size);

    QVarLengthArray<QLineF, 128> lines;
    if ( shape == snapping_grid::SQUARE )
    {
        for (double x = topleft.x(); x < rect.right(); x += size)
                lines.append(QLineF(x, rect.top(), x, rect.bottom()));
        for (double y = topleft.y(); y < rect.bottom(); y += size)
                lines.append(QLineF(rect.left(), y, rect.right(), y));
    }
    else if ( shape == snapping_grid::TRIANGLE1 )
    {
        double y_factor = size*sqrt(3.0)/2.0;
        double y;
        for ( y = topleft.y(); y < rect.bottom(); y += y_factor)
                lines.append(QLineF(rect.left(), y, rect.right(), y));

        double slope =  sqrt(3.0);
        double x = topleft.x();
        for ( double x2 = x; x2 < rect.right(); x += size )
        {
            x2 = (rect.bottom()-topleft.y())/(-slope)+x;
            lines.append(QLineF(x, topleft.y(),x2,rect.bottom() ));
        }

        for ( double x2 = x; x2 > rect.left(); x -= size )
        {
            x2 = (rect.bottom()-topleft.y())/(slope)+x;
            lines.append(QLineF(x, topleft.y(),x2,rect.bottom() ));
        }

    }
    else if ( shape == snapping_grid::TRIANGLE2 )
    {
        double x_factor = size*sqrt(3.0)/2.0;
        double x;
        for ( x = topleft.x(); x < rect.right(); x += x_factor)
                lines.append(QLineF(x,rect.top(), x, rect.bottom()));

        double slope =  sqrt(3.0);
        double y = topleft.y();
        for ( double y2 = y; y2 < rect.bottom(); y += size )
        {
            y2 = (rect.right()-topleft.x())/(-slope)+y;
            lines.append(QLineF(topleft.x(),y,rect.right(),y2 ));
        }

        for ( double y2 = y; y2 > rect.top(); y -= size )
        {
            y2 = (rect.right()-topleft.x())/(slope)+y;
            lines.append(QLineF(topleft.x(),y,rect.right(),y2 ));
        }

    }

    painter->drawLines(lines.data(), lines.size());
}

bool snapping_grid::is_enabled() const
{
    return enabled;
}

double snapping_grid::get_size() const
{
    return size;
}

snapping_grid::grid_shape snapping_grid::get_shape() const
{
    return shape;
}

QPointF snapping_grid::get_origin() const
{
    return origin;
}

void snapping_grid::enable(bool enabled)
{
    this->enabled = enabled;
}

void snapping_grid::set_size(double size)
{
    if ( size < 0 )
        size = -size;
    if ( size < 1 )
        return;

    this->size = size;
}

void snapping_grid::set_shape(snapping_grid::grid_shape shape)
{
    this->shape = shape;
}

void snapping_grid::set_origin(QPointF origin)
{
    this->origin = origin;
}
