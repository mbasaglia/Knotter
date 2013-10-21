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

#include "script_graph.hpp"
#include "resource_manager.hpp"
#include "xml_loader.hpp"

Script_Graph::Script_Graph(const Graph &graph, QObject *parent) :
    QObject(parent),
    m_style(graph.default_edge_style(),graph.default_node_style(),
            graph.colors())
{
    from_graph(graph);
    QObject::connect(&m_style,SIGNAL(style_changed(Node_Style,Edge_Style,Node_Style,Edge_Style)),
            SIGNAL(style_changed(Node_Style,Edge_Style,Node_Style,Edge_Style)));
    QObject::connect(&m_style,SIGNAL(colors_changed()),SIGNAL(colors_changed()));
}

Script_Graph::Script_Graph(const Script_Graph &g)
    : QObject(g.parent()),
      m_style(g.m_style)
{
    QObject::connect(&m_style,SIGNAL(style_changed(Node_Style,Edge_Style,Node_Style,Edge_Style)),
            SIGNAL(style_changed(Node_Style,Edge_Style,Node_Style,Edge_Style)));
    QObject::connect(&m_style,SIGNAL(colors_changed()),SIGNAL(colors_changed()));
}

void Script_Graph::from_graph(const Graph &graph)
{

    foreach ( Node* n, graph.nodes() )
    {
        if ( !node_map.contains(n) )
            add_node(n);
    }

    foreach ( Edge* e, graph.edges() )
    {
        if ( !edge_map.contains(e) )
            add_edge(e);
    }


}

Graph *Script_Graph::create_graph() const
{
    Graph* g = new Graph;
    foreach ( Script_Node* n, nodes() )
        g->add_node(n->wrapped_node());
    foreach ( Script_Edge* e, edges() )
        g->add_edge(e->wrapped_edge());
    return g;
}

Script_Node *Script_Graph::add_node(Node *n)
{

    Script_Node* sn =  new Script_Node(n,this);
    node_map[n] = sn;
    QObject::connect(sn,SIGNAL(moved(Script_Point)), SLOT(emit_node_moved(Script_Point)));
    QObject::connect(n,SIGNAL(destroyed()), SLOT(node_removed()));
    QObject::connect(sn,SIGNAL(style_changed(Node*,Node_Style,Node_Style)),
                     SIGNAL(node_style_changed(Node*,Node_Style,Node_Style)));
    m_nodes.push_back(sn);
    return sn;
}

QObject* Script_Graph::add_node(Script_Point p)
{
    Script_Node* n = add_node(new Node(p));
    emit node_added(n);
    return n;
}

QObject *Script_Graph::add_node(double x, double y)
{
    return add_node(Script_Point(x,y));
}

void Script_Graph::remove_node(QObject*n)
{
    Script_Node * node = qobject_cast<Script_Node*>(n);
    if ( node && node->parent() == this )
    {
        m_nodes.removeAll(node);
        emit node_removed(node);
        foreach(Script_Edge* e, node->edges())
            remove_edge(e);
    }
}

void Script_Graph::remove_edge(QObject *n)
{
    Script_Edge * edge = qobject_cast<Script_Edge*>(n);
    if ( edge && edge->parent() == this )
    {
        m_edges.removeAll(edge);
        emit edge_removed(edge);
    }
}

Script_Edge* Script_Graph::add_edge(Edge *e)
{

    Script_Edge* se =  new Script_Edge(e,this);
    edge_map[e] = se;
    QObject::connect(e,SIGNAL(destroyed()), SLOT(edge_removed()));
    QObject::connect(se,SIGNAL(style_changed(Edge*,Edge_Style,Edge_Style)),
                     SIGNAL(edge_style_changed(Edge*,Edge_Style,Edge_Style)));
    m_edges.push_back(se);
    return se;
}

QObject *Script_Graph::connect(QObject *on1, QObject *on2)
{
    Script_Node* n1 = qobject_cast<Script_Node*>(on1);
    Script_Node* n2 = qobject_cast<Script_Node*>(on2);

    if ( !n1 || !n2 || n1->wrapped_node() == n2->wrapped_node() )
        return nullptr;

    if ( n1->has_edge_to(n2) )
        return n1->edge_to(n2);

    Script_Edge * e = add_edge( new Edge(n1->wrapped_node(), n2->wrapped_node(),
                                        Resource_Manager::default_edge_type()) );
    emit edge_added(e);
    return e;
}

QObject *Script_Graph::node_at(Script_Point p)
{
    foreach(Script_Node *n, nodes())
    {
        if ( qFuzzyCompare(p,n->pos()) )
            return n;
    }

    return nullptr;
}

QObject *Script_Graph::node_at(double x, double y)
{
    return node_at(Script_Point(x,y));
}

