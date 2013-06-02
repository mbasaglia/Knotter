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

#include "plugin_cusp.hpp"
#include "resource_manager.hpp"
#include "cusp_scripted.hpp"

Plugin_Cusp::Plugin_Cusp(const QVariantMap &metadata)
    : Plugin(metadata,Plugin::Cusp)
{
    if ( icon().isNull() )
        set_data("icon","cusp-other");
    cusp_shape = new Cusp_Scripted(this);

    if ( is_enabled() )
        enable(true);
}

Plugin_Cusp::~Plugin_Cusp()
{
    if ( !Resource_Manager::cusp_shapes().contains(cusp_shape) )
        delete cusp_shape;
}

QString Plugin_Cusp::machine_name() const
{
    return "plugin_"+string_data("name");
}


void Plugin_Cusp::on_enable(bool b)
{
    if ( b )
    {
        if ( !Resource_Manager::cusp_shapes().contains(cusp_shape) )
            Resource_Manager::register_cusp_shape(cusp_shape);
    }
    else
        Resource_Manager::remove_cusp_shape(cusp_shape);
}
