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

Edge_Scripted::Edge_Scripted(Plugin_Crossing *plugin)
    : plugin(plugin)
{
}

Edge::Handle Edge_Scripted::traverse(Edge *edge, Edge::Handle handle, Path_Builder &path) const
{
    Resource_Manager::run_script(plugin);
    /// \todo
    return Resource_Manager::default_edge_type()->traverse(edge,handle,path);
}

QLineF Edge_Scripted::handle(const Edge *edge, Edge::Handle handle) const
{
    /// \todo
    return Resource_Manager::default_edge_type()->handle(edge,handle);
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



QScriptValue edge_handle_to_script(QScriptEngine *engine, const Edge::Handle &h)
{
    return engine->newVariant(int(h));
}

void edge_handle_from_script(const QScriptValue &obj, Edge::Handle &h)
{
    h = Edge::Handle(obj.toInt32());
}

