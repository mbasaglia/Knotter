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

#include "script_color.hpp"

Script_Color::Script_Color(QColor col, QObject *parent) :
    QObject(parent),QColor(col)
{
}


QScriptValue build_color (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() == 1 )
    {
        if ( context->argument(0).isString() ) // Color('Color name')
            return engine->toScriptValue(Script_Color(QColor(
                                            context->argument(0).toString())));
        else // copy constructor
            return engine->toScriptValue( qscriptvalue_cast<Script_Color> (context->argument(0)) );

    }
    else if ( context->argumentCount() == 3 ) // RGB
    {
        return engine->toScriptValue(Script_Color(QColor(
                                         context->argument(0).toNumber(),
                                         context->argument(1).toNumber(),
                                         context->argument(2).toNumber()
                                         )));
    }
    else if ( context->argumentCount() == 4 ) // RGBA
    {
        return engine->toScriptValue(Script_Color(QColor(
                                         context->argument(0).toNumber(),
                                         context->argument(1).toNumber(),
                                         context->argument(2).toNumber(),
                                         context->argument(3).toNumber()
                                         )));
    }
    else // empty color
        return engine->toScriptValue( Script_Color() );
}

QScriptValue color_to_script(QScriptEngine *engine, const Script_Color &c)
{
    QScriptValue obj = engine->newQObject(new Script_Color(c),QScriptEngine::ScriptOwnership);
    return obj;
}

void color_from_script(const QScriptValue &obj, Script_Color &c)
{
    c.setRed ( obj.property("red").toNumber() );
    c.setGreen ( obj.property("green").toNumber() );
    c.setBlue ( obj.property("blue").toNumber() );
    c.setAlpha ( obj.property("alpha").toNumber() );
}



QString Script_Color::toString() const
{
    return name();
}


QScriptValue script_rgb(QScriptContext *context, QScriptEngine *engine)
{
    int alpha = context->argumentCount() > 3 ? context->argument(3).toNumber() : 255;

    return engine->toScriptValue(Script_Color(QColor(
                                     context->argument(0).toNumber(),
                                     context->argument(1).toNumber(),
                                     context->argument(2).toNumber(),
                                     alpha
                                    )));
}

QScriptValue script_hsv(QScriptContext *context, QScriptEngine *engine)
{
    int alpha = context->argumentCount() > 3 ? context->argument(3).toNumber() : 255;

    return engine->toScriptValue(Script_Color(QColor::fromHsv(
                                     context->argument(0).toNumber(),
                                     context->argument(1).toNumber(),
                                     context->argument(2).toNumber(),
                                     alpha
                                    )));
}

QScriptValue script_hsl(QScriptContext *context, QScriptEngine *engine)
{
    int alpha = context->argumentCount() > 3 ? context->argument(3).toNumber() : 255;

    return engine->toScriptValue(Script_Color(QColor::fromHsl(
                                     context->argument(0).toNumber(),
                                     context->argument(1).toNumber(),
                                     context->argument(2).toNumber(),
                                     alpha
                                    )));
}
QScriptValue script_cmyk(QScriptContext *context, QScriptEngine *engine)
{
    int alpha = context->argumentCount() > 4 ? context->argument(4).toNumber() : 255;

    return engine->toScriptValue(Script_Color(QColor::fromCmyk(
                                     context->argument(0).toNumber(),
                                     context->argument(1).toNumber(),
                                     context->argument(2).toNumber(),
                                     context->argument(3).toNumber(),
                                     alpha
                                    )));
}
