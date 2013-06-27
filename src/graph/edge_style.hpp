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

#ifndef EDGE_STYLE_HPP
#define EDGE_STYLE_HPP

#include <QMetaType>
#include "c++.hpp"

class Edge_Type;

class Edge_Style
{

public:

    enum Enabled_Styles_Enum
    {
        NOTHING           = 0x00,
        HANDLE_LENGTH     = 0x01,
        CROSSING_DISTANCE = 0x02,
        EDGE_SLIDE        = 0x04,
        EDGE_TYPE         = 0x08,
        EVERYTHING        = 0xFF
    };
    Q_DECLARE_FLAGS(Enabled_Styles, Enabled_Styles_Enum)

public:
    /// Which styles to override
    Enabled_Styles enabled_style;
    /// Length of the handles (line from start point to control point in SVG curves )
    double handle_length;
    /// Length of the gap that highlights the thread passing under another one
    double crossing_distance;
    /// Value in [0,1] that determines the offset of the crossing
    double edge_slide;
    /// Polymorphic object that renders the crossing
    Edge_Type* edge_type;

public:
    Edge_Style (
               double handle_length = 24,
               double crossing_distance = 10,
               double edge_slide = 0.5,
               Edge_Type* edge_type = nullptr,
               Enabled_Styles enabled_style = NOTHING )
        :   enabled_style ( enabled_style ),
            handle_length(handle_length),
            crossing_distance(crossing_distance),
            edge_slide(edge_slide),
            edge_type(edge_type)
    {}

    /// Set disabled style to the values in other
    Edge_Style default_to(const Edge_Style& other) const;


};

Q_DECLARE_METATYPE(Edge_Style::Enabled_Styles)

#endif // EDGE_STYLE_HPP
