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

#ifndef EDGE_TRAVERSAL_PROXY_HPP
#define EDGE_TRAVERSAL_PROXY_HPP
#include "edge.hpp"

class Edge_Traversal_Proxy
{
public:


    Edge*        edge;
    Handle_Flags available_handles;

    Edge_Traversal_Proxy(Edge* edge)
        : edge(edge), available_handles(TOP_LEFT|TOP_RIGHT|BOTTOM_LEFT|BOTTOM_RIGHT)
    {}

    /// Mark all handles as untraversed
    void reset()
    {
        available_handles |= Handle_Flags(TOP_LEFT)|TOP_RIGHT|BOTTOM_LEFT|BOTTOM_RIGHT;
    }

    void mark_traversed(Edge_Handle h)
    {
        available_handles &=~ h;
    }

    /// Check if handle has been traversed
    bool traversed(Edge_Handle handle) const
    {
        return available_handles & handle;
    }

    /// Get a 'random' non-traversed handle
    Edge_Handle not_traversed() const
    {
        if ( available_handles & TOP_LEFT )
            return TOP_LEFT;
        else if ( available_handles & TOP_RIGHT )
            return TOP_RIGHT;
        else if ( available_handles & BOTTOM_LEFT )
            return BOTTOM_LEFT;
        else if ( available_handles & BOTTOM_RIGHT )
            return BOTTOM_RIGHT;
        else
            return NO_HANDLE;
    }

    Edge* operator->() { return edge; }

    /// Get node in the direction of the handle
    Node* vertex_for(Edge_Handle handle) const
    {
        return ( handle == TOP_RIGHT || handle == BOTTOM_RIGHT ) ?
            edge->vertex2() : edge->vertex1();
    }
};

#endif // EDGE_TRAVERSAL_PROXY_HPP
