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

#include "knot_tool.hpp"
#include "knot_view.hpp"
#include "commands.hpp"

Knot_Tool::Knot_Tool(Knot_View *view, Graph *graph)
    : view(view), graph(graph), m_enabled(false)
{
}

void Knot_Tool::release(const Mouse_Event &)
{
}

void Knot_Tool::double_click(const Mouse_Event &)
{
}

void Knot_Tool::enable(bool enable)
{
    m_enabled = enable;
    if ( m_enabled )
        activate();
    else
        deactivate();
}

Node *Knot_Tool::node_at(QPointF pos) const
{
    return view->node_at(pos);
}

Edge *Knot_Tool::edge_at(QPointF pos) const
{
    return view->edge_at(pos);
}

void Knot_Tool::initialize_movement(QPointF pivot)
{
    view->set_mouse_mode ( view->mouse_mode | Knot_View::MOVE_NODES );
    view->node_mover.initialize_movement(pivot);
}


bool Select_Tool::press(const Mouse_Event &event)
{


    if ( event.event->button() == Qt::LeftButton )
    {
        Node* node = node_at(event.mouse_pos);
        Edge* edge = edge_at(event.mouse_pos);
        bool modifier = event.event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier);

        if ( !node && !edge)
            return false;

        if ( node )
        {
            if ( modifier )
            {
                node->setSelected(!node->isSelected());
            }
            else
            {
                if ( !node->isSelected() )
                    view->scene()->clearSelection();
                node->setSelected(true);
                initialize_movement(node->pos());
            }
        }
        else if ( edge )
        {

            if ( modifier )
            {
                edge->setSelected(!edge->isSelected());
            }
            else
            {
                if ( !edge->isSelected() )
                    view->scene()->clearSelection();
                edge->setSelected(true);
                edge->vertex1()->setSelected(true);
                edge->vertex2()->setSelected(true);
                initialize_movement(edge->midpoint());
            }

        }

        view->update_selection(false);

        return true;
    }

    return false;
}

void Select_Tool::double_click(const Mouse_Event & event)
{

    Node *node=node_at(event.mouse_pos);

    if ( node )
    {
        view->begin_macro(Knot_View::tr("Snap to Grid"));
        view->push_command(new Move_Node(node,node->pos(),event.snapped_pos,view));
        view->end_macro();
    }
    else
        view->add_breaking_node(event.snapped_pos);
}

void Select_Tool::move(const Mouse_Event &event, QPointF &notify_pos)
{

    Node *node=node_at(event.mouse_pos);

    if ( node )
        notify_pos = node->pos();
}


Edge_Chain_Tool::Edge_Chain_Tool(Knot_View *view, Graph *graph)
    : Knot_Tool(view,graph), last_node(nullptr)
{
    guide.setPen(QPen(Qt::gray,0,Qt::DashLine));
}

bool Edge_Chain_Tool::press(const Mouse_Event &event)
{
    if ( event.event->button() == Qt::LeftButton )
    {

        Node* next_node = node_at(event.snapped_pos);
        Edge* next_edge = last_node && next_node ? last_node->edge_to(next_node) : nullptr;

        if ( !last_node || next_node != last_node )
        {
            QString title;

            if ( next_edge )
                title = Knot_View::tr("Move Ahead");
            else if ( !last_node && next_node )
                title = Knot_View::tr("Start Chain");
            else if ( !last_node)
                title = Knot_View::tr("Create Node");
            else
                title = Knot_View::tr("Add Edge");

            view->begin_macro(title);

            if ( !next_node )
                next_node = view->add_breaking_node(event.snapped_pos);

            if ( last_node && !next_edge && !last_node->has_edge_to(next_node))
                view->add_edge(last_node,next_node);

            view->push_command(new Last_Node(last_node,next_node,view));

            view->end_macro();
        }

        if ( !guide.scene() )
        {
            guide.setLine(QLineF(last_node->pos(),event.snapped_pos));
            view->scene()->addItem(&guide);
        }

        return true;
    }
    else if ( event.event->button() == Qt::RightButton )
        break_chain();

    return false;
}

