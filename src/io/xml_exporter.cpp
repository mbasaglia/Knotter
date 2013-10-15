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

#include "xml_exporter.hpp"
#include "resource_manager.hpp"
#include <QMetaEnum>
#include <QBuffer>
#include "image_exporter.hpp"

XML_Exporter::XML_Exporter(QIODevice *output,bool pretty_xml)
    : xml ( output )
{
    if ( pretty_xml )
    {
        xml.setAutoFormatting(true);
        xml.setAutoFormattingIndent(4);
    }
}

void XML_Exporter::export_graph(const Graph * graph)
{
    begin();

    save_style(graph);

    start_element("graph");

    start_element("nodes");
    foreach(Node* node, graph->nodes())
        save_node(node);
    end_element(); // nodes

    start_element("edges");
    foreach(Edge* edge, graph->edges())
        save_edge(edge);
    end_element(); // edges

    end();
}

void XML_Exporter::begin()
{
    xml.writeStartDocument("1.0");
    xml.writeStartElement("knot");
    xml.writeAttribute("version",QString::number(version));
    xml.writeAttribute("generator",QString("%1 %2")
                       .arg(Resource_Manager::program_name())
                       .arg(Resource_Manager::program_version())
                      );
}

void XML_Exporter::end()
{
    xml.writeEndDocument();
}

void XML_Exporter::start_element(QString name)
{
    xml.writeStartElement ( name );
}

void XML_Exporter::end_element()
{
    xml.writeEndElement();
}

void XML_Exporter::save_style(const Graph *graph)
{
    start_element("style");

    start_element("colors");
    foreach(QColor c, graph->colors())
        save_color("color",c);
    end_element(); // colors

    start_element("borders");
    foreach(Knot_Border b, graph->borders())
    {
        start_element("border");
        xml.writeAttribute("width",QString::number(b.width));
        xml.writeAttribute("alpha",QString::number(b.color.alpha()));
        xml.writeCharacters(b.color.name());
        end_element(); // border

    }
    end_element(); // borders

    save_cusp("cusp",graph->default_node_style());
    save_crossing("crossing",graph->default_edge_style());

    start_element("stroke");

    xml.writeTextElement("width",QString::number(graph->width()));

    const QMetaObject& mo = staticQtMetaObject;

    QMetaEnum bs_me = mo.enumerator(mo.indexOfEnumerator("BrushStyle"));
    xml.writeTextElement("style",bs_me.valueToKey(graph->brush_style()));


    QMetaEnum pjs_me = mo.enumerator(mo.indexOfEnumerator("PenJoinStyle"));
    xml.writeTextElement("join",pjs_me.valueToKey(graph->join_style()));

    end_element(); // stroke



    end_element(); // style
}

void XML_Exporter::save_cusp(QString name, Node_Style style)
{

    start_element(name);
    if ( style.enabled_style & Node_Style::CUSP_SHAPE )
        xml.writeTextElement("shape",style.cusp_shape->machine_name());

    if ( style.enabled_style & Node_Style::CUSP_ANGLE )
        xml.writeTextElement("min-angle",QString::number(style.cusp_angle));

    if ( style.enabled_style & Node_Style::CUSP_DISTANCE )
        xml.writeTextElement("distance",QString::number(style.cusp_distance));

    if ( style.enabled_style & Node_Style::HANDLE_LENGTH )
        xml.writeTextElement("handle-length",QString::number(style.handle_length));

    end_element();
}


void XML_Exporter::save_crossing(QString name, Edge_Style style)
{
    start_element(name);

    if ( style.enabled_style & Edge_Style::CROSSING_DISTANCE )
        xml.writeTextElement("gap",QString::number(style.crossing_distance));

    if ( style.enabled_style & Edge_Style::EDGE_SLIDE )
        xml.writeTextElement("slide",QString::number(style.edge_slide));

    if ( style.enabled_style & Edge_Style::HANDLE_LENGTH )
        xml.writeTextElement("handle-length",QString::number(style.handle_length));

    end_element();
}

void XML_Exporter::save_node(Node *node)
{
    int id = node_id(node);


    start_element("node");
        xml.writeAttribute("id",QString("node_%1").arg(id));
        xml.writeAttribute("x",QString::number(node->pos().x()));
        xml.writeAttribute("y",QString::number(node->pos().y()));
        if ( node->style().enabled_style != Node_Style::NOTHING )
            save_cusp("style",node->style());
    end_element(); // node

}

void XML_Exporter::save_edge(Edge *edge)
{
    start_element("edge");
    xml.writeAttribute("type", edge->style().edge_type->machine_name());
    xml.writeAttribute("v1",QString("node_%1").arg(node_id(edge->vertex1())));
    xml.writeAttribute("v2",QString("node_%1").arg(node_id(edge->vertex2())));
    if ( edge->style().enabled_style != Node_Style::NOTHING )
        save_crossing("style",edge->style());
    end_element(); // edge
}


void XML_Exporter::save_color(QString name, QColor col)
{
    start_element(name);
    xml.writeAttribute("alpha",QString::number(col.alpha()));
    xml.writeCharacters(col.name());
    end_element();
}

int XML_Exporter::node_id(Node *node)
{
    if ( node_ids.contains(node) )
        return node_ids[node];
    else
        return node_ids[node] = node_ids.size();
}


bool export_xml(const Graph& graph, QIODevice &file )
{
    if ( !file.isWritable() && !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return false;

    XML_Exporter(&file).export_graph(&graph);

    return true;
}

void export_xml_mime_data(QMimeData* data, const Graph& graph)
{

    QByteArray knot_xml;
    QBuffer xml_stream(&knot_xml);
    export_xml(graph,xml_stream);

    data->setData("application/x-knotter",knot_xml);


    if ( Resource_Manager::settings.clipboard_feature(Settings::XML) )
        data->setData("text/xml",knot_xml);

    if ( Resource_Manager::settings.clipboard_feature(Settings::SVG) )
    {
        QByteArray knot_svg;
        QBuffer svg_stream(&knot_svg);
        export_svg(svg_stream,graph,false);
        data->setData("image/svg+xml",knot_svg);
    }

    if ( Resource_Manager::settings.clipboard_feature(Settings::PNG) )
    {
        QByteArray knot_png;
        QBuffer png_stream(&knot_png);
        export_raster(png_stream,graph,Qt::transparent,true,
                      graph.full_image_bounding_rect().size().toSize(),100,false,
                      false,Background_Image(),"PNG");
        data->setData("image/png",knot_png);
    }


    if ( Resource_Manager::settings.clipboard_feature(Settings::TIFF) )
    {
        QByteArray knot_tiff;
        QBuffer tiff_stream(&knot_tiff);
        export_raster(tiff_stream,graph,Qt::transparent,true,
                      graph.full_image_bounding_rect().size().toSize(),100,false,
                      false,Background_Image(),"TIFF");
        data->setData("image/tiff",knot_tiff);
    }
}


QByteArray export_xml_style(const Graph& graph)
{
    QByteArray output;
    QBuffer buffer(&output);
    buffer.open(QIODevice::WriteOnly);
    XML_Exporter(&buffer,false).save_style(&graph);
    return output;
}
