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

#include "commands.hpp"

int Knot_Command::auto_id = 0;


int Knot_Command::generate_id()
{
    return auto_id++;
}

void Knot_Command::set_last_node(Node *n)
{
    view->last_node = n;
    if ( !n && view->guide.scene() )
        scene->removeItem(&view->guide);
    else if ( n )
    {
        view->guide.setLine(QLineF(n->pos(),view->guide.line().p2()));
    }
}

Create_Node::Create_Node(Node *node, Knot_View *kv)
    : Knot_Command(kv), node(node)
{
    setText(tr("Create Node"));
}


void Create_Node::undo()
{
    graph->remove_node(node);
    scene->removeItem(node);
}

void Create_Node::redo()
{
    graph->add_node(node);
    scene->addItem(node);
}


Create_Node::~Create_Node()
{
    delete node;
}


Create_Edge::Create_Edge(Edge *edge, Knot_View *kv)
    : Knot_Command(kv), edge(edge)
{
    setText(tr("Create Edge"));
}

void Create_Edge::undo()
{
    graph->remove_edge(edge);
    scene->removeItem(edge);
}

void Create_Edge::redo()
{
    graph->add_edge(edge);
    scene->addItem(edge);
}


Create_Edge::~Create_Edge()
{
    delete edge;
}


Last_Node::Last_Node(Node *node_before, Node *node_after, Knot_View *kv)
    : Knot_Command(kv), node_before(node_before), node_after(node_after)
{
}

void Last_Node::undo()
{
    set_last_node(node_before);
}

void Last_Node::redo()
{
    set_last_node(node_after);
}


Remove_Edge::Remove_Edge(Edge *edge, Knot_View *kv)
    : Knot_Command(kv), edge(edge)
{
    setText(tr("Remove Edge"));
}

void Remove_Edge::redo()
{
    graph->remove_edge(edge);
    scene->removeItem(edge);
}

void Remove_Edge::undo()
{
    graph->add_edge(edge);
    scene->addItem(edge);
}

