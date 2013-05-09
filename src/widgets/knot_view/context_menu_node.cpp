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

#include "context_menu_node.hpp"
#include "resource_manager.hpp"

Context_Menu_Node::Context_Menu_Node(Knot_View *parent) :
    QMenu(parent), view(parent)
{
    /*Reso
    snap_grid = addAction(load::icon("snap-grid"),tr("Snap to grid"),this,SLOT(snap()));
    addAction(load::icon("format-remove-node"),tr("Remove"),this,SLOT(remove()));
    addAction(load::icon("edit-node"),tr("Properties..."),this,SLOT(properties()));
    unset_style = addAction(tr("Reset custom style"),this,SLOT(no_custom_style()));*/
}
