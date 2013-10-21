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

#ifndef PLUGIN_CUSP_HPP
#define PLUGIN_CUSP_HPP

#include "plugin.hpp"

/**
 * \brief Plugin than manages a scripted cusp shape
 */
class Plugin_Cusp : public Plugin
{
    class Cusp_Scripted *cusp_shape;
public:
    Plugin_Cusp(const QVariantMap &metadata);
    ~Plugin_Cusp();

protected:
    void on_enable(bool b) override;
};

#endif // PLUGIN_CUSP_HPP
