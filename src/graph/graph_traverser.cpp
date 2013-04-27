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

#include "graph_traverser.hpp"

void Graph_Traverser::traverse(Path_Builder &path,
                               const Node_Style* default_node_style )
{
    for(iterator i = edges.begin(); i != edges.end(); ++i)
        (*i)->reset();

    // cycle while there are edges with untraversed handles
    while(!edges.empty())
    {
        // pick first edge/handle
        Edge_Traversal_Proxy* edge = edges.front();
        Edge_Handle handle = edge->not_traversed();

        if ( handle == NO_HANDLE )
        {
            // removed completed edge
            traversed_edges.push_back(edge);
            edges.pop_front();
            continue;
        }

        // begin new knot thread
        path.new_group();

        // loop around a knotline loop item
        while ( ! edge->traversed(handle) )
        {
            Traversal_Info ti = traverse(edge,handle,path,default_node_style);

            edge = proxy[ti.out.edge];
            // Don't mark handle as traversed but render and get next handle
            handle = edge->edge->style()->traverse(edge->edge,ti.out.handle,path,default_node_style);

        }
    }

    edges.swap(traversed_edges);
}

Traversal_Info Graph_Traverser::traverse(Edge_Traversal_Proxy *edge,
                               Edge_Handle handle,
                               Path_Builder &path,
                               const Node_Style *default_node_style)
{
    // set input values
    Traversal_Info ti;
    ti.in.edge = edge->edge;
    ti.in.handle = handle;
    ti.node = edge->vertex_for(handle);
    ti.in.angle = QLineF ( ti.node->pos(), ti.in.edge->other(ti.node)->pos() ).angle();
    edge->mark_traversed(handle);


    // select handside (as viewed from the edge)
    if ( ti.in.edge->vertex1() == ti.node )
    {
        // RH = TL,  LH = BL
        if ( handle == TOP_LEFT )
            ti.handside = Traversal_Info::RIGHT;
        else if ( handle == BOTTOM_LEFT )
            ti.handside = Traversal_Info::LEFT;
        else
            return Traversal_Info(); // Wrong handle
    }
    else if ( ti.in.edge->vertex2() == ti.node )
    {
        // RH = BR,  LH = TR
        if ( handle == BOTTOM_RIGHT )
            ti.handside = Traversal_Info::RIGHT;
        else if ( handle == TOP_RIGHT )
            ti.handside = Traversal_Info::LEFT;
        else
            return Traversal_Info(); // Wrong handle
    }
    else
        return Traversal_Info(); // Wrong edge


    ti.out.angle = ti.in.angle;
    ti.angle_delta = 360;
    ti.out.edge = ti.in.edge;
    // select the next edge as the one with the smallest angle difference
    // angle direction is based on handside
    foreach(Edge* i, ti.node->connections() )
    {
        if ( i != ti.in.edge )
        {
            double angle_out = QLineF ( ti.node->pos(),i->other(ti.node)->pos() ).angle();
            double delta = ti.in.angle - angle_out;
            if ( delta < 0 )
                delta += 360;
            if ( ti.handside == Traversal_Info::RIGHT )
                delta = 360-delta;
            if ( delta < ti.angle_delta )
            {
                ti.angle_delta = delta;
                ti.out.edge = i;
                ti.out.angle = angle_out;
            }
        }
    }


    // output values

    // select output handles based on handside
    if ( ti.out.edge->vertex1() == ti.node )
    {
        // RH -> BL,  LH -> TL
        ti.out.handle = ti.handside == Traversal_Info::RIGHT ?
                    BOTTOM_LEFT :
                    TOP_LEFT;
    }
    else if ( ti.out.edge->vertex2() == ti.node )
    {
        // RH -> TR,  LH -> BR
        ti.out.handle = ti.handside == Traversal_Info::RIGHT ?
                    TOP_RIGHT :
                    BOTTOM_RIGHT;
    }

    ti.success = true;

    ti.node->style().build(ti.node->pos(),path,default_node_style);

    return ti;
}


void Graph_Traverser::add_edge(Edge *edge)
{
    edges.push_back(new Edge_Traversal_Proxy(edge));
    proxy[edge] = edges.back();
}

void Graph_Traverser::remove_edge(Edge *edge)
{
    edge_map::iterator it = proxy.find(edge);
    edges.removeOne(it.value());
    delete it.value();
    proxy.erase(it);
}
