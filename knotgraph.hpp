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

class KnotGraph : public QGraphicsPathItem
{
    protected:
        QPainterPath knot;
        QList<Node*> nodes;
        QList<Edge*> edges;
        QList<Edge*> traversed_edges;
        QPainterPathStroker stroker;
        knot_curve_styler::style_id curve_style;
        double cusp_angle;
        double handle_length;
        double crossing_distance;

    public:
        KnotGraph();
        ~KnotGraph();
        void add_node(Node* which);
        void add_edge ( Edge* which );
        void remove_node(Node* which);
        void remove_edge ( Edge* which );
        void add ( CustomItem* what );

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

};

#endif // KNOTGRAPH_HPP
