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

#ifndef SCRIPT_POINT_HPP
#define SCRIPT_POINT_HPP


#include <QMetaType>
#include <QScriptEngine>
#include <QPointF>

/**
    Wrapper to QPointF
*/
typedef QPointF Script_Point;
Q_DECLARE_METATYPE(Script_Point)



/// Wrapper to line constructors
QScriptValue build_point (QScriptContext *context, QScriptEngine *engine);
/// Convert C++ object to JS object
QScriptValue point_to_script(QScriptEngine *engine, const Script_Point &p);
/// Convert JS object to C++ Object
void point_from_script(const QScriptValue &obj, Script_Point &p);

#endif // SCRIPT_POINT_HPP
