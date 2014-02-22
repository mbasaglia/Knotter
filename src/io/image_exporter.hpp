/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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
#include "background_image.hpp"

/**
 * @brief Export SVG Image
 *
 *  \param[out] file     Device to paint to
 *  \param graph         Graph to be rendered (must have already built the knot)
 *  \param draw_graph    Whether to render also the graph itself
 *  \param draw_bg_image Whether to render the background image
 *  \param bg_img        Background image
 */
void export_svg(QIODevice &file, const Graph& graph, bool draw_graph,
                bool draw_bg_image = false, const Background_Image &bg_img = Background_Image());


/**
 * \brief Export a raster image
 *
 *  \param[out] file    Device to paint to
 *  \param graph        Graph to be rendered (must have already built the knot)
 *  \param background   Background color to fill before painting
 *  \param antialias    Whether to perform supersampling with bilinear filtering
 *  \param img_size     Size of the image
 *  \param quality      Quality/Compression (See QPixmap::save())
 *  \param draw_graph   Whether to render also the graph itself
 *  \param draw_bg_image Whether to render the background image
 *  \param bg_img        Background image
 *  \param format       Name of the output format, if \c nullptr is deduced from the file name
*/
void export_raster(QIODevice &file, const Graph& graph, QColor background,
                   bool antialias, QSize img_size, int quality , bool draw_graph,
                   bool draw_bg_image = false, const Background_Image &bg_img = Background_Image(),
                   const char* format = nullptr);


#endif // IMAGE_EXPORTER_HPP
