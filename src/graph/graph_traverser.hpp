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

#ifndef GRAPH_TRAVERSER_HPP
#define GRAPH_TRAVERSER_HPP

#include "graph.hpp"
#include "path_builder.hpp"
#include "edge_traversal_proxy.hpp"
#include "traversal_info.hpp"

class Graph_Traverser
{
    typedef QList<Edge_Traversal_Proxy*> edge_list;
    typedef edge_list::iterator         iterator;
    typedef QHash<Edge*,Edge_Traversal_Proxy*> edge_map;

    edge_list edges;
    edge_list traversed_edges;
    edge_map  proxy;

public:

    ~Graph_Traverser();
    void add_edge(Edge*);
    void remove_edge(Edge*);

    void traverse(Path_Builder& path, const Node_Style* default_node_style );

private:

    /** Mark source and destionation handles as traversed,
     * get proper vertices and render
     */
    Traversal_Info traverse(Edge_Traversal_Proxy* edge,
                            Edge_Handle handle,
                            Path_Builder& path,
                            const Node_Style* default_node_style );

};

#endif // GRAPH_TRAVERSER_HPP
