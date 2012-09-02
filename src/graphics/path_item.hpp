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
#ifndef PATH_ITEM_HPP
#define PATH_ITEM_HPP

///    \todo move this in lib

#include <QPainterPath>

namespace path_item {



struct line
{
    QPointF begin;
    QPointF end;

    line ( QPointF begin, QPointF end );

    //virtual line* clone() const;

    /**
        \brief add the line to the painterpath
        \param move whether to start with moveTo(begin)
    */
    virtual void add_to ( bool move, QPainterPath& ppth ) const;

    /**
        \brief reverse direction of the line
    */
    virtual void reverse();

    virtual ~line();
};

struct cubiccurve : public line
{
    QPointF control1;
    QPointF control2;
    cubiccurve ( QPointF begin, QPointF control1, QPointF control2, QPointF end );

    void reverse();

    void add_to ( bool move, QPainterPath& ppth ) const;
};

struct quadcurve : public line
{
    QPointF control;
    quadcurve ( QPointF begin, QPointF control, QPointF end );

    void add_to ( bool move, QPainterPath& ppth ) const;
};

/**
    \brief sequence of adjacent lines
    \note takes ownership of passed line pointers
*/
struct compound : public line
{
    private:
        QList<line*> elements;

    public:
        explicit compound ( line* l );

        void append(line*);

        void add_to ( bool move, QPainterPath& ppth ) const;

        void reverse();

        ~compound();
};

compound* merge ( line* a, line* b );

/**
    \brief check consecutive lines
    \return true <=> a and be can be merged in a continuous line
    \note b may be reversed by this
*/
bool adjacent ( line* a, line* b );


} // namespace path_item

#endif // PATH_ITEM_HPP
