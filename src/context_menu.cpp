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
#include "context_menu.hpp"
#include "knotview.hpp"

node_cxmn::node_cxmn ( QWidget *parent )
    : QMenu(parent), n(NULL), kv ( NULL )
{
    snap_grid = addAction(QIcon(":/img/square_grid.svg"),tr("Snap to grid"),this,SLOT(snap()));
    addAction(QIcon(":/img/edit_nodes.svg"),tr("Properties..."),this,SLOT(properties()));
    unset_style = addAction(tr("Use default style"),this,SLOT(no_custom_style()));
}

void node_cxmn::set_view(KnotView *k_v)
{
    kv = k_v;
}

void node_cxmn::activate(Node *node)
{
    n = node;
    if ( n && kv)
    {
        snap_grid->setEnabled ( kv->get_grid().is_enabled() );
        unset_style->setEnabled ( n->has_custom_style() );
        exec(QCursor::pos());
    }
}

void node_cxmn::snap()
{
    if ( n && kv )
    {
        kv->get_undo_stack().beginMacro("Snap Node");
        kv->move_node(n, kv->get_grid().nearest(n->pos()) );
        kv->get_undo_stack().endMacro();
    }
}

void node_cxmn::properties()
{
    if ( n )
        emit request_properties(n);
}

void node_cxmn::no_custom_style()
{
    if ( n && kv )
    {
        n->unset_style_info();
        kv->redraw(true);
    }
}
