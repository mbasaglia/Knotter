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
class Script_Graph;

/**
 *  \brief Wrapper to Node
 */
class Script_Node : public QObject
{
    Q_OBJECT

    Q_PROPERTY(Script_Point pos READ pos WRITE set_pos )
    Q_PROPERTY(double x READ x WRITE set_x )
    Q_PROPERTY(double y READ y WRITE set_y )

    Q_PROPERTY(QObjectList edges READ edges)

    /// \todo : Style

    Node* m_wrapped_node;
    Script_Graph* graph;

public:
    Script_Node(Node* n, Script_Graph* graph);
    Script_Node ( const Script_Node& o);
    
    Script_Point pos() const;
    void set_pos(Script_Point p);
    double x () const { return pos().x(); }
    double y () const { return pos().y(); }
    void set_x(double x);
    void set_y(double y);

    QObjectList edges() const;

    /**
     * @brief Get the wrapped node
     * @return The wrapped node
     */
    Node* wrapped_node() const { return m_wrapped_node; }

    Q_INVOKABLE QString toString() const;

    Q_INVOKABLE bool has_edge_to(Script_Node *n) const;

signals:
    void moved(Script_Point p);

};


#endif // SCRIPT_NODE_HPP
