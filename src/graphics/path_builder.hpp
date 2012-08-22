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
#ifndef PATH_BUILDER_HPP
#define PATH_BUILDER_HPP

#include <QPainterPath>

/**
    \file
    \brief attempt to create a signle stroke out of consecutive lines, right now it fails in some cases
*/

namespace path_item {

struct line
{
    QPointF begin;
    QPointF end;

    line ( QPointF begin, QPointF end );

    virtual line* clone() const;

    virtual void add_to ( bool move, QPainterPath& ppth ) const;

    virtual ~line();
};

struct cubiccurve : public line
{
    QPointF control1;
    QPointF control2;
    cubiccurve ( QPointF begin, QPointF control1, QPointF control2, QPointF end );

    void add_to ( bool move, QPainterPath& ppth ) const;
};

struct quadcurve : public line
{
    QPointF control;
    quadcurve ( QPointF begin, QPointF control, QPointF end );

    void add_to ( bool move, QPainterPath& ppth ) const;
};


} // namespace path_item

class path_builder
{
    protected:
        QList<path_item::line*> strokes;
        typedef QList<path_item::line*>::iterator iterator;

    public:
        path_builder();
        path_builder(const path_builder&);
        path_builder& operator= (const path_builder&);
        ~path_builder();

        void add_line ( path_item::line* l );
        void add_line( QPointF begin, QPointF end );
        void add_cubic( QPointF begin, QPointF control1, QPointF control2, QPointF end );
        void add_quad ( QPointF begin, QPointF control, QPointF end );

        /// arranges the stokes in a way that endpoints can be merged
        void sort();

        QPainterPath build();
};

#endif // PATH_BUILDER_HPP
