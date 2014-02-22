/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#ifndef PLUGIN_CROSSING_HPP
#define PLUGIN_CROSSING_HPP
#include "plugin.hpp"
#include "edge_type.hpp"

/**
 * \brief Plugin that manages a scripted edge type
 */
class Plugin_Crossing : public Plugin
{
    class Edge_Scripted* edge_type;
public:
    Plugin_Crossing(const QVariantMap &metadata);
    ~Plugin_Crossing();

protected:
    void on_enable(bool b) override;
};

#endif // PLUGIN_CROSSING_HPP
