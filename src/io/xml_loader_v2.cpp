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

#include "xml_loader_v2.hpp"
#include "resource_manager.hpp"
#include "edge_style.hpp"

bool XML_Loader_v2::load(QIODevice *input)
{

    QDomDocument xml;
    if ( !xml.setContent(input) ) // could retrieve error details
        return false; // xml error

    QDomElement knot = xml.firstChildElement("knot");
    if ( knot.isNull() )
        return false; // XML does not contain a knot
    version = knot.attribute("version").toInt();
    if ( version > supported_version )
        return false; // unknown version

    if ( knot.firstChildElement("graph").isNull() )
        return false; // XML does not contain a graph description

    current_elements.push( knot );
    return true;
}

void XML_Loader_v2::get_graph(Graph &kv)
{


    if ( enter("style") )
    {
        QPen stroke(Qt::black,kv.width());
        stroke.setJoinStyle(kv.join_style());
        stroke = get_pen("stroke",stroke);
        kv.set_colors(QList<QColor>() << stroke.color());
        kv.set_width(stroke.widthF());
        kv.set_join_style(stroke.joinStyle());

        QPen p = get_pen("outline", Qt::NoPen );
        if ( p.style() != Qt::NoPen && p.color().alpha() > 0 )
        {
            kv.set_borders(Border_List()<<Knot_Border(p.color(),p.widthF()));
        }

        Node_Style ns = get_cusp( "cusp" );
        ns.enabled_style = Node_Style::EVERYTHING;
        if ( !ns.cusp_shape )
            ns.cusp_shape = resource_manager().default_cusp_shape();
        kv.set_default_node_style ( ns );


        QDomElement e_cusp = current_elements.top().firstChildElement("cusp");
        if ( !e_cusp.isNull() )
        {
            QDomElement e_gap = e_cusp.firstChildElement("gap");
            if ( !e_gap.isNull() )
                kv.default_edge_style_reference().crossing_distance =
                    e_gap.text().toDouble();
        }

        leave();
    }

    QDomElement graph = current_elements.top().firstChildElement("graph");
    QDomElement node = graph.firstChildElement("node");
    while ( !node.isNull() )
    {
        QString id = node.attribute("id");
        Node* cur_node = register_node ( id );
        cur_node->setPos(node.attribute("x").toDouble(),node.attribute("y").toDouble());

        if ( !node.firstChildElement("custom-style").isNull() )
        {
            current_elements.push(node);
            cur_node->set_style ( get_cusp ( "custom-style"  ) );
            current_elements.pop();
            /// \note Will lose custom-style/gap as it has been moved to edge style
        }

        kv.add_node(cur_node);

        QDomElement edge = node.firstChildElement("edge");
        while ( !edge.isNull() )
        {
            QString type_name = edge.attribute("type");
            QString target = edge.attribute("target");
            Node* target_node = register_node(target);
            Edge *e = cur_node->edge_to(target_node);
            if ( !e )
            {
                Edge* e = new Edge(cur_node,target_node,
                    resource_manager().edge_type_from_machine_name(type_name));
                kv.add_edge(e);
            }
            edge = edge.nextSiblingElement("edge");
        }
        node = node.nextSiblingElement("node");
    }
}

Node *XML_Loader_v2::register_node(QString id)
{
    if ( !node_ids.contains(id) )
    {
        return node_ids[id] = new Node(QPointF());
    }
    else
        return node_ids[id];
}

bool XML_Loader_v2::enter(QString name)
{
    QDomElement elem = current_elements.top().firstChildElement(name);
    if ( elem.isNull() )
        return false;
    current_elements.push ( elem );
    return true;
}

QPen XML_Loader_v2::get_pen(QString name, QPen pen)
{

    QDomElement pen_elem = current_elements.top().firstChildElement(name);

    QColor pen_col(pen_elem.attribute("color",pen.color().name()));
    pen_col.setAlpha(pen_elem.attribute("alpha",
                            QString::number(pen.color().alpha())
                        ).toInt());
    pen.setColor(pen_col);
    pen.setWidth(pen_elem.attribute("width",QString::number(pen.widthF())).toDouble() );


    QString pstyle_name = pen_elem.attribute("style","solid");
    if ( pstyle_name == "solid" )
        pen.setStyle ( Qt::SolidLine );
    else if ( pstyle_name == "dot" )
        pen.setStyle ( Qt::DotLine );
    else if ( pstyle_name == "dash" )
        pen.setStyle ( Qt::DashLine );
    else if ( pstyle_name == "dash_dot" )
        pen.setStyle ( Qt::DashDotLine );
    else if ( pstyle_name == "dash_dot_dot" )
        pen.setStyle ( Qt::DashDotDotLine );
    else
        pen.setStyle ( Qt::NoPen );

    QDomElement cusp_point = pen_elem.firstChildElement("point");
    if ( !cusp_point.isNull() )
    {
        QString point_name =  cusp_point.text();
        if ( point_name == "bevel" )
            pen.setJoinStyle(Qt::BevelJoin);
        else if ( point_name == "round" )
            pen.setJoinStyle ( Qt::RoundJoin );
        else
            pen.setJoinStyle ( Qt::MiterJoin );
    }

    return pen;
}

Node_Style XML_Loader_v2::get_cusp(QString name)
{

    Node_Style cusp_style_info;
    QDomElement cusp = current_elements.top().firstChildElement(name);

    QDomElement cusp_style = cusp.firstChildElement("style");
    if ( !cusp_style.isNull() )
    {
        cusp_style_info.cusp_shape =
                resource_manager().cusp_shape_from_machine_name(cusp_style.text());
        cusp_style_info.enabled_style |= Node_Style::CUSP_SHAPE;
    }

    QDomElement cusp_angle = cusp.firstChildElement("min-angle");
    if ( ! cusp_angle.isNull() )
    {
        cusp_style_info.cusp_angle =  cusp_angle.text().toDouble();
        cusp_style_info.enabled_style |= Node_Style::CUSP_ANGLE;
    }


    QDomElement cusp_dist = cusp.firstChildElement("distance");
    if ( !  cusp_dist.isNull() )
    {
        cusp_style_info.cusp_distance = cusp_dist.text().toDouble();
        cusp_style_info.enabled_style |= Node_Style::CUSP_DISTANCE;
    }

    QDomElement handle_length = cusp.firstChildElement("handle-length");
    if ( ! handle_length.isNull() )
    {
        cusp_style_info.handle_length =  handle_length.text().toDouble();
        cusp_style_info.enabled_style |= Node_Style::HANDLE_LENGTH;
    }


    return cusp_style_info;
}

void XML_Loader_v2::leave()
{
    current_elements.pop();
}
