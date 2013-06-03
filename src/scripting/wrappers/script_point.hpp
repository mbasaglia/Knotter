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
class Script_Point : public QObject, public QPointF
{
    Q_OBJECT
    Q_PROPERTY(double x READ x WRITE setX )
    Q_PROPERTY(double y READ y WRITE setY )
public:
    explicit Script_Point(double x = 0, double y = 0) : QPointF(x,y) {}
    Script_Point(const QPointF& o) : QPointF(o.x(), o.y()) {}
    Script_Point(const Script_Point& o) : QObject(o.parent()), QPointF(o.x(),o.y()) {}
    Script_Point& operator= (const Script_Point& o)
    {
        setX(o.x());
        setY(o.y());
        return *this;
    }

    Q_INVOKABLE QString toString()  const { return QString("(%1,%2)").arg(x()).arg(y()); }
};

Q_DECLARE_METATYPE(Script_Point)



/// Wrapper to line constructors
QScriptValue build_point (QScriptContext *context, QScriptEngine *engine);
/// Convert C++ object to JS object
QScriptValue point_to_script(QScriptEngine *engine, const Script_Point &p);
/// Convert JS object to C++ Object
void point_from_script(const QScriptValue &obj, Script_Point &p);


/// wrapper to operator-(point)
QScriptValue opposite_point (QScriptContext *context, QScriptEngine *engine);

#endif // SCRIPT_POINT_HPP
