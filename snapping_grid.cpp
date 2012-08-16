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

snapping_grid::snapping_grid(double size, grid_shape type, QPointF origin, bool enabled)
    : size ( size ), shape ( type ), origin ( origin ), enabled ( enabled )
{}

void snapping_grid::snap(QPointF &p)
{
    /// \todo find a way to draw the grid (lines or dots?)
    /// \todo find a cool way to configure the origin ( cool way != typing coordinates )

    if ( !enabled )
        return;

    if ( shape == SQUARE )
    {
        /**
            Square grid, simple enough
        */
        p /= size;
        p.setX(qRound64(p.x()));
        p.setY(qRound64(p.y()));
        p *= size;
        p += origin;
    }
    else if ( shape == TRIANGLE )
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
        */
        double y_factor = size * sqrt(3.0)/2.0;
        qint64 n2 = qRound64(p.y()/y_factor);
        p.setY(n2*y_factor+origin.y());
        qint64 n1 = qRound64 ( p.x()/size - n2/2.0 );
        p.setX(size*(n2/2.0+n1)+origin.y());
    }
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