void Edge_Chain_Tool::move(const Mouse_Event &event, QPointF &notify_pos)
{

    if ( last_node )
        guide.setLine(QLineF(last_node->pos(),event.snapped_pos));

    notify_pos = event.snapped_pos;
}

QCursor Edge_Chain_Tool::cursor() const
{
    return Qt::CrossCursor;
}


void Edge_Chain_Tool::set_last_node(Node *n)
{
    last_node = n;
    if ( !n && guide.scene() )
        view->scene()->removeItem(&guide);
    else if ( n )
    {
        guide.setLine(QLineF(n->pos(),guide.line().p2()));
    }
}

void Edge_Chain_Tool::deactivate()
{
    break_chain();
}

void Edge_Chain_Tool::break_chain()
{
    if ( guide.scene() )
        view->scene()->removeItem(&guide);
    last_node = nullptr;
}

Toggle_Edge_Tool::Toggle_Edge_Tool(Knot_View *view, Graph *graph)
    : Knot_Tool(view,graph)
{
    start_node = nullptr;
    guide.setPen(QPen(Qt::gray,0,Qt::DashLine));
}

bool Toggle_Edge_Tool::press(const Mouse_Event &event)
{
    if ( event.event->button() == Qt::LeftButton )
    {
        Edge* e = edge_at(event.mouse_pos);
        Node* n = node_at(event.mouse_pos);
        if ( e )
        {
            view->remove_edge(e);
        }
        else if ( n )
        {
            start_node = n;
            if ( !guide.scene() )
            {
                guide.setLine(QLineF(start_node->pos(),event.mouse_pos));
                view->scene()->addItem(&guide);
            }
        }
        else
        {
            // Limit distance to grid size * 3
            double max_distance = view->grid().size()*view->grid().size()*9;
            double min_distance = max_distance;
            Node* n1 = nullptr;
            Node* n2 = nullptr;
            // find closest node
            foreach(Node* n, graph->nodes())
            {
                double point_distance = point_distance_squared(n->pos(),event.mouse_pos);
                if ( point_distance < min_distance )
                {
                    n1 = n;
                    min_distance = point_distance;
                }
            }

            // find second closest node
            if ( n1 )
            {
                min_distance = max_distance;
                foreach(Node* n, graph->nodes())
                {
                    double point_distance = point_distance_squared(n->pos(),event.mouse_pos);
                    if ( n != n1 && point_distance < min_distance )
                    {
                        n2 = n;
                        min_distance = point_distance;
                    }
                }

                if ( n2 )
                {
                    if ( !n1->has_edge_to(n2) )
                        view->add_edge(n1,n2);
                    else
                        view->remove_edge(n1->edge_to(n2));
                }
            }
        }
        return true;
    }
    return false;
}

void Toggle_Edge_Tool::release(const Mouse_Event &event)
{
    if ( start_node )
    {
        Node* to = node_at(event.mouse_pos);
        if ( to && to != start_node )
        {
            Edge* old = start_node->edge_to(to);
            if ( old )
                view->remove_edge(old);
            else
                view->add_edge(start_node,to);
        }
        start_node = nullptr;
        view->scene()->removeItem(&guide);
    }
}

void Toggle_Edge_Tool::deactivate()
{
    if ( guide.scene() )
        view->scene()->removeItem(&guide);
    start_node = nullptr;
}

void Toggle_Edge_Tool::move(const Mouse_Event &event, QPointF &notify_pos)
{
    Node *node=node_at(event.mouse_pos);

    if ( node )
        notify_pos = node->pos();

    if ( start_node )
        guide.setLine(QLineF(start_node->pos(),event.mouse_pos));
}
