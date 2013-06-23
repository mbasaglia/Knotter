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

#include "misc_script_functions.hpp"
#include "resource_manager.hpp"

QScriptValue script_print( QScriptContext * context, QScriptEngine * )
{
    QString out;
    for ( int i = 0; i < context->argumentCount(); i++ )
    {
        out += context->argument(i).toString()+' ';
    }
    /*
     * Note: this is not debug code,
     * used qDebug instead of qWarning because this output is not a warning
     * but most likely debug output from the plugin
     */
    qDebug() << out;
    Resource_Manager::emit_script_output(out);
    return QScriptValue();
}

QString Script_Knotter::version() const
{
    return Resource_Manager::program_version();
}

bool Script_Knotter::has_version(int maj, int min)
{
    return Resource_Manager::has_least_version(maj,min);
}

QString Script_Knotter::toString() const
{
    return Resource_Manager::program_name()+" "+Resource_Manager::program_version();
}

QStringList Script_Knotter::edge_types()
{
    QStringList l;
    foreach(Edge_Type* s, Resource_Manager::edge_styles())
        l << s->machine_name();
    return l;
}

QStringList Script_Knotter::cusp_shapes()
{
    QStringList l;
    foreach(Cusp_Shape* s, Resource_Manager::cusp_shapes())
        l << s->machine_name();
    return l;
}
