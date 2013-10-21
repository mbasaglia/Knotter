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

#ifndef CUSP_SCRIPTED_HPP
#define CUSP_SCRIPTED_HPP

#include "node_cusp_shape.hpp"
#include "plugin_cusp.hpp"

class Cusp_Scripted : public Cusp_Shape
{
private:
    Plugin_Cusp* plugin;
public:
    Cusp_Scripted(Plugin_Cusp* plugin) : plugin(plugin) {}

    QString name() const override { return plugin->string_data("name"); }

    QString machine_name() const override;

    virtual QIcon icon() const override { return plugin->icon(); }

    void draw_joint ( Path_Builder& path,
                        const Traversal_Info& ti,
                        const Node_Style& style ) const override;
};

#endif // CUSP_SCRIPTED_HPP
