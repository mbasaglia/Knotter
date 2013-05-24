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

void Cusp_Scripted::draw_joint(Path_Builder &path, const Traversal_Info &ti, const Node_Style &style) const
{
    Script_Line input_edge ( ti.node->pos(), ti.in.edge->other(ti.node)->pos() );
    Script_Line output_edge ( ti.node->pos(), ti.out.edge->other(ti.node)->pos() );
    Script_Line start_handle = ti.in.edge->style()->handle(ti.in.edge,ti.in.handle,style);
    Script_Line finish_handle = ti.out.edge->style()->handle(ti.out.edge,ti.out.handle,style);
    Script_Point cusp_point = this->cusp_point(ti,style.cusp_distance);
    Script_Point node_point = ti.node->pos();

    QScriptEngine& engine = Resource_Manager::script_engine();


    engine.globalObject().setProperty("input_edge",engine.toScriptValue(input_edge));
    engine.globalObject().setProperty("output_edge",engine.toScriptValue(output_edge));
    engine.globalObject().setProperty("start_handle",engine.toScriptValue(start_handle));
    engine.globalObject().setProperty("finish_handle",engine.toScriptValue(finish_handle));
    engine.globalObject().setProperty("node_point",engine.toScriptValue(node_point));
    engine.globalObject().setProperty("cusp_point",engine.toScriptValue(cusp_point));
    engine.globalObject().setProperty("angle",ti.angle_delta);
    engine.globalObject().setProperty("handle_length",style.handle_length);
    engine.globalObject().setProperty("cusp_angle",style.cusp_angle);
    engine.globalObject().setProperty("cusp_distance",style.cusp_distance);
    engine.globalObject().setProperty("direction",
        engine.toScriptValue(ti.handside == Traversal_Info::LEFT ? -1 : +1 ));


    Script_Path_Builder script_path(&path);
    engine.globalObject().setProperty("path",engine.newQObject(&script_path));

    engine.evaluate(plugin->script_program());
    if ( engine.hasUncaughtException() )
    {
        qWarning() << plugin->string_data("script") << ":"
                   << engine.uncaughtExceptionLineNumber()
                   << QObject::tr("Error:")
                   << engine.uncaughtException().toString();

        qWarning() << engine.uncaughtExceptionBacktrace();

        engine.clearExceptions();
    }
}

