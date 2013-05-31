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

#include "script_node.hpp"

Script_Node::Script_Node(Script_Point p, QObject *parent) :
    QObject(parent), m_pos(p), m_wrapped_node(nullptr)
{
}

Script_Node::Script_Node(Node *n, QObject *parent):
    QObject(parent), m_pos(n->pos()), m_wrapped_node(n)
{
}

Script_Node::Script_Node(const Script_Node &o)
    : QObject(o.parent()), m_pos(o.m_pos), m_edges(o.m_edges)
{
}

void Script_Node::set_pos(Script_Point p)
{
    m_pos = p;
    emit moved(p);
}

void Script_Node::set_x(double x)
{
    m_pos.setX(x);
    emit moved(m_pos);
}

void Script_Node::set_y(double y)
{
    m_pos.setY(y);
    emit moved(m_pos);
}

Node *Script_Node::generate_wrapped_node()
{
    if ( m_wrapped_node )
        return m_wrapped_node;

    m_wrapped_node = new Node(m_pos);
    return m_wrapped_node;
}



QScriptValue build_node (QScriptContext *context, QScriptEngine *engine)
{

    if ( context->argumentCount() == 2 ) // x,y
    {
        double x = qscriptvalue_cast<double> (context->argument(0));
        double y = qscriptvalue_cast<double> (context->argument(1));

        return engine->toScriptValue(Script_Node(Script_Point(x,y)));
    }
    else if ( context->argumentCount() == 1 ) // point
    {
        /// \todo copy constructor
        return engine->toScriptValue(Script_Node(
                    qscriptvalue_cast<Script_Point> (context->argument(0))) );
    }
    else // default (0,0)
        return engine->toScriptValue( Script_Node(Script_Point(0,0)) );
}

QScriptValue node_to_script(QScriptEngine *engine, const Script_Node &n)
{

    QScriptValue obj = engine->newQObject(new Script_Node(n),QScriptEngine::ScriptOwnership);
    return obj;
}

void node_from_script(const QScriptValue &obj, Script_Node &n)
{
    n.set_pos ( qscriptvalue_cast<Script_Point>(obj.property("pos")) );
    n.set_edges( qscriptvalue_cast<QList<Script_Edge*> >(obj.property("edges")) );
}
