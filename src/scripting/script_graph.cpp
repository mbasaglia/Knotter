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

Script_Graph::Script_Graph(const Graph &graph, QObject *parent) :
    QObject(parent)
{
    blockSignals(true);

    QMap<Node*,Script_Node*> nodes;

    foreach ( Node* n, graph.nodes() )
        nodes[n] = add_node(n);

    foreach(Edge* e, graph.edges())
        connect(nodes[e->vertex1()],nodes[e->vertex2()]);

    blockSignals(false);
}


QObject* Script_Graph::add_node(Script_Point p)
{
    Script_Node* n = new Script_Node(p,this);
    QObject::connect(n,SIGNAL(moved(Script_Point)),SLOT(emit_node_moved(Script_Point)));
    emit node_added(n);
    return n;
}

QObject *Script_Graph::connect(Script_Node *n1, Script_Node *n2)
{
    Script_Edge * e = new Script_Edge(n1,n2,this);
    emit edge_added(e);
    return e;
}

Script_Node *Script_Graph::node_at(Script_Point p)
{
    foreach(Script_Node *n, nodes())
    {
        if ( qFuzzyCompare(p,n->pos()) )
            return n;
    }

    return nullptr;
}

QList<Script_Node *> Script_Graph::nodes()
{
    return findChildren<Script_Node*>();
}

QList<Script_Edge *> Script_Graph::edges()
{
    return findChildren<Script_Edge*>();
}


Script_Node *Script_Graph::add_node(Node *n)
{
    Script_Node* sn =  new Script_Node(n,this);
    QObject::connect(sn,SIGNAL(moved(Script_Point)),SLOT(emit_node_moved(QPointF)));
    return sn;
}


void Script_Graph::emit_node_moved(Script_Point pos)
{
    Script_Node *n = qobject_cast<Script_Node*>(sender());
    if ( n )
        emit node_moved(n,pos);
}

QObjectList Script_Graph::nodes_object()
{
    QObjectList l;
    foreach(Script_Node *n, nodes())
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
