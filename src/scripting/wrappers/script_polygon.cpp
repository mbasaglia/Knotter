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

#include "script_polygon.hpp"
#include <limits>
#include "point_math.hpp"

Script_Polygon::Script_Polygon(QObject *parent) :
    QObject(parent)
{
}

bool Script_Polygon::contains(Script_Point p)
{
    return wrapped.containsPoint(p,Qt::WindingFill);
}

bool Script_Polygon::contains(double x, double y)
{
    return wrapped.containsPoint(QPointF(x,y),Qt::WindingFill);
}

void Script_Polygon::add_vertex(Script_Point p)
{
    wrapped.push_back(p);
}


QString Script_Polygon::toString() const
{
    QString s = "[polygon ";
    foreach(QPointF p, wrapped)
    {
        s += QString("(%1,%2), ").arg(p.x()).arg(p.y());
    }
    s.remove(s.size()-2,2);
    s += "]";
    return s;
}


QObjectList Script_Polygon::vertices() const
{
    QObjectList l;
    foreach(QPointF p, wrapped)
    {
        l << new Script_Point(p);
    }
    return l;
}

void Script_Polygon::set_vertices(QObjectList v)
{
    wrapped.clear();
    foreach(QObject* o, v)
    {
        Script_Point* p = qobject_cast<Script_Point*>(o);
        if ( p )
        {
            wrapped.push_back(*p);
        }
    }
}


QScriptValue build_polygon (QScriptContext *context, QScriptEngine *engine)
{
    Script_Polygon poly;

    if ( context->argumentCount() == 1 && context->argument(0).isArray() )
    {
        QObjectList l = qscriptvalue_cast<QObjectList>(context->argument(0));
        poly.set_vertices(l);
    }
    else
    {
        for ( int i = 0; i < context->argumentCount(); i++ )
        {
            Script_Point p = qscriptvalue_cast<Script_Point> (context->argument(0));
            poly.add_vertex(p);
        }
    }
    return engine->toScriptValue(poly);
}

QScriptValue polygon_to_script(QScriptEngine *engine, const Script_Polygon &l)
{

    QScriptValue obj = engine->newQObject(new Script_Polygon(l),QScriptEngine::ScriptOwnership);
    return obj;
}

void polygon_from_script(const QScriptValue &obj, Script_Polygon &l)
{
    QObjectList points = qscriptvalue_cast<QObjectList>(obj.property("vertices"));
    l.set_vertices(points);
}
