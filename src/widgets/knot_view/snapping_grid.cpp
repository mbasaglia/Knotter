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

#include "snapping_grid.hpp"
#include <QVarLengthArray>
#include <QLineF>

static const double sqrt3 = 1.732050808;

Snapping_Grid::Snapping_Grid(unsigned size, Snapping_Grid::Grid_Shape shape,
                             QPointF origin, bool enabled)
    : m_size(size > 0 ? size : 1), m_shape(shape),
      m_origin(origin), m_enabled(enabled)
{
}

void Snapping_Grid::snap(QPointF &p) const
{
    if ( !m_enabled )
        return;

    if ( m_shape == SQUARE )
    {
        /**
            Square grid, simple enough
        */
        p -= m_origin;
        p /= m_size;
        p.setX(qRound64(p.x()));
        p.setY(qRound64(p.y()));
        p *= m_size;
        p += m_origin;
    }
    else if ( m_shape == TRIANGLE1 )
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
        p -= m_origin;
        double y_factor = m_size * sqrt3/2.0;
        qint64 n2 = qRound64(p.y()/y_factor);
        p.setY(n2*y_factor);
        qint64 n1 = qRound64 ( p.x()/m_size - n2/2.0 );
        p.setX(m_size*(n2/2.0+n1));
        p+=m_origin;
    }
    else if ( m_shape == TRIANGLE2 )
    {
        p -= m_origin;
        double x_factor = m_size * sqrt3/2.0;
        qint64 n2 = qRound64(p.x()/x_factor);
        p.setX(n2*x_factor);
        qint64 n1 = qRound64 ( p.y()/m_size - n2/2.0 );
        p.setY(m_size*(n2/2.0+n1));
        p+=m_origin;
    }
}

void Snapping_Grid::render(QPainter *painter, const QRectF &rect) const
{

    if ( !m_enabled )
        return;
    painter->setPen(QPen(Qt::lightGray,0));
    painter->drawEllipse(m_origin,5,5);
    QPointF topleft = nearest(rect.left()-m_size,rect.top()-m_size);

    QVarLengthArray<QLineF, 128> lines;
    if ( m_shape == Snapping_Grid::SQUARE )
    {
        for (double x = topleft.x(); x < rect.right(); x += m_size)
                lines.append(QLineF(x, rect.top(), x, rect.bottom()));
        for (double y = topleft.y(); y < rect.bottom(); y += m_size)
                lines.append(QLineF(rect.left(), y, rect.right(), y));
    }
    else if ( m_shape == Snapping_Grid::TRIANGLE1 )
    {
        double y_factor = m_size*sqrt3/2.0;
        double y;
        for ( y = topleft.y(); y < rect.bottom(); y += y_factor)
                lines.append(QLineF(rect.left(), y, rect.right(), y));

        double slope =  sqrt3;
        double x = topleft.x();
        for ( double x2 = x; x2 < rect.right(); x += m_size )
        {
            x2 = (rect.bottom()-topleft.y())/(-slope)+x;
            lines.append(QLineF(x, topleft.y(),x2,rect.bottom() ));
        }

        for ( double x2 = x; x2 > rect.left(); x -= m_size )
        {
            x2 = (rect.bottom()-topleft.y())/(slope)+x;
            lines.append(QLineF(x, topleft.y(),x2,rect.bottom() ));
        }

    }
    else if ( m_shape == Snapping_Grid::TRIANGLE2 )
    {
        double x_factor = m_size*sqrt3/2.0;
        double x;
        for ( x = topleft.x(); x < rect.right(); x += x_factor)
                lines.append(QLineF(x,rect.top(), x, rect.bottom()));

        double slope =  sqrt3;
        double y = topleft.y();
        for ( double y2 = y; y2 < rect.bottom(); y += m_size )
        {
            y2 = (rect.right()-topleft.x())/(-slope)+y;
            lines.append(QLineF(topleft.x(),y,rect.right(),y2 ));
        }

        for ( double y2 = y; y2 > rect.top(); y -= m_size )
        {
            y2 = (rect.right()-topleft.x())/(slope)+y;
            lines.append(QLineF(topleft.x(),y,rect.right(),y2 ));
        }

    }

    painter->drawLines(lines.data(), lines.size());
}

void Snapping_Grid::enable(bool enabled)
{
    m_enabled = enabled;
    emit grid_changed();
}

void Snapping_Grid::set_size(unsigned size)
{
    if ( size > 0 )
    {
        m_size = size;
        emit grid_changed();
    }
}

void Snapping_Grid::set_shape(Snapping_Grid::Grid_Shape shape)
{
    m_shape = shape;
    emit grid_changed();
}

void Snapping_Grid::set_origin(QPointF origin)
{
    m_origin = origin;
    emit grid_changed();
}
