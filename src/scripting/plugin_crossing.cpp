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

#include "plugin_crossing.hpp"
#include "edge_scripted.hpp"
#include "resource_manager.hpp"

Plugin_Crossing::Plugin_Crossing(const QVariantMap &metadata)
    : Plugin(metadata,Plugin::Crossing)
{
    if ( icon().isNull() )
        set_data("icon","edge-other");
    edge_type = new Edge_Scripted(this);

    if ( is_enabled() )
        enable(true);
}

Plugin_Crossing::~Plugin_Crossing()
{
    if ( !resource_manager().edge_types().contains(edge_type) )
        delete edge_type;
}


void Plugin_Crossing::on_enable(bool b)
{
    if ( b )
    {
        if ( !resource_manager().edge_types().contains(edge_type) )
            resource_manager().register_edge_type(edge_type);
    }
    else
        resource_manager().remove_edge_type(edge_type);
}

