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

#include "xml_loader.hpp"
#include "xml_loader_v2.hpp"
#include <QMetaEnum>
#include "resource_manager.hpp"
#include <QBuffer>

bool import_xml(QIODevice &file, Graph& graph)
{
    if ( !file.isOpen() && !file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        return false;
    }

    XML_Loader xml;

    if ( xml.load(&file,&graph) )
        return true;
    else if ( xml.version() > 0 && xml.version() < 3 )
    {
        file.seek(0);
        XML_Loader_v2 xml2;

        if ( !xml2.load(&file) )
            return false;

        xml2.get_graph(graph);
        return true;
    }

    return false;
}


bool XML_Loader::load(QIODevice *input, Graph* graph)
{
    m_version = 0;
    if ( !xml.setContent(input) ) // could retrieve error details
        return false; // xml error

    QDomElement knot = xml.firstChildElement("knot");
    if ( knot.isNull() )
        return false; // XML does not contain a knot
    m_version = knot.attribute("version").toInt();
    if ( m_version  > max_version || m_version < min_version )
        return false; // unknown version

    if ( knot.firstChildElement("graph").isNull() )
        return false; // XML does not contain a graph description

    get_style(knot.firstChildElement("style"),graph);
    get_graph(knot.firstChildElement("graph"),graph);

    return true;
}

void XML_Loader::load_style(QIODevice *input, Graph *graph)
{
    if ( !xml.setContent(input) )
        return;
    get_style(xml.firstChildElement(),graph);
}

int XML_Loader::version()
{
    return m_version;
}


Node *XML_Loader::get_node(QDomElement element)
{
    QString id = element.attribute("id");
    if ( id.isEmpty() || nodes.contains(id) )
        return nullptr;
    Node* n = new Node(QPointF(element.attribute("x","0").toDouble(),
                       element.attribute("y","0").toDouble()));
    n->set_style(get_node_style(element.firstChildElement("style"),false));
    nodes[id] = n;
    return n;
}

Edge *XML_Loader::get_edge(QDomElement element)
{
    QString id1 = element.attribute("v1");
    QString id2 = element.attribute("v2");
    if ( !nodes.contains(id1) || !nodes.contains(id2) )
        return nullptr;

    Edge* e = new Edge(nodes[id1],nodes[id2]);

    QString type = element.attribute("type");

    // v3 compatibility
    if  ( !element.hasAttribute("type") && element.hasAttribute("style") )
        type = element.attribute("style");


    Edge_Style es = get_edge_style(element.firstChildElement("style"),false);
    es.enabled_style |= Edge_Style::EDGE_TYPE;
    es.edge_type = Resource_Manager::edge_type_from_machine_name(type);
    e->set_style(es);

    return e;
}

Node_Style XML_Loader::get_node_style(QDomElement element, bool everything)
{
    Node_Style ns;
    if ( everything )
    {
        ns.enabled_style = Node_Style::EVERYTHING;
        ns.cusp_shape = Resource_Manager::default_cusp_shape();
    }

    if ( !element.isNull() )
    {
        QDomElement e_style = element.firstChildElement("shape");
        if ( !e_style.isNull() )
        {
            ns.enabled_style |= Node_Style::CUSP_SHAPE;
            ns.cusp_shape = Resource_Manager::cusp_shape_from_machine_name(e_style.text());
        }

        QDomElement e_min_angle = element.firstChildElement("min-angle");
        if ( !e_min_angle.isNull() )
        {
            ns.enabled_style |= Node_Style::CUSP_ANGLE;
            ns.cusp_angle = e_min_angle.text().toDouble();
        }

        QDomElement e_distance = element.firstChildElement("distance");
        if ( !e_distance.isNull() )
        {
            ns.enabled_style |= Node_Style::CUSP_DISTANCE;
            ns.cusp_distance = e_distance.text().toDouble();
        }

        QDomElement e_handle_length = element.firstChildElement("handle-length");
        if ( !e_handle_length.isNull() )
        {
            ns.enabled_style |= Node_Style::HANDLE_LENGTH;
            ns.handle_length = e_handle_length.text().toDouble();
        }

    }

    return ns;
}

