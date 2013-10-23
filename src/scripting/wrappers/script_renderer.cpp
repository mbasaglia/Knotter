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

#include "script_renderer.hpp"
#include "image_exporter.hpp"
#include "xml_exporter.hpp"
#include <QBuffer>


QString Script_Renderer::knot()
{
    QByteArray out_raw;
    QBuffer out(&out_raw);
    out.open(QIODevice::WriteOnly|QIODevice::Text);
    export_xml(*graph,out);
    return out_raw;
}

QString Script_Renderer::svg()
{
    QByteArray out_raw;
    QBuffer out(&out_raw);
    out.open(QIODevice::WriteOnly|QIODevice::Text);
    export_svg(out,*graph,m_draw_graph);
    return out_raw;
}

QByteArray Script_Renderer::raster(int width, int height, QString format, int quality, Script_Color background)
{
    QSizeF actual_size = graph->full_image_bounding_rect().size();
    if ( width <= 0 && height <= 0 )
    {
        width = actual_size.width();
        height = actual_size.height();
    }
    else if ( width <= 0 )
    {
        width = actual_size.width()*height/actual_size.height();
    }
    else if ( height <= 0 )
    {
        height = actual_size.height()*width/actual_size.width();
    }

    QByteArray out_raw;
    QBuffer out(&out_raw);
    out.open(QIODevice::WriteOnly);
    export_raster(out,*graph,background,true,QSize(width,height),
                  quality,m_draw_graph,false,Background_Image(),format.toStdString().c_str());
    return out_raw;
}

QString Script_Renderer::toString()
{
    return "[renderer]";
}
