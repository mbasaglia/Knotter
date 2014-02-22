/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#ifndef SCRIPT_POLYGON_HPP
#define SCRIPT_POLYGON_HPP

#include <QObject>
#include <QPolygon>
#include "script_point.hpp"

class Script_Polygon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObjectList vertices READ vertices WRITE set_vertices)

    QPolygonF wrapped;
public:
    explicit Script_Polygon(QObject *parent = 0);
    Script_Polygon ( const Script_Polygon& o) : QObject(), wrapped(o.wrapped) {}

    Q_INVOKABLE bool contains(Script_Point p);
    Q_INVOKABLE bool contains(double x, double y);
    Q_INVOKABLE void add_vertex(Script_Point p);
    Q_INVOKABLE QString toString() const;

    QObjectList vertices() const;
    void set_vertices(QObjectList v);

    
};


Q_DECLARE_METATYPE(Script_Polygon)

/// Wrapper to polygon constructors
QScriptValue build_polygon (QScriptContext *context, QScriptEngine *engine);
/// Convert C++ object to JS object
QScriptValue polygon_to_script(QScriptEngine *engine, const Script_Polygon &l);
/// Convert JS object to C++ Object
void polygon_from_script(const QScriptValue &obj, Script_Polygon &l);

#endif // SCRIPT_POLYGON_HPP
