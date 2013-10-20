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

#ifndef EDGE_SCRIPTED_HPP
#define EDGE_SCRIPTED_HPP

#include "edge_type.hpp"
#include "plugin_crossing.hpp"

class Edge_Scripted : public Edge_Type
{
private:
    Plugin_Crossing *plugin;

public:
    Edge_Scripted(Plugin_Crossing *plugin);

    /**
     *  \brief Perform any rendering to path and return the next handle
    */
    Edge::Handle traverse(Edge* edge, Edge::Handle handle,Path_Builder& path) const override;
    /**
     *  \brief Get handle geometry
     *
     *  p1 is the point where the path line should pass, p2 the control point
     */
    QLineF handle(const Edge *edge, Edge::Handle handle) const override;

    void paint(QPainter*painter, const Edge& edge) override;
    QString name() const override;
    QString machine_name() const override;
    QIcon icon() const override;
};

#endif // EDGE_SCRIPTED_HPP
