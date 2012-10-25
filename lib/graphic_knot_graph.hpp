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
#include <QMimeData>

/**
    \brief Drawable class representing the knot line

    This class will both evaluate the shape and render the knot
*/
class KnotGraph : public QAbstractGraphicsShapeItem, public basic_knot_graph
{
    public:
        enum PaintingMode
        {
            NORMAL = 0x0, // full shape
            MINIMAL= 0x1, // outline (same style as NORMAL)
            LOOPS  = 0x2  // outline (one color per loop)
        };

    protected:
        QPainterPathStroker stroker;    ///< Knot style
        QList<QPainterPath> paths;      ///< loops (just the shape)
        QList<QPainterPath> stroked;      ///< loops (stroked)
        QRectF bounding_cache;
        PaintingMode paint_mode;

        void recalculate_rect_cache();

        void paint_minimal(QPainter *painter);
        void paint_loops(QPainter *painter);
        void paint_normal(QPainter *painter);

        void paint(QPainter *painter, PaintingMode paint_mode);

    public:
        KnotGraph();
        ~KnotGraph();
        KnotGraph& operator= ( const KnotGraph& o );

        QRectF boundingRect() const { return bounding_cache; }

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


        /// Draw the knot to device (normalize position to 0,0)
        void paint_knot ( QPaintDevice* device, PaintingMode mode );
        /// Draw the knot (normalize position to 0,0)
        void paint_knot ( QPainter* painter, PaintingMode mode );

        void set_paint_mode ( PaintingMode mode ) { paint_mode = mode; }
        PaintingMode get_paint_mode () const { return paint_mode; }

        void copy_style( const KnotGraph& o );

        void add ( CustomItem* what );

        /**
            \brief Update the path

            This function traverses the graph and contructs the knot line.

            \return the thin resulting path, without any style
        */
        //QPainterPath build();

        /**
            \brief Create a list of paths. Each path corresponds to a loop
        */
        const QList<QPainterPath> &build();

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

        void export_svg(QIODevice &file, PaintingMode mode);

        void export_raster(QIODevice &file, PaintingMode mode,
            QColor background, bool antialias, QSize img_size, int quality);


        void to_mime ( QMimeData* data );

};

#endif // KNOTGRAPH_HPP
