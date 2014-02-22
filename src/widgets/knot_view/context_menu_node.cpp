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

#include "context_menu_node.hpp"
#include "resource_manager.hpp"
#include "commands.hpp"
#include "cusp_style_dialog.hpp"

Context_Menu_Node::Context_Menu_Node(Knot_View *parent) :
    QMenu(parent), view(parent)
{
    action_snap = addAction(QIcon::fromTheme("snap-orthogonal"),tr("Snap to grid"),this,SLOT(snap()));
    addAction(QIcon::fromTheme("format-remove-node"),tr("Remove"),this,SLOT(remove()));
    addAction(QIcon::fromTheme("edit-node"),tr("Properties..."),this,SLOT(properties()));
    action_reset_style = addAction(tr("Reset custom style"),this,SLOT(reset_custom_style()));
}

void Context_Menu_Node::popup(Node *n, QPoint pos)
{
    node = n;
    action_snap->setEnabled( view->grid().is_enabled() );
    action_reset_style->setEnabled(node->style().enabled_style != Node_Style::NOTHING);

    QMenu::popup(pos);
}

void Context_Menu_Node::reset_custom_style()
{

    view->push_command(new Node_Style_Enable(node,node->style().enabled_style,
                            Node_Style::NOTHING,tr("Reset Node Style"),view));
}

void Context_Menu_Node::properties()
{
    Cusp_Style_Dialog dialog;
    dialog.set_style(node->style());
    if ( dialog.exec() )
    {
        view->push_command(new Node_Style_All(node,node->style(),
                                              dialog.node_style(),view));
    }
}


void Context_Menu_Node::remove()
{
    view->remove_node(node);
}


void Context_Menu_Node::snap()
{
    view->begin_macro(tr("Snap to Grid"));
    view->push_command(new Move_Node(node,node->pos(),
                                     view->grid().nearest(node->pos()),view));
    view->end_macro();
}
