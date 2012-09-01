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
#include "commands.hpp"
#include "translator.hpp"

KnotViewUndoCommand::KnotViewUndoCommand ( KnotView* kv )
    : kv ( kv )
{
    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));
}
int KnotViewUndoCommand::get_id()
{
    static int next = 1;
           //docmd
    return 0xD0C3D000 | next++;
}

LastNodeCommand::LastNodeCommand(KnotView *kv)
    : KnotViewUndoCommand ( kv )
{
}

AddNode::AddNode(Node *node, node_list links, KnotView *kv)
    : LastNodeCommand ( kv ),
      node ( node ), links (links)
{
    retranslate();
}

void AddNode::undo()
{
    kv->do_remove_node(node);
}

void AddNode::redo()
{
    kv->do_add_node(node,links);
}

/// delete node if it is not within a scene
AddNode::~AddNode()
{
    if ( !node->scene() )
        delete node;
}

Node *AddNode::get_node() const
{
    return node;
}

void AddNode::retranslate()
{
    setText( tr("Insert Node") );
}

RemoveNode::RemoveNode(Node *node, node_list links, KnotView *kv)
    : KnotViewUndoCommand(kv), node ( node ), links (links)
{
    retranslate();
}

void RemoveNode::undo()
{
    kv->do_add_node(node,links);
}

void RemoveNode::redo()
{
    kv->do_remove_node(node);
}

void RemoveNode::retranslate()
{
    setText(tr("Remove Node"));
}

AddEdge::AddEdge(Node *n1, Node *n2, KnotView *kv)
    : LastNodeCommand(kv), n1 ( n1 ), n2 ( n2 )
{
    retranslate();
}

void AddEdge::undo()
{
    kv->unlink(n1,n2);
}

void AddEdge::redo()
{
    kv->link(n1,n2);
}

Node *AddEdge::get_node() const
{
    return n2;
}

void AddEdge::retranslate()
{
    setText(tr("Insert Edge"));
}

RemoveEdge::RemoveEdge(Node *n1, Node *n2, KnotView *kv)
    : KnotViewUndoCommand(kv), n1 ( n1 ), n2 ( n2 )
{
    retranslate();
}

void RemoveEdge::undo()
{
    kv->link(n1,n2);
}

void RemoveEdge::redo()
{
    kv->unlink(n1,n2);
}

void RemoveEdge::retranslate()
{
    setText(tr("Remove Edge"));
}

MoveNode::MoveNode(Node *node, QPointF start_pos, QPointF end_pos, KnotView *kv)
    : KnotViewUndoCommand(kv), node(node), start_pos ( start_pos ), end_pos ( end_pos )
{
    retranslate();
}

void MoveNode::undo()
{
    kv->do_move_node(node,start_pos);
}

void MoveNode::redo()
{
    kv->do_move_node(node,end_pos);
}

void MoveNode::retranslate()
{
    setText(tr("Move Node"));
}

void ToggleEdge::retranslate()
{
    setText(tr("Change Edge Type"));
}

bool ToggleEdge::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id())
        return false;
    const ToggleEdge* o = dynamic_cast<const ToggleEdge*>(other);
    if ( !o || o->from != to || n1 != o->n1 || n2 != o->n2 )
        return false;
    to = o->to;
    return true;
}

int ToggleEdge::id_ = KnotViewUndoCommand::get_id();

ToggleEdge::ToggleEdge(Node *n1, Node *n2, Edge::type_type from, Edge::type_type to, KnotView *kv)
    : KnotViewUndoCommand(kv), n1 ( n1 ), n2 ( n2 ), from ( from ), to ( to )
{
    retranslate();
}

void ToggleEdge::undo()
{
    kv->do_toggle_edge(n1,n2,from);
}

void ToggleEdge::redo()
{
    kv->do_toggle_edge(n1,n2,to);
}

int ToggleEdge::id()
{
    return id_;
}

void ChangeDefaultNodeStyle::retranslate()
{
    setText(tr("Change default node style"));
}

int ChangeDefaultNodeStyle::id_ = KnotViewUndoCommand::get_id();

ChangeDefaultNodeStyle::ChangeDefaultNodeStyle(styleinfo style_old, styleinfo style_new, KnotView *kv)
    : KnotViewUndoCommand(kv), style_old(style_old), style_new(style_new)
{
    retranslate();
}

void ChangeDefaultNodeStyle::undo()
{
    kv->do_set_default_style(style_old);
}

void ChangeDefaultNodeStyle::redo()
{
    kv->do_set_default_style(style_new);
}

int ChangeDefaultNodeStyle::id() const
{
    return id_;
}

bool ChangeDefaultNodeStyle::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id())
        return false;
    const ChangeDefaultNodeStyle* o = dynamic_cast<const ChangeDefaultNodeStyle*>(other);
    if ( o )
    {
        style_new = o->style_new;
        return true;
    }
    return false;
}

ChangeCustomNodeStyle::ChangeCustomNodeStyle(Node *node, styleinfo style_old, styleinfo style_new, KnotView *kv)
    : KnotViewUndoCommand(kv),
    was_disabled(false), node(node), style_old(style_old), style_new(style_new)
{
    retranslate();
    was_disabled = !node->has_custom_style();
}

