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

#ifndef TRAVERSAL_INFO_HPP
#define TRAVERSAL_INFO_HPP

#include "c++.hpp"
#include "edge.hpp"


/**
    \brief Information of the way an edge has be traversed
*/
struct Traversal_Info
{
    enum handside_type { LEFT, RIGHT };

    struct Source
    {
        Edge* edge;
        Edge::Handle handle;
        double angle;

        Source()
            : edge(nullptr), handle(Edge::NO_HANDLE), angle(0)
        {}
    };

    Source in;
    Source out;
    Node* node;
    handside_type handside;         ///< Side of the input handle relative to the input edge
    double angle_delta;             ///< Difference between angle_in and angle_out (clock or counter depends on handside)
    bool success;                   ///< Input parameters were OK


    Traversal_Info()
        :   node(nullptr), handside ( LEFT ), angle_delta(0),
            success(false)
    {}


};
#endif // TRAVERSAL_INFO_HPP
