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

#include "image_exporter.hpp"

#include <QSvgGenerator>

void paint_knot(const Graph& graph , QPaintDevice *device)
{
    QPainter painter;
    painter.begin(device);

    graph.const_paint(&painter);

    painter.end();
}


void export_svg(const Graph& graph, QIODevice &file)
{
    QSvgGenerator gen;
    gen.setOutputDevice(&file);
    gen.setTitle("");
    gen.setDescription("");

    paint_knot ( graph, &gen );
}


void export_raster(const Graph& graph, QIODevice &file, QColor background,
                   bool antialias, QSize img_size, int quality )
{

    QSizeF actual_size = graph.boundingRect().size();
    double scale_x = img_size.width() / actual_size.width();
    double scale_y = img_size.height() / actual_size.height();


    QPixmap *pix;

    if ( antialias )
    {
        pix = new QPixmap(img_size);
        scale_x *= 2;
        scale_y *= 2;
    }
    else
        pix = new QPixmap(img_size*2);

    pix->fill(background);

    QPainter painter;
    painter.begin(pix);

    painter.scale(scale_x,scale_y);

    graph.const_paint(&painter);

    painter.end();

    if ( antialias )
        pix->scaled(img_size,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)
                .save(&file,0,quality);
    else
        pix->save(&file,0,quality);

    delete pix;
}
