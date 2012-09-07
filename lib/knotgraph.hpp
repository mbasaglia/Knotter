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
#include <QGraphicsPathItem>
#include "basic_knot_graph.hpp"

/**
    \brief Class representing the knot line

    This class will both evaluate the shape and render the knot
*/
class KnotGraph : public QGraphicsPathItem, public basic_knot_graph
{
    protected:
        QPainterPathStroker stroker;            ///< Knot style

    public:
        KnotGraph();
        ~KnotGraph();
        void add ( CustomItem* what );

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

        Qt::PenJoinStyle get_join_style() const;
        void set_join_style(Qt::PenJoinStyle);

        void load_xml(class xml_loader&xml);

        void save_xml(class xml_saver&xml) const;

        bool load_xml(QIODevice* file);
        void save_xml(QIODevice* file) const;


        /// Draw the knot to device
        void paint_knot ( QPaintDevice* device, bool minimal );
        /// Draw the knot
        void paint_knot ( QPainter* painter, bool minimal );

        void export_svg(QIODevice &file, bool minimal);

        void export_raster(QIODevice &file, bool minimal,
            QColor background, bool antialias, QSize img_size, int quality);

};

#endif // KNOTGRAPH_HPP
