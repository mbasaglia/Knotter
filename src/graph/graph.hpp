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
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <QObject>
#include "node.hpp"
#include "edge.hpp"

class Graph : public QObject
{
    Q_OBJECT

private:
    QList<Node*> nodes;
    QList<Edge*> edges;

public:
    explicit Graph(QObject *parent = 0);

    /**
     *  Add node to graph
     *  \param n Node to be added
     *  \pre n is not already in the graph
     *  \post n is in the graph
     *  \note Signals from n are connected to this graph
     */
    void add_node(Node* n);
    /**
     *  Add edge to graph
     *  \param e Edge to be added
     *  \pre e is not already in the graph
     *  \post e is in the graph
     */
    void add_edge(Edge* e);
    /**
     *  Remove node from graph
     *  \param n Node to be removed
     *  \pre n is in the graph and has no connected edges
     *  \post n is not in the graph
     */
    void remove_node(Node* n);
    /**
     *  Remove edge from graph
     *  \param e Edge to be removed
     *  \pre e is in the graph
     *  \post e is not in the graph
     */
    void remove_edge(Edge* e);

public slots:
    /// Render knot again
    void update();
    
signals:
    /// Emitted when nodes or edges have changed and requires redrawing
    void graph_changed();
    
};

#endif // GRAPH_HPP
