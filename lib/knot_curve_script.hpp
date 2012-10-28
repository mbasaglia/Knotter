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
#ifndef KNOT_CURVE_SCRIPT_HPP
#define KNOT_CURVE_SCRIPT_HPP
#include "knot_curve_style.hpp"


class knot_curve_script : public knot_curve_style
{
    public:
        void draw_joint ( path_builder& path,
                            const TraversalInfo& ti,
                            styleinfo style );
};
#endif // KNOT_CURVE_SCRIPT_HPP
