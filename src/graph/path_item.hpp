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

#include <QPointF>
#include <QPainterPath>

/**
 * Classes that can be stored inside a Path_Builder and related functions
 */
namespace path_item {

/**
 *  Straight line
 */
struct Line
{

    QPointF begin;
    QPointF end;

    Line ( QPointF begin, QPointF end );

    /**
        \brief Add the line to the painterpath

        if move==false, it will assume that the painter path cursor is already
        at begin

        \param      move Whether to start with moveTo(begin)
        \param[out] ppth Output painter path
    */
    virtual void add_to ( bool move, QPainterPath& ppth ) const;

    /**
        \brief reverse direction of the line
    */
    virtual void reverse();

    virtual ~Line();
};

/**
 *  Quadratic curve (1 control point)
 */
struct Quad_Curve : public Line
{
    QPointF control;
    Quad_Curve ( QPointF begin, QPointF control, QPointF end );

    void add_to ( bool move, QPainterPath& ppth ) const;
};

/**
 *  Cubic curve (1 control points)
 */
struct Cubic_Curve : public Line
{
    QPointF control1;
    QPointF control2;
    Cubic_Curve ( QPointF begin, QPointF control1, QPointF control2, QPointF end );

    void reverse();

    void add_to ( bool move, QPainterPath& ppth ) const;
};


/**
    \brief sequence of adjacent lines
    \note takes ownership of passed line pointers
*/
struct Compound : public Line
{
    private:
        QList<Line*> elements;

    public:
        explicit Compound ( Line* l );

        /**
         *  \brief Append item and take ownership.
         *
         *  \note if l is a Compound object it will be cleared and deleted
         */
        void append(Line* l);

        void add_to ( bool move, QPainterPath& ppth ) const;

        void reverse();

        ~Compound();
};

/**
 *  \brief Merge two lines
 *
 *  If any of them is already a coumpoind object, it will take ownership of the other one
 */
Compound* merge ( Line* a, Line* b );

/**
    \brief check consecutive lines
    \return true <=> a and be can be merged in a continuous line
*/
bool adjacent ( const Line* a, const Line* b );



} // namespace path_item

