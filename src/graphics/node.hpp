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
    \brief Information of the way an edge has be traversed
*/
struct TraversalInfo
{
    enum handside_type { LEFT, RIGHT };

    Edge* edge_in;                  ///< Input Edge
    Edge::handle_type handle_in;    ///< Input Handle
    Edge* edge_out;                 ///< Output Edge
    Edge::handle_type handle_out;   ///< Output Handle
    handside_type handside;         ///< Side of the input handle relative to the input edge
    double angle_in;                ///< Angle of the input edge relative to the vertex bewteen the two edges
    double angle_out;               ///< Angle of the output edge relative to the vertex bewteen the two edges
    double angle_delta;             ///< Difference between angle_in and angle_out (clock or counter depends on handside)
    bool success;                   ///< Input parameters were OK
    QString error_message;          ///< If !success, error message

    TraversalInfo ( QString error_message )
        : edge_in ( NULL ), handle_in ( Edge::NOHANDLE ),
            edge_out ( NULL ), handle_out ( Edge::NOHANDLE ),
            angle_in(0), angle_out(0), angle_delta(0),
            success(false), error_message ( error_message )
    {}

    TraversalInfo()
        : edge_in ( NULL ), handle_in ( Edge::NOHANDLE ),
            edge_out ( NULL ), handle_out ( Edge::NOHANDLE ),
            angle_in(0), angle_out(0), angle_delta(0),
            success(false), error_message ()
    {}


};

/**
\brief Edge between two nodes

This class is responsible to draw the node in the KnotView
and to perform the logic of the node, in particular is responsible for the graph traversal
*/
class Node : public CustomItem
{
    protected:
        static const int radius = 5; ///< Radius of the circle used to represent the node
        QList<Edge*> edges;          ///< Edges connected to the node
        bool        custom_style_enabled;
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
        /// List of edges connected to the node
        QList<Edge*> links() const { return edges; }
        /// Adjacency list
        QList<Node*> linked_to() const;


        // curve-building functions:

        /**
            \brief find next edge in traversal

            If edge is connected to this node, finds out the next edge depending on handle
        */
        TraversalInfo next_edge ( Edge* edge, Edge::handle_type handle ) const;

        bool has_custom_style() const { return custom_style_enabled; }
        const styleinfo& get_custom_style() const { return custom_style; }

        void set_custom_style ( styleinfo si );
        void disable_custom_style ();
        void enable_custom_style ();
};

/// Used very often
typedef QList<Node*> node_list;

#endif // NODE_HPP
