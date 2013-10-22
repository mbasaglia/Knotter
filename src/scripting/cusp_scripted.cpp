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

#include "cusp_scripted.hpp"
#include "script_line.hpp"
#include "script_path_builder.hpp"
#include "script_point.hpp"
#include "traversal_info.hpp"
#include "resource_manager.hpp"

QString Cusp_Scripted::machine_name() const
{
    return plugin->string_data("plugin_shortname");
}

void Cusp_Scripted::draw_joint(Path_Builder &path, const Traversal_Info &ti, const Node_Style &style) const
{
    Script_Line input_edge ( ti.node->pos(), ti.in.edge->other(ti.node)->pos() );
    Script_Line output_edge ( ti.node->pos(), ti.out.edge->other(ti.node)->pos() );
    Script_Line start_handle = ti.in.edge->style().edge_type->handle(ti.in.edge,ti.in.handle);
    Script_Line finish_handle = ti.out.edge->style().edge_type->handle(ti.out.edge,ti.out.handle);
    Script_Point cusp_point = this->cusp_point(ti,style.cusp_distance);
    Script_Point node_point = ti.node->pos();


    Resource_Manager::script_param_template("input_edge",input_edge);
    Resource_Manager::script_param_template("output_edge",output_edge);
    Resource_Manager::script_param_template("start_handle",start_handle);
    Resource_Manager::script_param_template("finish_handle",finish_handle);
    Resource_Manager::script_param_template("node_point",node_point);
    Resource_Manager::script_param_template("cusp_point",cusp_point);
    Resource_Manager::script_param_template("angle",ti.angle_delta);
    Resource_Manager::script_param_template("handle_length",style.handle_length);
    Resource_Manager::script_param_template("cusp_angle",style.cusp_angle);
    Resource_Manager::script_param_template("cusp_distance",style.cusp_distance);
    Resource_Manager::script_param_template("direction", ti.handside == Traversal_Info::LEFT ? -1 : +1 );


    Script_Path_Builder script_path(&path);
    Resource_Manager::script_param("path",&script_path);

    Resource_Manager::run_script(plugin);

}

