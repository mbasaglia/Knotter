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

#include "path_item.hpp"

namespace path_item {

line::line ( QPointF begin, QPointF end )
    : begin ( begin ), end ( end ) {}


/*line* line::clone() const
{
    return new line ( begin, end );
}*/

void line::add_to(bool move, QPainterPath &ppth) const
{
    if ( move )
        ppth.moveTo(begin);

    ppth.lineTo(end);
}

void line::reverse()
{
    qSwap(begin,end);
}

line::~line()
{

}

cubiccurve::cubiccurve(QPointF begin, QPointF control1, QPointF control2, QPointF end)
    : line ( begin, end ), control1 ( control1 ), control2 ( control2 )
{}

void cubiccurve::reverse()
{
    qSwap(control1,control2);
    qSwap(begin,end);
}

void cubiccurve::add_to(bool move, QPainterPath &ppth) const
{
    if ( move )
        ppth.moveTo(begin);

    ppth.cubicTo(control1,control2,end);
}

quadcurve::quadcurve(QPointF begin, QPointF control, QPointF end)
    : line ( begin, end ), control ( control )
{}

void quadcurve::add_to(bool move, QPainterPath &ppth) const
{
    if ( move )
        ppth.moveTo(begin);

    ppth.quadTo(control,end);
}

compound::compound(line *l)
    : line(*l)
{
    append ( l );
}

void compound::append(line * l)
{
    end = l->end;

    compound* c = dynamic_cast<compound*>(l);

    if ( c )
    {
        elements += c->elements;
        c->elements.clear();
        delete c;
    }
    else
        elements.push_back(l);
}

void compound::add_to(bool move, QPainterPath &ppth) const
{
    if ( move )
        ppth.moveTo(begin);

    foreach(line* l,elements)
    {
        l->add_to(false,ppth);
    }
}

void compound::reverse()
{
    QList<line*> nit;
    line::reverse();
    foreach(line* l,elements)
    {
        nit.push_front(l);
        l->reverse();
    }
    elements = nit;
}

compound::~compound()
{
    foreach(line* l,elements)
        delete l;
}

compound* merge ( line* a, line* b )
{
    adjacent(a,b);

    if ( a->begin == b->end )
        qSwap(a,b);

    compound* c = dynamic_cast<compound*>(a);

    if ( !c )
    {
        c = new compound(a);
    }

    c->append(b);

    return c;
}

inline bool fuzzy_double_equals ( double p1, double p2 )
{
    return qFuzzyCompare(p1,p2);
    //return qAbs(p1-p2) <= 0.1*qMax(p1,p2);
}

inline bool fuzzy_point_equals ( QPointF p1, QPointF p2 )
{
    return fuzzy_double_equals(p1.x(),p2.x()) && fuzzy_double_equals(p1.y(),p2.y());
}


bool adjacent ( line* a, line* b )
{
    if ( fuzzy_point_equals(a->begin,b->end) ||
         fuzzy_point_equals(a->end,b->begin) )
    {
        return true;
    }
    else if ( fuzzy_point_equals(a->begin,b->begin) ||
              fuzzy_point_equals(a->end,b->end) )
    {
        b->reverse();
        return true;
    }

    return false;
}



} // namespace path_item