void ChangeCustomNodeStyle::undo()
{
    if ( was_disabled )
        node->disable_custom_style();
    else
        node->set_custom_style(style_old);
    kv->redraw(true);
}

void ChangeCustomNodeStyle::redo()
{
    node->set_custom_style(style_new);
    kv->redraw(true);
}

void ChangeCustomNodeStyle::retranslate()
{
    setText(tr("Change node style"));
}

RemoveNodeStyle::RemoveNodeStyle(Node *node, KnotView *kv)
    : KnotViewUndoCommand(kv), node(node)
{
    retranslate();
}

void RemoveNodeStyle::undo()
{
    node->enable_custom_style();
    kv->redraw(true);
}

void RemoveNodeStyle::redo()
{
    node->disable_custom_style();
    kv->redraw(true);
}

void RemoveNodeStyle::retranslate()
{
    setText(tr("Remove custom node style"));
}

void ChangeKnotWidth::retranslate()
{
    setText(tr("Change knot width"));
}

int ChangeKnotWidth::id_ = KnotViewUndoCommand::get_id();

ChangeKnotWidth::ChangeKnotWidth(double old_width, double new_width, KnotView *kv)
    : KnotViewUndoCommand(kv), old_width(old_width), new_width(new_width), accepted ( false )
{
    retranslate();
}

ChangeKnotWidth::ChangeKnotWidth(double old_width, double new_width, bool accepted, KnotView *kv)
    : KnotViewUndoCommand(kv), old_width(old_width), new_width(new_width), accepted ( accepted )
{
    retranslate();
}


void ChangeKnotWidth::undo()
{
    kv->do_set_width(old_width);
}

void ChangeKnotWidth::redo()
{
    kv->do_set_width(new_width);
}

int ChangeKnotWidth::id() const
{
    return id_;
}

bool ChangeKnotWidth::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id() )
        return false;
    const ChangeKnotWidth* o = dynamic_cast<const ChangeKnotWidth*>(other);
    if ( o && !accepted )
    {
        accepted = o->accepted;
        new_width = o->new_width;
        return true;
    }
    else if ( o && accepted && qFuzzyCompare(new_width, o->new_width ) )
        return true; // prevent enter + lose focus to cause multiple entries
    return false;
}


int ChangeKnotBrush::id_ = KnotViewUndoCommand::get_id();

ChangeKnotBrush::ChangeKnotBrush(QBrush old_brush, QBrush new_brush, KnotView *kv)
    : KnotViewUndoCommand(kv), old_brush(old_brush), new_brush(new_brush), accepted(false)
{
    retranslate();
}

ChangeKnotBrush::ChangeKnotBrush(QBrush old_brush, QBrush new_brush, bool accepted, KnotView *kv)
    : KnotViewUndoCommand(kv), old_brush(old_brush), new_brush(new_brush), accepted(accepted)
{
    retranslate();
}

void ChangeKnotBrush::undo()
{
    kv->do_set_brush(old_brush);
}

void ChangeKnotBrush::redo()
{
    kv->do_set_brush(new_brush);
}

int ChangeKnotBrush::id() const
{
    return id_;
}

bool ChangeKnotBrush::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id() )
        return false;
    const ChangeKnotBrush* o = dynamic_cast<const ChangeKnotBrush*>(other);
    if ( o && !accepted )
    {
        accepted = o->accepted;
        new_brush = o->new_brush;
        return true;
    }
    return false;
}

void ChangeKnotBrush::retranslate()
{
    setText(tr("Change knot color"));
}

void ChangeKnotPen::retranslate()
{
    setText(tr("Change knot outline"));
}

bool ChangeKnotPen::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id())
        return false;
    const ChangeKnotPen* o = dynamic_cast<const ChangeKnotPen*>(other);
    if ( o )
    {
        new_pen = o->new_pen;
        return true;
    }
    return false;
}


int ChangeKnotPen::id_ = KnotViewUndoCommand::get_id();

ChangeKnotPen::ChangeKnotPen(QPen old_pen, QPen new_pen, KnotView *kv)
    : KnotViewUndoCommand(kv), old_pen(old_pen), new_pen(new_pen)
{
    retranslate();
}

void ChangeKnotPen::undo()
{
    kv->do_set_pen(old_pen);
}

void ChangeKnotPen::redo()
{
    kv->do_set_pen(new_pen);
}

int ChangeKnotPen::id() const
{
    return id_;
}

ChangeKnotJoinStyle::ChangeKnotJoinStyle(Qt::PenJoinStyle old_pjs, Qt::PenJoinStyle new_pjs, KnotView *kv)
    : KnotViewUndoCommand(kv), old_pjs(old_pjs), new_pjs(new_pjs)
{
    retranslate();
}

void ChangeKnotJoinStyle::undo()
{
    kv->do_set_join_style(old_pjs);
}

void ChangeKnotJoinStyle::redo()
{
    kv->do_set_join_style(new_pjs);
}

void ChangeKnotJoinStyle::retranslate()
{
    setText(tr("Change knot point style"));
}

