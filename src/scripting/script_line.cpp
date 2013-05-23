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

#include "script_line.hpp"
#include "c++.hpp"

/// wrapper to line constructors
QScriptValue build_line (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() == 2 ) // point constructor
    {
        Script_Point arg1 = qscriptvalue_cast<Script_Point> (context->argument(0));
        Script_Point arg2 = qscriptvalue_cast<Script_Point> (context->argument(1));

        return engine->toScriptValue(Script_Line(arg1,arg2));
    }
    else if ( context->argumentCount() == 1 ) // copy constructor
    {
        return engine->toScriptValue( qscriptvalue_cast<Script_Line> (context->argument(0)) );
    }
    else // empty line
        return engine->toScriptValue( Script_Line() );
}

QScriptValue line_to_script(QScriptEngine *engine, const Script_Line &l)
{
    QScriptValue obj = engine->newQObject(new Script_Line(l));
    return obj;
}


void line_from_script(const QScriptValue &obj, Script_Line &l)
{
    l.setP1 ( qscriptvalue_cast<Script_Point>(obj.property("p1")) );
    l.setP2 ( qscriptvalue_cast<Script_Point>(obj.property("p2")) );
}