QList<Script_Node *> Script_Graph::nodes() const
{
    return m_nodes;
}

QList<Script_Edge *> Script_Graph::edges() const
{
    return m_edges;
}

Script_Edge *Script_Graph::script_edge(Edge *e)
{
    if ( edge_map.contains(e) )
        return edge_map[e];
    else
        return add_edge(e);
}

Script_Node *Script_Graph::script_node(Node *n)
{
    if ( node_map.contains(n) )
        return node_map[n];
    else
        return add_node(n);
}

QString Script_Graph::toString() const
{
    return "[graph]";
}

void Script_Graph::emit_node_moved(Script_Point pos)
{
    Script_Node *n = qobject_cast<Script_Node*>(sender());
    if ( n )
        emit node_moved(n,pos);
}

void Script_Graph::node_removed()
{
    Node *n = qobject_cast<Node*>(sender());
    if ( n )
    {
        m_nodes.removeAll(node_map[n]);
        delete node_map[n];
        node_map.remove(n);
    }
}

void Script_Graph::edge_removed()
{
    Edge *e = qobject_cast<Edge*>(sender());
    if ( e )
    {
        m_edges.removeAll(edge_map[e]);
        delete edge_map[e];
        edge_map.remove(e);
    }
}


QObjectList Script_Graph::nodes_object()
{
    QObjectList l;
    foreach(Script_Node *n, nodes())
        l << n;
    return l;
}

QObjectList Script_Graph::selected_nodes_object()
{
    QObjectList l;
    foreach(Script_Node *n, nodes())
        if ( n->selected() )
            l << n;
    return l;
}

QObjectList Script_Graph::edges_object()
{
    QObjectList l;
    foreach(Script_Edge *e, edges())
        l << e;
    return l;
}




QScriptValue build_graph (QScriptContext *, QScriptEngine * engine)
{
    return engine->toScriptValue( Script_Graph() );
}

QScriptValue graph_to_script(QScriptEngine *engine, const Script_Graph &g)
{
    QScriptValue obj = engine->newQObject(new Script_Graph(g),QScriptEngine::ScriptOwnership);
    return obj;
}

void graph_from_script(const QScriptValue &obj, Script_Graph &graph)
{
    QObjectList nodes = qscriptvalue_cast<QObjectList>(obj.property("nodes"));
    QObjectList edges = qscriptvalue_cast<QObjectList>(obj.property("edges"));
    foreach( QObject* o, nodes )
    {
        Script_Node *n = qobject_cast<Script_Node *>(o);
        if ( n )
            graph.add_node(n->wrapped_node());
    }


    foreach( QObject* o, edges )
    {
        Script_Edge *e = qobject_cast<Script_Edge*>(o);
        if ( e )
            graph.add_edge(e->wrapped_edge());
    }
}



QObjectList Script_Graph::nodes_at(Script_Point p, double radius)
{
    QObjectList nl;

    foreach(Script_Node *n, nodes())
    {
        if ( point_distance_squared(n->pos(),p) <= radius*radius )
            nl << n;
    }

    return nl;
}

QObjectList Script_Graph::nodes_at(double x, double y, double radius)
{
    return nodes_at(Script_Point(x,y),radius);
}


bool Script_Graph::append(QString file, bool keep_style, Script_Point offset)
{
    Graph graph;
    QFile knot_file(file);
    if ( ! import_xml(knot_file,graph) )
        return false;

    foreach(Node* n, graph.nodes())
    {
        n->setPos(n->pos()+offset);
        if ( keep_style )
        {
            Node_Style ns = n->style().default_to(graph.default_node_style());
            n->set_style(ns);
        }
        graph.remove_node(n);
        emit node_added(add_node(n));
    }


    foreach(Edge* e, graph.edges())
    {
        graph.remove_edge(e);
        emit edge_added(add_edge(e));
    }

    return true;
}

void Script_Graph::clear()
{
    foreach(Script_Node*n, nodes())
    {
        remove_node(n);
    }
}

void Script_Graph::append(QObject *other)
{
    Script_Graph* other_graph = qobject_cast<Script_Graph*>(other);
    if ( other_graph )
    {
        QMap<Script_Node*,Script_Node*> other_nodes;
        foreach(Script_Node* n, other_graph->m_nodes )
        {
            Script_Node* own_node = add_node(n->wrapped_node());
            other_nodes[n] = own_node;
            emit node_added(own_node);
        }
        //other_graph->m_nodes.clear();

        foreach(Script_Edge* e, other_graph->m_edges )
        {
            Script_Edge* own_edge = add_edge(e->wrapped_edge());
            emit edge_added(own_edge);
        }
        //other_graph->m_edges.clear();
    }
}

void Script_Graph::copy_style(const Graph &copy)
{
    m_style.copy_style(copy);
}

