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

#include "script_edge_style.hpp"
#include <limits>
#include "resource_manager.hpp"


Script_Edge_Style::Script_Edge_Style(Edge_Style *wrapped, QObject *parent)
    : QObject(parent), wrapped(wrapped)
{
}

double Script_Edge_Style::handle_length()
{
    return wrapped->enabled_style & Edge_Style::HANDLE_LENGTH ?
                wrapped->handle_length : std::numeric_limits<double>::quiet_NaN();
}

double Script_Edge_Style::crossing_distance()
{
    return wrapped->enabled_style & Edge_Style::CROSSING_DISTANCE ?
                wrapped->crossing_distance : std::numeric_limits<double>::quiet_NaN();
}

double Script_Edge_Style::slide()
{
    return wrapped->enabled_style & Edge_Style::EDGE_SLIDE ?
                wrapped->edge_slide : std::numeric_limits<double>::quiet_NaN();
}

QString Script_Edge_Style::edge_type()
{
    return wrapped->edge_type ? wrapped->edge_type->machine_name() :
                                Resource_Manager::default_edge_type()->machine_name();
}

void Script_Edge_Style::set_crossing_distance(double value)
{
    Edge_Style before = *wrapped;
    if ( value != value )
        wrapped->enabled_style &= ~Edge_Style::CROSSING_DISTANCE;
    else
    {
        wrapped->enabled_style |= Edge_Style::CROSSING_DISTANCE;
        wrapped->crossing_distance = value;
    }
    emit changed(before,*wrapped);
}

void Script_Edge_Style::set_handle_length(double value)
{
    Edge_Style before = *wrapped;
    if ( value != value )
        wrapped->enabled_style &= ~Edge_Style::HANDLE_LENGTH;
    else
    {
        wrapped->enabled_style |= Edge_Style::HANDLE_LENGTH;
        wrapped->handle_length = value;
    }
    emit changed(before,*wrapped);
}

void Script_Edge_Style::set_slide(double value)
{
    Edge_Style before = *wrapped;
    if ( value != value )
        wrapped->enabled_style &= ~Edge_Style::EDGE_SLIDE;
    else
    {
        wrapped->enabled_style |= Edge_Style::EDGE_SLIDE;
        wrapped->edge_slide = value;
    }
    emit changed(before,*wrapped);
}

void Script_Edge_Style::set_edge_type(QString name)
{

    Edge_Style before = *wrapped;

    wrapped->edge_type = Resource_Manager::edge_type_from_machine_name(name);

    emit changed(before,*wrapped);
}

void Script_Edge_Style::clear()
{
    Edge_Style before = *wrapped;
    wrapped->enabled_style = Edge_Style::EDGE_TYPE;
    emit changed(before,*wrapped);
}
