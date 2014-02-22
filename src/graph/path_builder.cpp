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

#include "path_builder.hpp"

Path_Builder::Path_Builder()
{
}

Path_Builder::~Path_Builder()
{
    foreach ( const container::value_type& ll, strokes )
        foreach(group::value_type stroke, ll)
            delete stroke;
    strokes.clear();
}

void Path_Builder::add_line(path_item::Line *current)
{
    if ( strokes.empty() )
        new_group();

    group& last_group = strokes.back();

    // check adjacent strokes to merge them
    for ( iterator i = last_group.begin(); i != last_group.end(); ++i )
    {
        if ( path_item::adjacent(*i,current) )
        {
            path_item::Line* merged = path_item::merge(*i,current);
            last_group.erase(i);
            // check adjacent on the other end
            for ( iterator j = last_group.begin(); j != last_group.end(); )
            {
                if ( *j != merged && path_item::adjacent(merged,*j) )
                {
                    merged = path_item::merge(merged,*j);
                    strokes.back().erase(j);
                    //j = strokes.back().begin();
                    break;
                }
                else
                    ++j;
            }
            strokes.back().push_back(merged);
            return;
        }

    }

    strokes.back().push_back(current);
}



void Path_Builder::add_line(QPointF begin, QPointF end)
{
    add_line ( new path_item::Line ( begin, end ) );
}

void Path_Builder::add_cubic(QPointF begin, QPointF control1, QPointF control2, QPointF end)
{
    add_line ( new path_item::Cubic_Curve(begin,control1,control2,end) );
}

void Path_Builder::add_quad(QPointF begin, QPointF control, QPointF end)
{
    add_line ( new path_item::Quad_Curve ( begin, control, end ) );
}

void Path_Builder::new_group()
{
    strokes.push_back(group());
}

QList<QPainterPath> Path_Builder::build()
{
    if ( strokes.empty() )
        return QList<QPainterPath>();

    QList<QPainterPath> paths;

    foreach ( const container::value_type& ll, strokes )
    {
        if ( ll.empty() )
            continue;

        paths.push_back(QPainterPath());

        QPointF nextpoint = ll[0]->begin;

        paths.back().moveTo(nextpoint);

        foreach ( path_item::Line* stroke, ll )
        {
            stroke->add_to(true,paths.back());
            nextpoint = stroke->end;
        }
    }

    return paths;
}
