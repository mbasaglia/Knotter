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
#ifndef KNOTGRAPH_HPP
#define KNOTGRAPH_HPP
#include <QPainterPath>
#include "node.hpp"
#include "edge.hpp"
#include <QGraphicsPathItem>
#include "knot_curve_style.hpp"

/**
    \brief Class representing the knot line

    This class will both evaluate the shape and render the knot
*/
class KnotGraph : public QGraphicsPathItem
{
    protected:
        QList<Node*> nodes;                     ///< List of nodes
        QList<Edge*> edges;                     ///< List of edges (to be traversed)
        QList<Edge*> traversed_edges;           ///< List of edges (already fully traversed)
        QPainterPathStroker stroker;            ///< Knot style
        knot_curve_styler::style_id curve_style;///< Knot shape
        double cusp_angle;                      ///< Min cusp angle
        double handle_length;                   ///< Length of handles (line from start point to control point in SVG curves )
        double crossing_distance;               ///< Crossing gap value

    public:
        KnotGraph();
        ~KnotGraph();
        void add_node(Node* which);
        void add_edge ( Edge* which );
        void remove_node(Node* which);
        void remove_edge ( Edge* which );
        void add ( CustomItem* what );
        void clear();   ///< remove all edges and nodes

        /**
            \brief Update the path

            This function traverses the graph and contructs the knot line.

            \return the thin resulting path, without any style
        */
        QPainterPath build();

        /// set knot path width
        void set_width ( double w );
        /// get knot path width
        double get_width ( ) const;

        knot_curve_styler::style_id get_curve_style() const;
        void set_curve_style(knot_curve_styler::style_id);


        double get_cusp_angle() const;
        void set_cusp_angle(double ca);

        double get_handle_length() const;
        void set_handle_length(double hl);

        double get_crossing_distance() const;
        void set_crossing_distance(double cd);

        Qt::PenJoinStyle get_join_style() const;
        void set_join_style(Qt::PenJoinStyle);

};

#endif // KNOTGRAPH_HPP
