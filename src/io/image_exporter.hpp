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

#ifndef IMAGE_EXPORTER_HPP
#define IMAGE_EXPORTER_HPP

#include "graph.hpp"

void paint_knot(const Graph& graph, QPaintDevice *device, bool draw_graph );

void export_svg(const Graph& graph, QIODevice &file, bool draw_graph);

/**
 * \brief Export a raster image
 *
 *  \param graph        Graph to be rendered (must have already built the knot)
 *  \param[out] file    Device to paint to
 *  \param background   Background color to fill before painting
 *  \param antialias    Whether to perform supersampling with bilinear filtering
 *  \param img_size     Size of the image
 *  \param quality      Quality/Compression (See QPixmap::save())
 *  \param draw_graph   Whether to render also the graph itself
*/
void export_raster(const Graph& graph, QIODevice &file, QColor background,
                   bool antialias, QSize img_size, int quality , bool draw_graph);

#endif // IMAGE_EXPORTER_HPP
