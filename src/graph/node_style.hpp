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

#ifndef NODE_STYLE_HPP
#define NODE_STYLE_HPP

#include <QObject>
#include "path_builder.hpp"
#include "node_cusp_shape.hpp"

class Node_Style
{

public:

    enum Enabled_Styles_Enum
    {
        NOTHING           = 0x00,
        CURVE_STYLE       = 0x01,
        CUSP_ANGLE        = 0x02,
        HANDLE_LENGTH     = 0x04,
        CROSSING_DISTANCE = 0x08,
        CUSP_DISTANCE     = 0x10,
        EVERYTHING        = 0xFF
    };
    Q_DECLARE_FLAGS(Enabled_Styles, Enabled_Styles_Enum)

public:
    /// Which styles to override
    Enabled_Styles enabled_style;
    /// Minimum angle required to enable the cusp
    double cusp_angle;
    /// Length of the handles (line from start point to control point in SVG curves )
    double handle_length;
    /// Length of the gap that highlights the thread passing under another one
    double crossing_distance;
    /// Distance from the node to the cusp point
    double cusp_distance;
    /// Polymorphic object that renders the cusp
    Node_Cusp_Shape* cusp_shape;

public:
    Node_Style (
               double cusp_angle = 225,
               double handle_length = 24,
               double crossing_distance = 10,
               double cusp_distance = 32,
               Node_Cusp_Shape* cusp_shape = nullptr,
               Enabled_Styles enabled_style = NOTHING )
        :   enabled_style ( enabled_style ),
            cusp_angle(cusp_angle),
            handle_length(handle_length),
            crossing_distance(crossing_distance),
            cusp_distance ( cusp_distance ),
            cusp_shape ( cusp_shape )
    {}

    /// Set disabled style to the values in other
    Node_Style default_to(const Node_Style& other) const;

    void build(const Traversal_Info& ti,Path_Builder&path, const Node_Style& default_style) const
    {
        Node_Style def = default_to(default_style);
        if ( def.cusp_shape )
            def.cusp_shape->draw_joint(path,ti,def);
    }

};

#endif // NODE_STYLE_HPP
