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
#include "graph.hpp"

Graph::Graph(QObject *parent) :
    QObject(parent)
{
}

void Graph::add_node(Node *n)
{
    nodes.append(n);
    connect(n,SIGNAL(moved(QPointF)),this,SLOT(update()));
}

void Graph::add_edge(Edge *e)
{
    edges.append(e);
    e->vertex1()->add_edge(e);
    update();
}

void Graph::remove_node(Node *n)
{
    nodes.removeOne(n);
    update();
}

void Graph::remove_edge(Edge *e)
{
    edges.removeOne(e);
    e->detach();
    update();
}

void Graph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    foreach(Edge* e, edges)
        e->paint(painter,option,widget);
    foreach(Node* n, nodes)
        n->paint(painter,option,widget);
}

void Graph::update()
{
    emit graph_changed();
}