Edge_Style XML_Loader::get_edge_style(QDomElement element, bool everything)
{
    Edge_Style es;

    if ( everything )
    {
        es.enabled_style = Edge_Style::EVERYTHING;
        es.edge_type = Resource_Manager::default_edge_type();
    }

    if ( !element.isNull() )
    {
        QDomElement e_gap = element.firstChildElement("gap");
        if ( !e_gap.isNull() )
        {
            es.enabled_style |= Edge_Style::CROSSING_DISTANCE;
            es.crossing_distance = e_gap.text().toDouble();
        }


        QDomElement e_slide = element.firstChildElement("slide");
        if ( !e_slide.isNull() )
        {
            es.enabled_style |= Edge_Style::EDGE_SLIDE;
            es.edge_slide = e_slide.text().toDouble();
        }

        QDomElement e_handle_length = element.firstChildElement("handle-length");
        if ( !e_handle_length.isNull() )
        {
            es.enabled_style |= Edge_Style::HANDLE_LENGTH;
            es.handle_length = e_handle_length.text().toDouble();
        }
    }

    return es;
}


void XML_Loader::get_style(QDomElement e_style, Graph *graph)
{
    if ( e_style.isNull() )
        return;

    QDomElement e_color = e_style.firstChildElement("colors").firstChildElement("color");
    QList<QColor> colors;

    while ( !e_color.isNull() )
    {
        colors.push_back(get_color(e_color));
        e_color = e_color.nextSiblingElement("color");
    }
    if ( colors.isEmpty() )
        colors.push_back(Qt::black);
    graph->set_colors(colors);

    Border_List borders;
    QDomElement e_border = e_style.firstChildElement("borders").firstChildElement("border");
    while ( !e_border.isNull() )
    {
        borders.push_back(Knot_Border(get_color(e_border),
                                e_border.attribute("width","1").toDouble()));
        e_border = e_border.nextSiblingElement("border");
    }
    graph->set_borders(borders);

    QDomElement e_cusp = e_style.firstChildElement("cusp");
    if ( !e_cusp.isNull() )
        graph->set_default_node_style(get_node_style(e_cusp,true));

    QDomElement e_crossing = e_style.firstChildElement("crossing");
    if ( !e_crossing.isNull() )
        graph->set_default_edge_style(get_edge_style(e_crossing,true));
    else
    {
        // v3 compatibility
        QDomElement e_gap = e_cusp.firstChildElement("gap");
        if ( !e_gap.isNull() )
            graph->default_edge_style_reference().crossing_distance =
                e_gap.text().toDouble();
    }


    QDomElement e_stroke = e_style.firstChildElement("stroke");
    if ( !e_stroke.isNull() )
    {
        graph->set_width(e_stroke.firstChildElement("width").text().toDouble());

        const QMetaObject& mo = staticQtMetaObject;
        QString style_name = e_stroke.firstChildElement("style").text();
        QMetaEnum bs_me = mo.enumerator(mo.indexOfEnumerator("BrushStyle"));
        graph->set_brush_style(Qt::BrushStyle(bs_me.keyToValue(
                                style_name.trimmed().toStdString().c_str())));


        style_name = e_stroke.firstChildElement("join").text();
        QMetaEnum pjs_me = mo.enumerator(mo.indexOfEnumerator("PenJoinStyle"));
        graph->set_join_style(Qt::PenJoinStyle(pjs_me.keyToValue(
                                style_name.trimmed().toStdString().c_str())));
    }
}

void XML_Loader::get_graph(QDomElement element, Graph *graph)
{
    QDomElement e_nodes = element.firstChildElement("nodes");
    if ( e_nodes.isNull() )
        return;

    QDomElement e_node = e_nodes.firstChildElement("node");
    while(!e_node.isNull())
    {
        Node* n = get_node(e_node);
        if ( n )
            graph->add_node(n);
        e_node = e_node.nextSiblingElement("node");
    }



    QDomElement e_edge = element.firstChildElement("edges")
                            .firstChildElement("edge");
    while(!e_edge.isNull())
    {
        Edge* e = get_edge(e_edge);
        if ( e )
            graph->add_edge(e);
        e_edge = e_edge.nextSiblingElement("edge");
    }
}


QColor XML_Loader::get_color(QDomElement e)
{
    QColor c = Qt::black;
    c.setNamedColor(e.text().trimmed());
    c.setAlpha(e.attribute("alpha","0").trimmed().toInt());
    return c;
}


void import_xml_style(QString style, Graph& graph)
{
    QByteArray output(style.toUtf8());
    QBuffer buffer(&output);
    buffer.open(QIODevice::ReadOnly);
    XML_Loader().load_style(&buffer,&graph);
}
