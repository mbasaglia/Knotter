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
#include "knotgraph.hpp"
#include <QBrush>
#include <QPen>
#include "path_builder.hpp"
#include "xml_saver.hpp"

KnotGraph::KnotGraph()
{
    setBrush(QBrush(Qt::black));
    setPen(QPen(Qt::gray));
    stroker.setWidth(5);
    stroker.setCapStyle(Qt::FlatCap);

    /// \todo configure miter limit
    stroker.setJoinStyle(Qt::MiterJoin);

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    //setCacheMode(QGraphicsItem::ItemCoordinateCache);

}

KnotGraph::~KnotGraph()
{
}

void KnotGraph::add(CustomItem *what)
{
    add_node ( dynamic_cast<Node*>(what) );
    add_edge ( dynamic_cast<Edge*>(what) );
}

QPainterPath KnotGraph::build()
{

    path_builder path_b;
    build_knotline(path_b);

    QPainterPath path = path_b.build();

    /// \bug Qt? if not simplified weird artifacts on cusps
    QPainterPath styled = stroker.createStroke(path).simplified();
    styled.setFillRule(Qt::WindingFill);
    setPath(styled);

    return path;
}

void KnotGraph::set_width(double w)
{
    stroker.setWidth(w);
}

double KnotGraph::get_width() const
{
    return stroker.width();
}

Qt::PenJoinStyle KnotGraph::get_join_style() const
{
    return stroker.joinStyle();
}

void KnotGraph::set_join_style(Qt::PenJoinStyle pjs)
{
    stroker.setJoinStyle(pjs);
}

void KnotGraph::load_xml(xml_loader &xml)
{

    if ( xml.enter("style") )
    {
        QPen stroke(brush(),get_width());
        stroke.setJoinStyle(get_join_style());
        stroke = xml.get_pen("stroke",stroke);
        setBrush(stroke.brush());
        set_width(stroke.widthF());
        set_join_style(stroke.joinStyle());

        setPen ( xml.get_pen("outline", pen() ) );

        set_style_info ( xml.get_cusp( "cusp", get_style_info() ) );

        xml.leave();
    }


    xml.get_graph(*this);
}

void KnotGraph::save_xml(xml_saver &xml) const
{
    xml.start_element("style");
        QPen stroke(brush(),get_width());
        stroke.setJoinStyle(get_join_style());
        xml.save_pen ( "stroke", stroke, false, true );
        xml.save_pen ( "outline", pen(), true, false );
        xml.save_cusp ( "cusp", get_style_info() );
    xml.end_element();

    xml.save_graph("graph",nodes);
}


