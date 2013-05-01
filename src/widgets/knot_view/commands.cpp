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

Knot_Command::Knot_Command(Knot_View *view, Knot_Macro *parent)
    : QObject(parent),
      view(view), graph(&view->graph), scene(view->scene())
{}

void Knot_Command::set_parent(Knot_Macro *macro)
{
    setParent(macro);
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

void Knot_Command::update_knot() const
{
    if ( qobject_cast<Knot_Macro*>(parent()) == nullptr )
        view->update_knot();
}



Create_Node::Create_Node(Node *node, Knot_View *kv, Knot_Macro* parent)
    : Knot_Command(kv,parent), node(node)
{
    setText(tr("Create Node"));
}


void Create_Node::undo()
{
    graph->remove_node(node);
    scene->removeItem(node);
    update_knot();
}

void Create_Node::redo()
{
    graph->add_node(node);
    scene->addItem(node);
    update_knot();
}


Create_Node::~Create_Node()
{
    delete node;
}


Create_Edge::Create_Edge(Edge *edge, Knot_View *kv, Knot_Macro* parent)
    : Knot_Command(kv,parent), edge(edge)
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


Last_Node::Last_Node(Node *node_before, Node *node_after, Knot_View *kv, Knot_Macro* parent)
    : Knot_Command(kv,parent), node_before(node_before), node_after(node_after)
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


Remove_Edge::Remove_Edge(Edge *edge, Knot_View *kv, Knot_Macro* parent)
    : Knot_Command(kv,parent), edge(edge)
{
    setText(tr("Remove Edge"));
}

void Remove_Edge::redo()
{
    graph->remove_edge(edge);
    scene->removeItem(edge);
    update_knot();
}

void Remove_Edge::undo()
{
    graph->add_edge(edge);
    scene->addItem(edge);
    update_knot();
}


int Change_Colors::m_id = generate_id();
Change_Colors::Change_Colors(QList<QColor> before, QList<QColor> after, Knot_View *kv,
                             Knot_Macro* parent )
    : Knot_Command(kv,parent),before(before),after(after)
{
    setText(tr("Change Color"));
}

void Change_Colors::undo()
{
    graph->set_colors(before);
    scene->invalidate(graph->boundingRect());
}

void Change_Colors::redo()
{
    graph->set_colors(after);
    scene->invalidate(graph->boundingRect());
}

int Change_Colors::id() const
{
    return m_id;
}

bool Change_Colors::mergeWith(const QUndoCommand *other)
{
    const Change_Colors* cc = static_cast<const Change_Colors*>(other);
    if ( cc->graph == graph )
    {
        after = cc->after;
        return true;
    }
    return false;
}


Move_Node::Move_Node(Node *node, QPointF before, QPointF after, Knot_View *kv,
                     Knot_Macro* parent)
    : Knot_Command(kv,parent), node(node), before(before), after(after)
{
    setText(tr("Move Node"));
}

void Move_Node::undo()
{
    node->setPos(before);
    update_knot();
}

void Move_Node::redo()
{
    node->setPos(after);
    update_knot();
}


int Knot_Width::m_id = generate_id();

Knot_Width::Knot_Width(double before, double after, Knot_View *kv, Knot_Macro* parent)
    : Knot_Command(kv,parent),before(before), after(after)
{
    setText(tr("Change Stroke Width"));
}

void Knot_Width::undo()
{
    graph->set_width(before);
}

void Knot_Width::redo()
{
    graph->set_width(after);
}

int Knot_Width::id() const
{
    return m_id;
}

bool Knot_Width::mergeWith(const QUndoCommand *other)
{
    const Knot_Width* cc = static_cast<const Knot_Width*>(other);
    if ( cc->graph == graph )
    {
        after = cc->after;
        return true;
    }
    return false;
}


void Knot_Macro::redo()
{
    foreach ( QObject* obj, children() )
        qobject_cast<Knot_Command*>(obj)->redo();
    update_knot();
}

void Knot_Macro::undo()
{
    foreach ( QObject* obj, children() )
        qobject_cast<Knot_Command*>(obj)->undo();
    update_knot();
}


Remove_Node::Remove_Node(Node *node, Knot_View *kv, Knot_Macro *parent)
    : Knot_Command(kv,parent),node(node)
{
    setText(tr("Remove Node"));
}

void Remove_Node::undo()
{
    graph->add_node(node);
    update_knot();
}

void Remove_Node::redo()
{
    graph->remove_node(node);
    update_knot();
}
