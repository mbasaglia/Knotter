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

#include "script_point.hpp"

QScriptValue point_to_script(QScriptEngine *engine, const Script_Point &p)
{
    QScriptValue obj = engine->newObject();
    obj.setProperty("x",p.x());
    obj.setProperty("y",p.y());
    return obj;
}

void point_from_script(const QScriptValue &obj, Script_Point &p)
{
    p.setX ( obj.property("x").toNumber() );
    p.setY ( obj.property("y").toNumber() );
}


QScriptValue build_point (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() == 2 ) // point constructor
    {
        double x = context->argument(0).toNumber();
        double y = context->argument(1).toNumber();

        return engine->toScriptValue(Script_Point(x,y));
    }
    else if ( context->argumentCount() == 1 ) // copy constructor
    {
        return engine->toScriptValue( qscriptvalue_cast<Script_Point> (context->argument(0)) );
    }
    else // empty point
        return engine->toScriptValue( Script_Point() );
}


/// wrapper to operator-(point)
QScriptValue opposite_point (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() >= 1 )
    {
        Script_Point p = qscriptvalue_cast<Script_Point> (context->argument(0));

        return engine->toScriptValue(-p);
    }
    else
        return  engine->toScriptValue(Script_Point());
}
