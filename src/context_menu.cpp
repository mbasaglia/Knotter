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
#include <QInputDialog>
#include "resource_loader.hpp"
node_cxmn::node_cxmn ( QWidget *parent )
    : QMenu(parent), n(NULL), kv ( NULL )
{
    snap_grid = addAction(load::icon("snap-grid"),tr("Snap to grid"),this,SLOT(snap()));
    addAction(load::icon("format-remove-node"),tr("Remove"),this,SLOT(remove()));
    addAction(load::icon("edit-node"),tr("Properties..."),this,SLOT(properties()));
    unset_style = addAction(tr("Reset custom style"),this,SLOT(no_custom_style()));
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
        kv->get_undo_stack().beginMacro(tr("Snap Node"));
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
        kv->disable_custom_style(n);
    }
}

void node_cxmn::remove()
{
    if ( n && kv )
    {
        kv->remove_node(n);
    }
}

edge_cxmn::edge_cxmn(QWidget *parent)
    : QMenu(parent), n1(NULL), n2(NULL), kv(NULL)
{
    snap_grid = addAction(load::icon("square_grid"),tr("Snap to grid"),this,SLOT(snap()));
    addAction(load::icon("format-disconnect-node"),tr("Remove"),this,SLOT(remove()));

    QMenu *type = new QMenu(tr("Edge type"));
    type_action[Edge::CROSSING] = type->addAction(tr("Regular"),this,SLOT(set_regular()));
    type_action[Edge::INVERTED] = type->addAction(tr("Inverted"),this,SLOT(set_inverted()));
    type_action[Edge::WALL] = type->addAction(tr("Wall"),this,SLOT(set_wall()));
    type_action[Edge::HOLE] = type->addAction(tr("Hole"),this,SLOT(set_hole()));
    foreach(QAction*a,type_action.values())
        a->setCheckable(true);
    addMenu ( type );


    addAction(tr("Break on intersections"),this,SLOT(break_edge_intersections()));
    addAction(tr("Break..."),this,SLOT(break_edge_equal()));

    //addAction(load::icon("draw-line"),tr("Properties..."),this,SLOT(properties()));

}

void edge_cxmn::set_view(KnotView *k_v)
{
    kv = k_v;
}

void edge_cxmn::activate(Edge *edge)
{
    if ( edge && kv )
    {
        n1 = edge->vertex1();
        n2 = edge->vertex2();
        snap_grid->setEnabled ( kv->get_grid().is_enabled() );
        foreach(QAction*a,type_action.values())
            a->setChecked(false);
        type_action[edge->type]->setChecked(true);
        exec(QCursor::pos());
    }
}

void edge_cxmn::snap()
{
    if ( n1 && n2 && kv )
    {
        kv->get_undo_stack().beginMacro(tr("Snap Edge"));
        kv->move_node(n1, kv->get_grid().nearest(n1->pos()) );
        kv->move_node(n2, kv->get_grid().nearest(n2->pos()) );
        kv->get_undo_stack().endMacro();
    }
}

void edge_cxmn::properties()
{
    if ( n1 && n2 && kv )
        emit request_properties(n1->get_link(n2));
}

void edge_cxmn::remove()
{
    if ( n1 && n2 && kv )
        kv->remove_edge(n1,n2);
}

void edge_cxmn::set_regular()
{
    if ( n1 && n2 && kv )
        kv->set_edge_type(n1,n2,Edge::CROSSING);
}

void edge_cxmn::set_inverted()
{
    if ( n1 && n2 && kv )
        kv->set_edge_type(n1,n2,Edge::INVERTED);
}

void edge_cxmn::set_wall()
{
    if ( n1 && n2 && kv )
        kv->set_edge_type(n1,n2,Edge::WALL);
}

void edge_cxmn::set_hole()
{
    if ( n1 && n2 && kv )
        kv->set_edge_type(n1,n2,Edge::HOLE);
}

void edge_cxmn::break_edge_intersections()
{
    if ( n1 && n2 && kv )
        kv->break_edge_intersections(n1->get_link(n2));
}

void edge_cxmn::break_edge_equal()
{
    if ( n1 && n2 && kv )
    {
        bool ok;
        int segments = QInputDialog::getInt(0,tr("Break edge"),
                                    tr("Number of segments"),2,2,32,1,&ok);
        kv->break_edge_equal(n1->get_link(n2),segments);
    }
}
