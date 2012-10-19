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
#include "basic_knot_graph.hpp"

basic_knot_graph::basic_knot_graph()
    : default_style ( knot_curve_styler::idof("pointed"), 225, 24, 10, 32)
{
}

void basic_knot_graph::add_node(Node *which)
{
    if ( which && !nodes.contains(which) )
        nodes.push_back(which);
}

void basic_knot_graph::add_edge(Edge *which)
{
    if ( which && !edges.contains(which) )
        edges.push_back(which);
}

Edge *basic_knot_graph::add_edge(Node *a, Node *b)
{
    if ( !a || !b )
        return NULL;

    Edge* already_there = a->get_link ( b );
    if ( !already_there )
    {
        Edge *brand_new = new Edge(a,b);
        a->add_link(brand_new);
        b->add_link(brand_new);
        add_edge(brand_new);
        return brand_new;
    }
    else if ( !edges.contains(already_there) )
    {
        add_edge(already_there);
    }

    return already_there;
}

void basic_knot_graph::set_edge_type(Node *a, Node *b, Edge::type_type type)
{
    Edge* edge = a->get_link ( b );
    if ( edge )
        edge->type = type;
}

void basic_knot_graph::remove_node(Node *which)
{
    nodes.removeAll(which);
}

void basic_knot_graph::remove_edge(Edge *which)
{
    edges.removeAll(which);
}

void basic_knot_graph::clear()
{
    nodes.clear();
    edges.clear();
}

void basic_knot_graph::set_style_info(styleinfo new_style)
{
    default_style = new_style.default_to(default_style);
}

styleinfo basic_knot_graph::get_style_info() const
{
    return default_style;
}

void basic_knot_graph::build_knotline(path_builder &path_b)
{
    knot_curve_style* pcs = knot_curve_styler::style(default_style.curve_style);

    foreach(Edge*e,edges)
    {
        e->reset();
        QList<QLineF> connected = e->connected(default_style.handle_length,
                                                default_style.crossing_distance);
        foreach ( QLineF l, connected )
        {
            path_b.add_line(l.p1(),l.p2());
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


                TraversalInfo ti = next_edge(n,e,handle);

                if ( n->has_custom_style() )
                {
                    styleinfo csi = n->get_custom_style().default_to(default_style);
                    knot_curve_style* customcs = knot_curve_styler::style(csi.curve_style);
                    customcs->draw_joint(path_b,n,ti,csi);
                }
                else
                {
                    pcs->draw_joint(path_b,n,ti,default_style);
                }

                ti.edge_out->traverse(ti.handle_out);
            }
        }
    }


    edges = traversed_edges;
    traversed_edges.clear();
}






TraversalInfo next_edge(Node* node, Edge *edge, Edge::handle_type handle)
{

    TraversalInfo ti;
    ti.edge_in = edge;
    ti.handle_in = handle;

    if ( edge->vertex1() == node )
    {
        // RH = TL,  LH = BL
        if ( handle == Edge::TOPLEFT )
            ti.handside = TraversalInfo::RIGHT;
        else if ( handle == Edge::BOTTOMLEFT )
            ti.handside = TraversalInfo::LEFT;
        else
            return TraversalInfo("Wrong handle");
    }
    else if ( edge->vertex2() == node )
    {
        // RH = BR,  LH = TR
        if ( handle == Edge::BOTTOMRIGHT )
            ti.handside = TraversalInfo::RIGHT;
        else if ( handle == Edge::TOPRIGHT )
            ti.handside = TraversalInfo::LEFT;
        else
            return TraversalInfo("Wrong handle");
    }
    else
        return TraversalInfo("Wrong edge");

    ti.angle_in =  QLineF ( node->pos(), edge->other(node)->pos() ).angle();

    ti.angle_out = ti.angle_in;
    ti.angle_delta = 360;
    ti.edge_out = edge;
    foreach(Edge* i, node->links())
    {
        if ( i != edge )
        {
            double angle_out = QLineF ( node->pos(),i->other(node)->pos() ).angle();
            double delta = ti.angle_in - angle_out;
            if ( delta < 0 )
                delta += 360;
            if ( ti.handside == TraversalInfo::RIGHT )
                delta = 360-delta;
            if ( delta < ti.angle_delta )
            {
                ti.angle_delta = delta;
                ti.edge_out = i;
                ti.angle_out = angle_out;
            }
        }
    }


    if ( ti.edge_out->vertex1() == node )
    {
        // RH -> BL,  LH -> TL
        ti.handle_out = ti.handside == TraversalInfo::RIGHT ? Edge::BOTTOMLEFT : Edge::TOPLEFT;
    }
    else if ( ti.edge_out->vertex2() == node )
    {
        // RH -> TR,  LH -> BR
        ti.handle_out = ti.handside == TraversalInfo::RIGHT ? Edge::TOPRIGHT : Edge::BOTTOMRIGHT;
    }
    ti.success = true;
    return ti;
}
