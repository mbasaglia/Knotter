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

#include "script_node_style.hpp"
#include <limits>
#include "resource_manager.hpp"

Script_Node_Style::Script_Node_Style(Node_Style wrapped, QObject *parent) :
    QObject(parent), wrapped(wrapped)
{
}

double Script_Node_Style::cusp_angle()
{
    return wrapped.enabled_style & Node_Style::CUSP_ANGLE ?
                wrapped.cusp_angle : std::numeric_limits<double>::quiet_NaN();
}

double Script_Node_Style::handle_length()
{
    return wrapped.enabled_style & Node_Style::HANDLE_LENGTH ?
                wrapped.handle_length : std::numeric_limits<double>::quiet_NaN();
}

double Script_Node_Style::cusp_distance()
{
    return wrapped.enabled_style & Node_Style::CUSP_DISTANCE ?
                wrapped.cusp_distance : std::numeric_limits<double>::quiet_NaN();
}

QString Script_Node_Style::cusp_shape()
{
    return wrapped.enabled_style & Node_Style::CUSP_SHAPE ?
                wrapped.cusp_shape->machine_name() : QString();
}

void Script_Node_Style::set_cusp_angle(double value)
{
    Node_Style before = wrapped;
    if ( value != value )
        wrapped.enabled_style &= ~Node_Style::CUSP_ANGLE;
    else
    {
        wrapped.enabled_style |= Node_Style::CUSP_ANGLE;
        wrapped.cusp_angle = value;
    }
    emit changed(before,wrapped);
}

void Script_Node_Style::set_handle_length(double value)
{
    Node_Style before = wrapped;
    if ( value != value )
        wrapped.enabled_style &= ~Node_Style::HANDLE_LENGTH;
    else
    {
        wrapped.enabled_style |= Node_Style::HANDLE_LENGTH;
        wrapped.handle_length = value;
    }
    emit changed(before,wrapped);
}


void Script_Node_Style::set_cusp_distance(double value)
{
    Node_Style before = wrapped;
    if ( value != value )
        wrapped.enabled_style &= ~Node_Style::CUSP_DISTANCE;
    else
    {
        wrapped.enabled_style |= Node_Style::CUSP_DISTANCE;
        wrapped.cusp_distance = value;
    }
    emit changed(before,wrapped);
}

void Script_Node_Style::set_cusp_shape(QString name)
{
    Node_Style before = wrapped;
    if ( name.isEmpty() )
        wrapped.enabled_style &= ~Node_Style::CUSP_SHAPE;
    else
    {
        wrapped.enabled_style |= Node_Style::CUSP_SHAPE;
        wrapped.cusp_shape = resource_manager().cusp_shape_from_machine_name(name);
    }
    emit changed(before,wrapped);
}

void Script_Node_Style::from_style(Node_Style style)
{
    Node_Style before = wrapped;
    wrapped = style;
    emit changed(before,wrapped);
}

void Script_Node_Style::clear()
{
    Node_Style before = wrapped;
    wrapped.enabled_style = Node_Style::NOTHING;
    emit changed(before,wrapped);
}
