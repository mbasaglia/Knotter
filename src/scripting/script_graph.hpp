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

#ifndef SCRIPT_GRAPH_HPP
#define SCRIPT_GRAPH_HPP

#include <QObject>
#include "graph.hpp"
#include "script_edge.hpp"

/**
 * @brief Wrapper to Graph
 */
class Script_Graph : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObjectList nodes READ nodes_object)
    Q_PROPERTY(QObjectList edges READ edges_object)

public:
    explicit Script_Graph(const Graph &graph, QObject *parent = 0);

    /**
     * @brief Create a Graph from nodes and edges
     * @return a new Graph object
     */
    Graph* create_graph() const;

    Script_Node* add_node(Node* n);
    Q_INVOKABLE QObject *add_node(Script_Point p);
    Q_INVOKABLE QObject *add_node(double x,double y);
    Q_INVOKABLE QObject *connect(Script_Node* n1, Script_Node*n2);

    Q_INVOKABLE Script_Node* node_at(Script_Point p);

    QList<Script_Node*> nodes ();
    QList<Script_Edge*> edges ();

signals:
    void node_added(Script_Node* n);
    void edge_added(Script_Edge* e);
    void node_moved(Script_Node* n, Script_Point pos);

private slots:
    void emit_node_moved(Script_Point pos);

private:
    QObjectList nodes_object();
    QObjectList edges_object();
    
};

Q_DECLARE_METATYPE(Script_Graph*)

#endif // SCRIPT_GRAPH_HPP
