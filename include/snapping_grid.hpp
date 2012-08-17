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
#ifndef SNAPPING_GRID_HPP
#define SNAPPING_GRID_HPP

#include <QPointF>
#include <QPainter>

class snapping_grid
{
    public:
        enum grid_shape { SQUARE, TRIANGLE1, TRIANGLE2 };

    protected:
        double size;
        grid_shape shape;
        QPointF origin;
        bool enabled;

    public:
        explicit snapping_grid ( double size = 64,
                                 grid_shape shape = SQUARE,
                                 QPointF origin=QPointF(0,0),
                                 bool enabled = true );

        /// move p to closest grid point
        void snap ( QPointF &p ) const;
        /// returns closest grid point
        QPointF nearest ( QPointF p ) const { snap(p); return p; }
        /// returns closest grid point
        QPointF nearest ( double x, double y ) const { return nearest(QPointF(x,y)); }

        /// draws grid lines that cover at least rect
        void render (QPainter *painter, const QRectF &rect) const;



        bool is_enabled () const;

        double get_size() const;

        grid_shape get_shape() const;

        QPointF get_origin() const;

        void enable ( bool enabled );

        void set_size ( double size );

        void set_shape ( grid_shape shape );

        void set_origin ( QPointF origin );
};

#endif // SNAPPING_GRID_HPP
