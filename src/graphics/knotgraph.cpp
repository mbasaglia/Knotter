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
KnotGraph::KnotGraph()
    : default_style ( knot_curve_styler::idof("pointed"), 225, 32, 10, 24)
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

void KnotGraph::add_node(Node* which)
{
    if ( which )
        nodes.push_back(which);
}

void KnotGraph::add_edge(Edge *which)
{
    if ( which )
        edges.push_back ( which );
}

void KnotGraph::remove_node(Node *which)
{
    nodes.removeAll(which);
}

void KnotGraph::remove_edge(Edge *which)
{
    edges.removeAll(which);
}

void KnotGraph::add(CustomItem *what)
{
    add_node ( dynamic_cast<Node*>(what) );
    add_edge ( dynamic_cast<Edge*>(what) );
}

void KnotGraph::clear()
{
    nodes.clear();
    edges.clear();
}

QPainterPath KnotGraph::build()
{
    knot_curve_style* pcs = knot_curve_styler::style(default_style.curve_style);

    //path_builder path_b;
    QPainterPath path;

    foreach(Edge*e,edges)
    {
        e->reset();
        QList<QLineF> connected = e->connected(default_style.handle_length,
                                                default_style.crossing_distance);
        foreach ( QLineF l, connected )
        {
            path.moveTo(l.p1());
            path.lineTo(l.p2());
            //path_b.add_line(l.p1(),l.p2());
        }
    }

    while ( !edges.empty() )
    {
        Edge* e = edges.front();
        Edge::handle_type handle = e->not_traversed();
        if ( handle == Edge::NOHANDLE )
        {
            traversed_edges.push_back(e);
            edges.pop_front();
        }
        else
        {
            if ( ! e->traversed(handle) )
            {
                e->traverse(handle);

                Node* n = e->vertex1();
                if ( handle == Edge::TOPRIGHT || handle == Edge::BOTTOMRIGHT )
                    n = e->vertex2();


                TraversalInfo ti = n->next_edge(e,handle);

                if ( n->has_custom_style() )
                {
                    const styleinfo& csi = n->get_custom_style();
                    knot_curve_style* customcs = knot_curve_styler::style(csi.curve_style);
                    customcs->draw_joint(path,n,ti,csi);
                }
                else
                {
                    pcs->draw_joint(path,n,ti,default_style);
                }

                ti.edge_out->traverse(ti.handle_out);
            }
        }
    }

    //path_b.sort();
    //QPainterPath path = path_b.build();


    edges = traversed_edges;
    traversed_edges.clear();

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

void KnotGraph::set_style_info(styleinfo new_style)
{
    default_style = new_style;
}

styleinfo KnotGraph::get_style_info() const
{
    return default_style;
}
/*
knot_curve_styler::style_id KnotGraph::get_curve_style() const
{
    return curve_style;
}

void KnotGraph::set_curve_style(knot_curve_styler::style_id cs )
{
    curve_style = cs;
}

double KnotGraph::get_cusp_angle() const
{
    return cusp_angle;
}

void KnotGraph::set_cusp_angle(double ca)
{
    cusp_angle = ca;
}

double KnotGraph::get_handle_length() const
{
    return handle_length;
}

void KnotGraph::set_handle_length(double hl)
{
    handle_length = hl;
}

double KnotGraph::get_crossing_distance() const
{
    return crossing_distance;
}

void KnotGraph::set_crossing_distance(double cd)
{
    crossing_distance = cd;
}*/

Qt::PenJoinStyle KnotGraph::get_join_style() const
{
    return stroker.joinStyle();
}

void KnotGraph::set_join_style(Qt::PenJoinStyle pjs)
{
    stroker.setJoinStyle(pjs);
}


