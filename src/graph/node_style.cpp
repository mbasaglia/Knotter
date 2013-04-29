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

#include "node_style.hpp"

Node_Style Node_Style::default_to(const Node_Style &other) const
{

    return Node_Style (
                //(enabled_style & CURVE_STYLE ) ? curve_style : other.curve_style,
                (enabled_style & CUSP_ANGLE ) ? cusp_angle : other.cusp_angle,
                (enabled_style & HANDLE_LENGTH ) ? handle_length : other.handle_length,
                (enabled_style & CROSSING_DISTANCE ) ? crossing_distance : other.crossing_distance,
                (enabled_style & CUSP_DISTANCE ) ? cusp_distance : other.cusp_distance,
                (enabled_style & CURVE_STYLE) && cusp_shape ? cusp_shape : other.cusp_shape,
                EVERYTHING
            );
}

