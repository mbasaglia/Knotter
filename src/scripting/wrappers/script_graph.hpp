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
    Q_PROPERTY(QObject* crossing_style READ crossing_style)
    Q_PROPERTY(QObject* cusp_style READ cusp_style)

    friend void graph_from_script(const QScriptValue &obj, Script_Graph &graph);


    QMap<Node*,Script_Node*> node_map;
    QMap<Edge*,Script_Edge*> edge_map;

    QList<Script_Node*> m_nodes;
    QList<Script_Edge*> m_edges;

    Script_Edge_Style m_crossing_style;
    Script_Node_Style m_cusp_style;

public:
    explicit Script_Graph(const Graph &graph=Graph(), QObject *parent = 0);
    Script_Graph(const Script_Graph &g);

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


    Q_INVOKABLE void remove_node(QObject* n);

    Q_INVOKABLE void remove_edge(QObject* n);

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
    Q_INVOKABLE QObject *connect(QObject* n1, QObject*n2);

    /**
     * \brief Find node at position
     */
    Q_INVOKABLE QObject *node_at(Script_Point p);
    /**
     * \brief Find node at position
     */
    Q_INVOKABLE QObject *node_at(double x, double y);



    /**
     * \brief Find nodes near position
     */
    Q_INVOKABLE QObjectList nodes_at(Script_Point p, double radius);
    /**
     * \brief Find nodes near position
     */
    Q_INVOKABLE QObjectList nodes_at(double x, double y, double radius);

    /**
     * \brief List of nodes
     */
    QList<Script_Node*> nodes () const;
    /**
     * \brief List of edges
     */
    QList<Script_Edge*> edges () const;

    /**
     * \brief Get wrapper script edge for given edge
     */
    Script_Edge* script_edge(Edge*);
    /**
     *
     * \brief Get wrapper script node for given node
     */
    Script_Node* script_node(Node*);

    Q_INVOKABLE QString toString() const;

    /**
     * \brief Append contents from file
     * \param file          File name
     * \param keep_style    If true, use the file style as local style for each node
     * \param offset        Offset from the original location
     *
     * \note Calls to this function will result in the plain addition of nodes and edges,
     *       this will be visible as single command unless wrapped by document macros.
     */
    Q_INVOKABLE bool append(QString file, bool keep_style = false,
                            Script_Point offset=Script_Point());

    /**
     * \brief Remove all nodes and edges
     *
     * \note Calls to this function will result in the plain removal of nodes and edges,
     *       this will be visible as single command unless wrapped by document macros.
     */
    Q_INVOKABLE void clear();

    /**
     * \brief Merge nodes and edges from other graph
     *
     *  The contents of the other graph are copied to this graph.
     *
     *  \param other The other Script_Graph object.
     */
    Q_INVOKABLE void append(QObject* other);


    QObject* crossing_style() { return &m_crossing_style; }
    QObject* cusp_style () { return &m_cusp_style; }

signals:
    void node_added(Script_Node* n);
    void node_removed(Script_Node* n);
    void edge_added(Script_Edge* e);
    void edge_removed(Script_Edge* e);
    void node_moved(Script_Node* n, Script_Point pos);
    void edge_style_changed(Edge* edge, Edge_Style before, Edge_Style after);
    void node_style_changed(Node* node, Node_Style before, Node_Style after );
    void style_changed(Node_Style bef_node, Edge_Style bef_edge,
                       Node_Style aft_node, Edge_Style aft_edge );

private slots:
    void emit_node_moved(Script_Point pos);
    void node_removed();
    void edge_removed();
    void emit_style_changed(Node_Style before,Node_Style after);
    void emit_style_changed(Edge_Style before,Edge_Style after);

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

//Q_DECLARE_METATYPE(Script_Graph*)
Q_DECLARE_METATYPE(Script_Graph)


/// Wrapper to Graph constructor
QScriptValue build_graph (QScriptContext *context, QScriptEngine *engine);
/// Convert C++ object to JS object
QScriptValue graph_to_script(QScriptEngine *engine, const Script_Graph &l);
/// Convert JS object to C++ Object
void graph_from_script(const QScriptValue &obj, Script_Graph &graph);

#endif // SCRIPT_GRAPH_HPP
