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
#include "path_builder.hpp"
#include <QMap>

path_builder::path_builder() {}

/*/// deep copy
path_builder::path_builder(const path_builder &o)
{
    foreach(path_item::line* stroke, o.strokes)
        strokes.push_back(stroke->clone());
}

path_builder &path_builder::operator= (const path_builder &o)
{
    this->~path_builder();

    foreach(path_item::line* stroke, o.strokes)
        strokes.push_back(stroke->clone());

    return *this;
}*/

path_builder::~path_builder()
{
    foreach ( const line_list::value_type& ll, strokes )
        foreach(path_item::line* stroke, ll)
            delete stroke;
    strokes.clear();
}

void path_builder::add_line(path_item::line *l)
{
    if ( strokes.empty() )
        new_group();

    for ( iterator i = strokes.back().begin(); i != strokes.back().end(); ++i )
    {
        if ( path_item::adjacent(*i,l) )
        {
            path_item::line* c = path_item::merge(*i,l);
            strokes.back().erase(i);
            for ( iterator j = strokes.back().begin(); j != strokes.back().end(); )
            {
                if ( *j != c && path_item::adjacent(c,*j) )
                {
                    c = path_item::merge(c,*j);
                    strokes.back().erase(j);
                    //j = strokes.back().begin();
                    break;
                }
                else
                    ++j;
            }
            strokes.back().push_back(c);
            return;
        }

    }

    strokes.back().push_back(l);
}

void path_builder::add_line(QPointF begin, QPointF end)
{
    add_line ( new path_item::line ( begin, end ) );
}

void path_builder::add_cubic(QPointF begin, QPointF control1, QPointF control2, QPointF end)
{
    add_line ( new path_item::cubiccurve(begin,control1,control2,end) );
}

void path_builder::add_quad(QPointF begin, QPointF control, QPointF end)
{
    add_line ( new path_item::quadcurve ( begin, control, end ) );
}

void path_builder::new_group()
{
    strokes.push_back(line_list::value_type());
}

QList<QPainterPath> path_builder::build()
{
    if ( strokes.empty() )
        return QList<QPainterPath>();

    QList<QPainterPath> paths;

    foreach ( const line_list::value_type& ll, strokes )
    {
        if ( ll.empty() )
            continue;

        paths.push_back(QPainterPath());

        //QPointF nextpoint = ll[0]->begin;

        //paths.back().moveTo(nextpoint);

        foreach ( path_item::line* stroke, ll )
        {
            stroke->add_to(true,paths.back());
            //nextpoint = stroke->end;
        }
    }

    return paths;
}
