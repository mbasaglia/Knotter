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

#include "edge_scripted.hpp"
#include "resource_manager.hpp"
#include "script_path_builder.hpp"
#include "script_graph.hpp"

Edge_Scripted::Edge_Scripted(Plugin_Crossing *plugin)
    : plugin(plugin)
{
}


void Edge_Scripted::setup_script() const
{
    // set up enum
    Resource_Manager::script_param_template("BOTTOM_LEFT",Edge::BOTTOM_LEFT);
    Resource_Manager::script_param_template("BOTTOM_RIGHT",Edge::BOTTOM_RIGHT);
    Resource_Manager::script_param_template("TOP_LEFT",Edge::TOP_LEFT);
    Resource_Manager::script_param_template("TOP_RIGHT",Edge::TOP_RIGHT);

    // run common script, preserving changes to the local context
    QScriptValue old_local;
    Resource_Manager::run_script(plugin,&old_local);

    // restore context from previous execution
    QScriptContext *new_context = Resource_Manager::script_context();
    new_context->setActivationObject(old_local);
}

Edge::Handle Edge_Scripted::traverse(Edge *edge, Edge::Handle handle, Path_Builder &path) const
{
    // run common script
    setup_script();

    // set up params
    Resource_Manager::script_param_template("handle",handle);
    Resource_Manager::script_param_template("result",handle);

    Script_Path_Builder script_path(&path);
    Resource_Manager::script_param("path",&script_path);

    Script_Graph script_graph(*edge->graph());
    Script_Edge script_edge(edge,&script_graph);
    Resource_Manager::script_param("edge",&script_edge);

    Script_Edge_Style script_style(edge->defaulted_style());
    Resource_Manager::script_param("style",&script_style);

    // Run traverse script
    QScriptValue local;
    Resource_Manager::run_script(plugin->string_data("traverse"),
        QString("%1:traverse").arg(plugin->string_data("plugin_file")),
        1,&local);

    // Extract value
    edge_handle_from_script(local.property("result"),handle);
    return handle;
}

QLineF Edge_Scripted::handle(const Edge *edge, Edge::Handle handle) const
{
    // run common script
    setup_script();

    // set up params
    Resource_Manager::script_param_template("handle",handle);

    Script_Graph script_graph(*edge->graph());
    /// \warning const_cast
    Script_Edge script_edge(const_cast<Edge*>(edge),&script_graph);
    Resource_Manager::script_param("edge",&script_edge);

    Script_Edge_Style script_style(edge->defaulted_style());
    Resource_Manager::script_param("style",&script_style);

    Script_Line result;
    Resource_Manager::script_param("result",&result);

    // Run handle script
    QScriptValue local;
    Resource_Manager::run_script(plugin->string_data("handle"),
        QString("%1:handle").arg(plugin->string_data("plugin_file")),
        1,&local);

    // Extract value
    line_from_script(local.property("result"),result);
    return result;
}

void Edge_Scripted::paint(QPainter *painter, const Edge &edge)
{

    painter->drawLine(edge.to_line());

    const double radius = 5;
    QPointF p = edge.to_line().pointAt(
                edge.style().enabled_style & Edge_Style::EDGE_SLIDE ?
                edge.style().edge_slide : 0.5
    );

    painter->drawEllipse(p,radius,radius);
}


QIcon Edge_Scripted::icon() const
{
    return plugin->icon();
}



QString Edge_Scripted::machine_name() const
{
    return "plugin_"+name();
}


QString Edge_Scripted::name() const
{
    return plugin->string_data("name");
}



QScriptValue edge_handle_toString (QScriptContext *context, QScriptEngine *engine)
{
    QString name;

    qint32 n = context->thisObject().toInt32();
    switch ( n )
    {
        case Edge::BOTTOM_LEFT: name = "BOTTOM_LEFT"; break;
        case Edge::BOTTOM_RIGHT: name = "BOTTOM_RIGHT"; break;
        case Edge::TOP_LEFT: name = "TOP_LEFT"; break;
        case Edge::TOP_RIGHT: name = "TOP_RIGHT"; break;
        case Edge::NO_HANDLE: name = "NO_HANDLE"; break;
        default: name = "INVALID_HANDLE"; break;
    }

    return engine->toScriptValue(QString("%1 (%2)").arg(name).arg(n));
}



QScriptValue edge_handle_to_script(QScriptEngine *engine, const Edge::Handle &h)
{
    QScriptValue sh = engine->newVariant(int(h));
    sh.setProperty("toString", engine->newFunction(edge_handle_toString));
    return sh;
}

void edge_handle_from_script(const QScriptValue &obj, Edge::Handle &h)
{
    h = Edge::Handle(obj.toInt32());
}

