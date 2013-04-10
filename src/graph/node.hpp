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
#ifndef NODE_HPP
#define NODE_HPP

#include <QPointF>
#include <QObject>

class Edge;

class Node : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF position READ position WRITE set_position NOTIFY moved)

private:
    QPointF pos;
    QList<Edge*> edges;

public:
    Node(QPointF pos = QPointF(), QObject* parent=0 ) : QObject(parent),pos(pos) {}

    QPointF position() const { return pos; }

    /**
     *  Add edge to node
     *  \param e Edge to be added
     *  \pre e is not already in the list of edges and this is one of its vertices
     */
    void add_edge(Edge* e);
    /**
     *  Remove edge from node
     *  \param e Edge to be removed
     *  \pre e is in the edge list
     */
    void remove_edge(Edge*e);
    /**
     *  Whether exists an edge from this node to the given node
     */
    bool has_edge_to(const Node*n) const;


public slots:
    void set_position(QPointF p);

signals:
    /// Emitted when the position is changed
    void moved(QPointF);
};

#endif // NODE_HPP
