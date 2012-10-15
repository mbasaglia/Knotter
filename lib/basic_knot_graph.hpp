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
#ifndef BASIC_KNOT_GRAPH_HPP
#define BASIC_KNOT_GRAPH_HPP

#include <QList>
#include "path_builder.hpp"
#include "edge.hpp"
#include "node.hpp"
#include "knot_curve_style.hpp"

/**
    \brief Class representing the knot line

    This class represents and builds the knot but is not responsible for any
    appearence setting.

    \note This class does NOT take ownership over anything!
*/
class basic_knot_graph
{
    protected:
        QList<Node*> nodes; ///< List of nodes
        QList<Edge*> edges; ///< List of edges (to be traversed)
        QList<Edge*> traversed_edges; ///< List of edges (already fully traversed)
        styleinfo default_style; ///< Knot style

    public:
        basic_knot_graph();
        virtual ~basic_knot_graph() {}
        void add_node(Node* which);
        void add_edge ( Edge* which );
        /**
            \brief creates new edge and inserts it
            The edge is created only if a and b are not already linked
            \return The newly created Edge or NULL
            \note returns NULL if the edge was already there
        */
        Edge* add_edge(Node* a, Node* b);

        void set_edge_type(Node *a, Node *b, Edge::type_type type);

        void remove_node(Node* which);
        void remove_edge ( Edge* which );
        void clear(); ///< remove all edges and nodes
        void set_style_info( styleinfo new_style );
        styleinfo get_style_info() const;

        /**
            \brief Build the knot path
            This function traverses the graph and contructs the knot line.
        */
        void build_knotline(path_builder&path);

        const QList<Node*> get_nodes() const { return nodes; }
        const QList<Edge*> get_edges() const { return edges; }

};


#endif // BASIC_KNOT_GRAPH_HPP
