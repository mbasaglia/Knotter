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

#include "path_item.hpp"
#include "point_math.hpp"

namespace path_item {

Line::Line ( QPointF begin, QPointF end )
    : begin ( begin ), end ( end ) {}

void Line::add_to(bool move, QPainterPath &ppth) const
{
    if ( move )
        ppth.moveTo(begin);

    ppth.lineTo(end);
}

void Line::reverse()
{
    qSwap(begin,end);
}

Line::~Line()
{}

Cubic_Curve::Cubic_Curve(QPointF begin, QPointF control1, QPointF control2, QPointF end)
    : Line ( begin, end ), control1 ( control1 ), control2 ( control2 )
{}

void Cubic_Curve::reverse()
{
    qSwap(control1,control2);
    qSwap(begin,end);
}

void Cubic_Curve::add_to(bool move, QPainterPath &ppth) const
{
    if ( move )
        ppth.moveTo(begin);

    ppth.cubicTo(control1,control2,end);
}

Quad_Curve::Quad_Curve(QPointF begin, QPointF control, QPointF end)
    : Line ( begin, end ), control ( control )
{}

void Quad_Curve::add_to(bool move, QPainterPath &ppth) const
{
    if ( move )
        ppth.moveTo(begin);

    ppth.quadTo(control,end);
}

Compound::Compound(Line *l)
    : Line(*l)
{
    append ( l );
}

void Compound::append(Line * l)
{
    end = l->end;

    Compound* c = dynamic_cast<Compound*>(l);

    if ( c )
    {
        elements += c->elements;
        c->elements.clear();
        delete c;
    }
    else
        elements.push_back(l);
}

void Compound::add_to(bool move, QPainterPath &ppth) const
{
    if ( move )
        ppth.moveTo(begin);

    foreach(Line* l,elements)
    {
        l->add_to(false,ppth);
    }
}

void Compound::reverse()
{
    Line::reverse();
    // reverse container and items
    QList<Line*> nit;
    foreach(Line* l,elements)
    {
        nit.push_front(l);
        l->reverse();
    }
    elements = nit;
}

Compound::~Compound()
{
    foreach(Line* l,elements)
        delete l;
}

Compound* merge ( Line* a, Line* b )
{
    if ( qFuzzyCompare(a->begin,b->begin) ||
            qFuzzyCompare(a->end,b->end) )
        b->reverse();

    if ( qFuzzyCompare(a->begin,b->end) )
        qSwap(a,b);

    Compound* c = dynamic_cast<Compound*>(a);

    if ( !c )
    {
        c = new Compound(a);
    }

    c->append(b);

    return c;
}

bool adjacent (const Line *a, const Line *b )
{
    if ( qFuzzyCompare(a->begin,b->end) ||
         qFuzzyCompare(a->end,b->begin) )
    {
        return true;
    }
    else if ( qFuzzyCompare(a->begin,b->begin) ||
              qFuzzyCompare(a->end,b->end) )
    {
        return true;
    }

    return false;
}



} // namespace path_item
