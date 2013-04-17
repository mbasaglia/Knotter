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
#include "graph_item.hpp"
#include "node_style.hpp"
#include <QPainter>
#include "c++.hpp"

class Edge;

class Node : public Graph_Item
{
    Q_OBJECT

private:
    QList<Edge*> edges;
    static const int radius = 6; ///< \todo maybe not const and settings
    /// Extended radius
    static const int xradius = 7; ///< \todo maybe not const and settings
    /// \todo node style: pointer or value?

public:
    Node(QPointF pos, class Graph* parent );




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

    double distance_squared(QPointF to) const;

    QList<Edge*> connections() const { return edges; }

    int type() const override { return UserType + 0x01; }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* =0, QWidget* =0) override;
    QRectF boundingRect() const override;
};

#endif // NODE_HPP
