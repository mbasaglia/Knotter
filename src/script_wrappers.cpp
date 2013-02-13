/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012  Mattia Basaglia

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

#include "script_wrappers.hpp"

script_path_builder::script_path_builder(path_builder* pb) : pb(pb)
{
    qRegisterMetaType<script_point>("script_point");
    qRegisterMetaType<script_line>("script_line");
}

/// wrapper to line constructors
QScriptValue build_line (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() == 2 ) // point constructor
    {
        script_point arg1 = qscriptvalue_cast<script_point> (context->argument(0));
        script_point arg2 = qscriptvalue_cast<script_point> (context->argument(1));

        return engine->toScriptValue(script_line(arg1,arg2));
    }
    else if ( context->argumentCount() == 1 ) // copy constructor
    {
        return engine->toScriptValue( qscriptvalue_cast<script_line> (context->argument(0)) );
    }
    else // empty line
        return engine->toScriptValue( script_line() );
}

QScriptValue line_to_script(QScriptEngine *engine, const script_line &l)
{
    QScriptValue obj = engine->newQObject(new script_line(l));
    return obj;
}

void line_from_script(const QScriptValue &obj, script_line &l)
{
    l.setP1 ( obj.property("p1").toVariant().value<script_point>() );
    l.setP2 ( obj.property("p2").toVariant().value<script_point>() );
}

/// wrapper to point constructors
QScriptValue build_point (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() == 2 ) // point constructor
    {
        double x = context->argument(0).toNumber();
        double y = context->argument(1).toNumber();

        return engine->toScriptValue(script_point(x,y));
    }
    else if ( context->argumentCount() == 1 ) // copy constructor
    {
        return engine->toScriptValue( qscriptvalue_cast<script_point> (context->argument(0)) );
    }
    else // empty point
        return engine->toScriptValue( script_point() );
}

QScriptValue point_to_script(QScriptEngine *engine, const script_point &p)
{
    QScriptValue obj = engine->newObject();
    obj.setProperty("x",p.x());
    obj.setProperty("y",p.y());
    return obj;
}

void point_from_script(const QScriptValue &obj, script_point &p)
{
    p.setX ( obj.property("x").toNumber() );
    p.setY ( obj.property("y").toNumber() );
}

/// wrapper to operator-(point,point)
QScriptValue subtract_points (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() == 2 )
    {
        script_point arg1 = qscriptvalue_cast<script_point> (context->argument(0));
        script_point arg2 = qscriptvalue_cast<script_point> (context->argument(1));

        return engine->toScriptValue(arg1-arg2);
    }
    else
        return  engine->toScriptValue(script_point());
}

/// wrapper to operator-(point)
QScriptValue opposite_point (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() >= 1 )
    {
        script_point p = qscriptvalue_cast<script_point> (context->argument(0));

        return engine->toScriptValue(-p);
    }
    else
        return  engine->toScriptValue(script_point());
}



void initialize_engine ( QScriptEngine* engine )
{
    qScriptRegisterMetaType(engine, line_to_script, line_from_script);
    qScriptRegisterMetaType(engine, point_to_script, point_from_script);
    engine->globalObject().setProperty("line", engine->newFunction(build_line));
    engine->globalObject().setProperty("point", engine->newFunction(build_point));
    engine->globalObject().setProperty("diff", engine->newFunction(subtract_points));
    engine->globalObject().setProperty("opposite", engine->newFunction(opposite_point));
}
