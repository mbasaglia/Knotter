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

#include "context_menu_edge.hpp"
#include "commands.hpp"
#include "resource_manager.hpp"
#include <QInputDialog>
#include "dialog_edge_properties.hpp"

Context_Menu_Edge::Context_Menu_Edge(Knot_View *parent) :
    QMenu(parent), view(parent), edge_type_actions(new QActionGroup(this))
{
    action_snap = addAction(QIcon::fromTheme("snap-orthogonal"),tr("Snap to grid"),this,SLOT(snap()));
    addAction(QIcon::fromTheme("format-disconnect-node"),tr("Remove"),this,SLOT(remove()));

    menu_edge_types = new QMenu(tr("Edge type"));
    addMenu ( menu_edge_types );


    addAction(tr("Break on intersections"),this,SLOT(break_intersections()));
    addAction(tr("Subdivide..."),this,SLOT(subdivide()));

    addAction(QIcon::fromTheme("edge-crossing"),tr("Properties..."),this,SLOT(properties()));
    action_reset_style = addAction(tr("Reset custom style"),this,SLOT(reset_custom_style()));

    connect(&mapper,SIGNAL(mapped(QString)),SLOT(change_edge_type(QString)));
}

void Context_Menu_Edge::popup(Edge *e, QPoint pos)
{
    edge = e;

    action_snap->setEnabled( view->grid().is_enabled() );
    action_reset_style->setEnabled(
        edge->style().enabled_style != Edge_Style::EDGE_TYPE ||
        edge->style().edge_type != Resource_Manager::default_edge_type()
    );

    menu_edge_types->clear();
    foreach(Edge_Type* es, Resource_Manager::edge_types())
    {
        QAction*a = menu_edge_types->addAction(es->icon(),es->name(),&mapper,SLOT(map()));
        mapper.setMapping(a,es->machine_name());
        a->setCheckable(true);
        a->setChecked(e->style().edge_type == es);
        edge_type_actions->addAction(a);
    }

    QMenu::popup(pos);
}

void Context_Menu_Edge::snap()
{

    view->begin_macro(tr("Snap to Grid"));
    view->push_command(new Move_Node(edge->vertex1(),edge->vertex1()->pos(),
                            view->grid().nearest(edge->vertex1()->pos()),view));
    view->push_command(new Move_Node(edge->vertex2(),edge->vertex2()->pos(),
                            view->grid().nearest(edge->vertex2()->pos()),view));
    view->end_macro();
}

void Context_Menu_Edge::change_edge_type(QString type_name)
{
    view->push_command(new Change_Edge_Type(edge,edge->style().edge_type,
                Resource_Manager::edge_type_from_machine_name(type_name), view));
}


void Context_Menu_Edge::remove()
{
    view->remove_edge(edge);
}


struct Node_Comparator
{
    QPointF base;

    Node_Comparator(Node*node) :base(node->pos()) {}


    bool operator() (Node* a, Node* b)
    {
        return point_distance_squared(base,a->pos()) < point_distance_squared(base,b->pos());
    }
};

void Context_Menu_Edge::break_intersections()
{

    QList<Edge*> intersected;

    foreach ( QGraphicsItem* ietm, edge->collidingItems() )
    {
        Edge* other_edge = dynamic_cast<Edge*>(ietm);
        if ( other_edge && !other_edge->is_vertex(edge->vertex1())
             && !other_edge->is_vertex(edge->vertex2()) )
            intersected.push_back(other_edge);
    }

    if ( intersected.empty() )
        return;

    view->begin_macro("Break edge");


    QLineF start_line = edge->to_line();

    QList<Node*> new_nodes;

    foreach ( Edge* o, intersected )
    {
        QLineF intersect_line  = o->to_line();
        QPointF intersect_point;
        start_line.intersect(intersect_line,&intersect_point);

        Node* next_node = view->add_node(intersect_point);

        new_nodes.push_back(next_node);

        // break other edge (1)
        Edge* next_edge = view->add_edge(o->vertex1(),next_node);
        next_edge->set_style(o->style());
        // break other edge (2)
        next_edge = view->add_edge(next_node,o->vertex2());
        next_edge->set_style(o->style());

        view->remove_edge(o);
    }

    qSort(new_nodes.begin(),new_nodes.end(),Node_Comparator(edge->vertex1()));
    new_nodes.push_front(edge->vertex1());
    new_nodes.push_back(edge->vertex2());

    for ( int i = 0; i < new_nodes.size()-1; i++ )
    {
        Edge* next_edge = view->add_edge(new_nodes[i],new_nodes[i+1]);
        next_edge->set_style(edge->style());
    }

    view->remove_edge ( edge );


    view->end_macro();
}

void Context_Menu_Edge::subdivide()
{
    bool ok;
    int segments = QInputDialog::getInt(view,tr("Break edge"),
                                tr("Number of segments"),2,2,32,1,&ok);
    if ( ok )
    {

        view->begin_macro(tr("Subdivide Edge"));

        Node* v1 = edge->vertex1();
        Node* v2 = edge->vertex2();

        QLineF line  = edge->to_line();


        line.setLength(line.length()/segments);
        Node* last = v1;
        for ( int i = 1; i < segments; i++ )
        {

            Node* next = view->add_node(line.p2());
            Edge* e = view->add_edge(last,next);
            e->set_style(edge->style());

            last = next;
            line.translate(line.p2()-line.p1());
        }
        Edge* e = view->add_edge(last,v2);
        e->set_style(edge->style());
        view->remove_edge ( edge );

        view->end_macro();
    }
}

void Context_Menu_Edge::properties()
{
    Dialog_Edge_Properties dialog;
    dialog.set_style(edge->style());
    if ( dialog.exec() )
    {
        view->push_command(new Edge_Style_All(edge,edge->style(),
                                              dialog.edge_style(),view));
    }
}

void Context_Menu_Edge::reset_custom_style()
{
    Edge_Style es;
    es.enabled_style |= Edge_Style::EDGE_TYPE;
    es.edge_type = Resource_Manager::default_edge_type();
    view->begin_macro(tr("Reset Edge Style"));
    view->push_command(new Edge_Style_All(edge,edge->style(),es,view));
    view->end_macro();
}
