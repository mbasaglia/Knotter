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

#ifndef SCRIPT_NODE_HPP
#define SCRIPT_NODE_HPP

#include <QObject>
#include "node.hpp"
#include "script_point.hpp"

class Script_Edge;

/**
 *  \brief Wrapper to Node
 */
class Script_Node : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Script_Point pos READ pos WRITE set_pos )
    Q_PROPERTY(double x READ x WRITE set_x )
    Q_PROPERTY(double y READ y WRITE set_y )

    Q_PROPERTY(QList<Script_Edge*> edges READ edges)

    /// \todo : Style

    Script_Point m_pos;
    QList<Script_Edge*> m_edges;
    Node* m_wrapped_node;

public:
    explicit Script_Node(Script_Point p = Script_Point(), QObject *parent = 0);
    Script_Node(Node* n, QObject *parent);
    Script_Node ( const Script_Node& o);
    
    Script_Point pos() const { return m_pos; }
    void set_pos(Script_Point p);
    double x () const { return m_pos.x(); }
    double y () const { return m_pos.y(); }
    void set_x(double x);
    void set_y(double y);

    QList<Script_Edge*> edges() const { return m_edges; }

    void set_edges(QList<Script_Edge*> edges)
    {
        m_edges = edges;
    }

    /**
     * @brief Get the wrapped node
     * @return The wrapped node, nullptr if not set
     */
    Node* wrapped_node() const { return m_wrapped_node; }
    /**
     * @brief Get the wrapped node
     * @return Will create a new instance if needed
     */
    Node* generate_wrapped_node();

signals:
    void moved(Script_Point p);


};

Q_DECLARE_METATYPE(Script_Node)
Q_DECLARE_METATYPE(QList<Script_Edge*>)
Q_DECLARE_METATYPE(QList<Script_Node*>)


/// Wrapper to Node constructors
QScriptValue build_node (QScriptContext *context, QScriptEngine *engine);
/// Convert C++ object to JS object
QScriptValue node_to_script(QScriptEngine *engine, const Script_Node &n);
/// Convert JS object to C++ Object
void node_from_script(const QScriptValue &obj, Script_Node &n);
#endif // SCRIPT_NODE_HPP
