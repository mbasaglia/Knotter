/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012  Mattia Basaglia

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

#include "custom_item.hpp"
#include "edge.hpp"
#include "knot_curve_style.hpp"


/**
\brief Node

This class is responsible to draw the node in the KnotView
and to perform the logic of the node, in particular is responsible for the graph traversal

*/
class Node : public CustomItem
{
    protected:
        static const int radius = 5; ///< Radius of the circle used to represent the node
        QList<Edge*> edges;          ///< Edges connected to the node
        styleinfo   custom_style;

    public:

        Node(QPointF position);
        ~Node();

        QRectF boundingRect() const;

        QRectF actualRect() const;

        void paint( QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget);

        /// Get edge connecting this to other or NULL
        Edge* get_link(Node* other) const;
        void add_link ( Edge* edge );
        void remove_link ( Edge* which );
        void clear_links();
        /// List of edges connected to the node
        QList<Edge*> links() const { return edges; }
        /// Adjacency list
        QList<Node*> linked_to() const;

        bool has_custom_style() const { return custom_style.enabled_style != styleinfo::NOTHING; }
        const styleinfo& get_custom_style() const { return custom_style; }

        void set_custom_style ( styleinfo si );
        void disable_custom_style ();
};

/// Used very often
typedef QList<Node*> node_list;

#endif // NODE_HPP
