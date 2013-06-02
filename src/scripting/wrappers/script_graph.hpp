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
    Q_PROPERTY(QObjectList selected_nodes READ selected_nodes_object)


    QMap<Node*,Script_Node*> node_map;
    QMap<Edge*,Script_Edge*> edge_map;

public:
    explicit Script_Graph(const Graph &graph, QObject *parent = 0);

    /**
     * @brief Populate from actual graph
     */
    void from_graph(const Graph &graph);
    /**
     * @brief Create a Graph from nodes and edges
     * @return a new Graph object
     * \note It's needed only if this graph is not connected to a Script_Document
     */
    Graph* create_graph() const;

    /**
     * @brief Add a new wrapped node
     *
     * This function adds a node and connects the needed signals but does not
     * emit node_added().
     * It is intended as internal function, not directly invokable from scripts.
     *
     * @param n The real node that needs to be wrapped
     * @return The wrapped node
     */
    Script_Node* add_node(Node* n);
    /**
     * \brief Add a new node at a given position
     *
     * Calls add_node(Node*) and emits node_added()
     *
     * \return A script object
     */
    Q_INVOKABLE QObject *add_node(Script_Point p);
    /**
     * \brief Add a new node at a given position
     *
     * Calls add_node(Node*) and emits node_added()
     *
     * \return A script object
     */
    Q_INVOKABLE QObject *add_node(double x,double y);


    /**
     * @brief Add a new wrapped edge
     *
     * This function adds a edge and connects the needed signals but does not
     * emit edge_added().
     * It is intended as internal function, not directly invokable from scripts.
     *
     * @param e The real edge that needs to be wrapped
     * @return The wrapped edge
     */
    Script_Edge* add_edge(Edge *e);
    /**
     * \brief Add a new edge connecting the given nodes
     *
     * Emits edge_added()
     *
     * \return A script object
     */
    Q_INVOKABLE QObject *connect(Script_Node* n1, Script_Node*n2);

    /**
     * \brief Find node at position
     */
    Q_INVOKABLE QObject *node_at(Script_Point p);
    /**
     * \brief Find node at position
     */
    Q_INVOKABLE QObject *node_at(double x, double y);

    /**
     * \brief List of nodes
     */
    QList<Script_Node*> nodes ();
    /**
     * \brief List of edges
     */
    QList<Script_Edge*> edges ();

    /**
     * \brief Get wrapper script edge for given edge
     */
    Script_Edge* script_edge(Edge*) const;

    Q_INVOKABLE QString toString() const;


signals:
    void node_added(Script_Node* n);
    void edge_added(Script_Edge* e);
    void node_moved(Script_Node* n, Script_Point pos);

private slots:
    void emit_node_moved(Script_Point pos);
    void node_removed();
    void edge_removed();

private:
    /**
     * \brief List of nodes, as seen from the script
     */
    QObjectList nodes_object();
    /**
     * \brief List of selected nodes, as seen from the script
     */
    QObjectList selected_nodes_object();
    /**
     * \brief List of edges, as seen from the script
     */
    QObjectList edges_object();
    
};

Q_DECLARE_METATYPE(Script_Graph*)

#endif // SCRIPT_GRAPH_HPP
