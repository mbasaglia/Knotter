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
/**
    \file
    Here are located the Undo/Redo commands.

    Note that when KnotView needs to alter the graph it creates one of these,
    which then calls KnotView back to perform the action

    \todo translatebale names
*/
#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "knotview.hpp"
#include <QUndoCommand>

/**
    \brief command that alters last_node

    Used to backtrack when in edge list mode
*/
class LastNodeCommand : public QUndoCommand
{
    public:
        virtual Node* get_node() const { return NULL; }
};

/**
    \brief Inserts the node specified

    This class takes ownership of the node pointer, when destructed it will
    delete the node if that is not owned by a scene.
*/
class AddNode : public LastNodeCommand
{
    Node*     node;
    node_list links;
    KnotView* kv;

    public:
        AddNode ( Node* node, node_list links, KnotView* kv )
            : node ( node ), links (links), kv ( kv )
        {
            setText(KnotView::tr("Insert Node"));
        }

        void undo() { kv->do_remove_node(node); }
        void redo() { kv->do_add_node(node,links); }

        ~AddNode()
        {
            if ( !node->scene() )
                delete node;
        }

        Node* get_node() const { return node; }

};

/**
    \brief Removes a node from the scene (doesn't delete it)
*/
class RemoveNode : public QUndoCommand
{
    Node*     node;
    node_list links;
    KnotView* kv;

    public:
        RemoveNode ( Node* node, node_list links, KnotView* kv )
            : node ( node ), links (links), kv ( kv )
        {
            setText(KnotView::tr("Remove Node"));
        }

        void undo() { kv->do_add_node(node,links); }
        void redo() { kv->do_remove_node(node); }

};

/**
    \brief adds an edge between two nodes
*/
class AddEdge : public LastNodeCommand
{
    Node *n1, *n2;
    KnotView* kv;

    public:
        AddEdge ( Node* n1, Node* n2, KnotView* kv )
            : n1 ( n1 ), n2 ( n2 ), kv ( kv )
        {
            setText(KnotView::tr("Insert Edge"));
        }

        void undo() { kv->unlink(n1,n2); }
        void redo() { kv->link(n1,n2); }

        Node* get_node() const { return n2; }
};

/**
    \brief removes an edge between two nodes
*/
class RemoveEdge : public QUndoCommand
{
    Node *n1, *n2;
    KnotView* kv;

    public:
        RemoveEdge ( Node* n1, Node* n2, KnotView* kv )
            : n1 ( n1 ), n2 ( n2 ), kv ( kv )
        {
            setText(KnotView::tr("Remove Edge"));
        }

        void undo() { kv->link(n1,n2); }
        void redo() { kv->unlink(n1,n2); }
};

/**
    \brief Move a node from start_pos to end_pos
*/
class MoveNode : public QUndoCommand
{
    Node* node;
    QPointF start_pos;
    QPointF end_pos;
    KnotView* kv;

    public:
        MoveNode ( Node* node, QPointF start_pos, QPointF end_pos, KnotView* kv )
            : node(node), start_pos ( start_pos ), end_pos ( end_pos ), kv ( kv )
        {
            setText(KnotView::tr("Move Node"));
        }

        void undo()
        {
            kv->do_move_node(node,start_pos);
        }
        void redo()
        {
            kv->do_move_node(node,end_pos);
        }
};

/**
    \brief Changes edge type
*/
class ToggleEdge : public QUndoCommand
{
    Node *n1, *n2;
    Edge::type_type from;
    Edge::type_type to;
    KnotView* kv;

    public:
        ToggleEdge ( Node* n1, Node* n2, Edge::type_type from,
                    Edge::type_type to, KnotView* kv )
            : n1 ( n1 ), n2 ( n2 ), from ( from ), to ( to ), kv ( kv )
        {
            setText(KnotView::tr("Change Edge Type"));
        }

        void undo() { kv->do_toggle_edge(n1,n2,from); }
        void redo() { kv->do_toggle_edge(n1,n2,to); }

        int id() const {return 0xD0C3D001;} // number: do cmd 001

        bool mergeWith(const QUndoCommand *other)
        {
            if (other->id() != id())
                return false;
            const ToggleEdge* o = dynamic_cast<const ToggleEdge*>(other);
            if ( !o || o->from != to || n1 != o->n1 || n2 != o->n2 )
                return false;
            to = o->to;
            return true;
        }


};


/**
    \brief Change default node style
*/
class ChangeDefaultNodeStyle : public QUndoCommand
{
    styleinfo style_old, style_new;
    KnotView* kv;

    public:
        ChangeDefaultNodeStyle ( styleinfo style_old, styleinfo  style_new, KnotView* kv )
            : style_old(style_old), style_new(style_new), kv ( kv )
        {
            setText(KnotView::tr("Change default node style"));
        }

        void undo()
        {
            kv->do_set_default_style(style_old);
        }
        void redo()
        {
            kv->do_set_default_style(style_new);
        }

        int id() const {return 0xD0C3D002;} // number: do cmd 002

        bool mergeWith(const QUndoCommand *other)
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
};

/**
    \brief Change node style
    \todo create commands on change
*/
class ChangeCustomNodeStyle : public QUndoCommand
{
    bool was_disabled;
    Node *node;
    styleinfo style_old, style_new;
    KnotView* kv;

    public:
        ChangeCustomNodeStyle ( Node *node, styleinfo style_old, styleinfo  style_new, KnotView* kv )
            : was_disabled(false), node(node),
              style_old(style_old), style_new(style_new), kv ( kv )
        {
            was_disabled = !node->has_custom_style();
            setText(KnotView::tr("Change node style"));
        }

        void undo()
        {
            if ( was_disabled )
                node->disable_custom_style();
            else
                node->set_custom_style(style_old);
            kv->redraw(true);
        }
        void redo()
        {
            node->set_custom_style(style_new);
            kv->redraw(true);
        }
};
/**
    \brief Set node to follow default style
*/
class RemoveNodeStyle : public QUndoCommand
{
    Node *node;
    KnotView* kv;
    public:
        RemoveNodeStyle ( Node *node, KnotView* kv )
            : node(node), kv ( kv )
        {
            setText(KnotView::tr("Remove custom node style"));
        }

        void undo()
        {
            node->enable_custom_style();
            kv->redraw(true);
        }
        void redo()
        {
            node->disable_custom_style();
            kv->redraw(true);
        }
};

/**
    \brief Change knot width
*/
class ChangeKnotWidth : public QUndoCommand
{
    double old_width, new_width;
    KnotView* kv;

    public:
        ChangeKnotWidth ( double old_width, double new_width, KnotView* kv )
            : old_width(old_width), new_width(new_width), kv ( kv )
        {
            setText(KnotView::tr("Change knot width"));
        }

        void undo() { kv->do_set_width(old_width); }
        void redo() { kv->do_set_width(new_width); }

        int id() const {return 0xD0C3D003;} // number: do cmd 003

        bool mergeWith(const QUndoCommand *other)
        {
            if (other->id() != id())
                return false;
            const ChangeKnotWidth* o = dynamic_cast<const ChangeKnotWidth*>(other);
            if ( o )
            {
                new_width = o->new_width;
                return true;
            }
            return false;
        }
};
/**
    \brief Change knot color
*/
class ChangeKnotBrush : public QUndoCommand
{
    QBrush old_brush, new_brush;
    KnotView* kv;

    public:
        ChangeKnotBrush ( QBrush old_brush, QBrush new_brush, KnotView* kv )
            : old_brush(old_brush), new_brush(new_brush), kv ( kv )
        {
            setText(KnotView::tr("Change knot color"));
        }

        void undo() { kv->do_set_brush(old_brush); }
        void redo() { kv->do_set_brush(new_brush); }

        int id() const {return 0xD0C3D004;} // number: do cmd 004

        bool mergeWith(const QUndoCommand *other)
        {
            if (other->id() != id())
                return false;
            const ChangeKnotBrush* o = dynamic_cast<const ChangeKnotBrush*>(other);
            if ( o )
            {
                new_brush = o->new_brush;
                return true;
            }
            return false;
        }
};
/**
    \brief Change knot outline
*/
class ChangeKnotPen : public QUndoCommand
{
    QPen old_pen, new_pen;
    KnotView* kv;

    public:
        ChangeKnotPen ( QPen old_pen, QPen new_pen, KnotView* kv )
            : old_pen(old_pen), new_pen(new_pen), kv ( kv )
        {
            setText(KnotView::tr("Change knot outline"));
        }

        void undo() { kv->do_set_pen(old_pen); }
        void redo() { kv->do_set_pen(new_pen); }

        int id() const {return 0xD0C3D005;} // number: do cmd 005

        bool mergeWith(const QUndoCommand *other)
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
};
/**
    \brief Change knot join style
*/
class ChangeKnotJoinStyle : public QUndoCommand
{
    Qt::PenJoinStyle old_pjs, new_pjs;
    KnotView* kv;

    public:
        ChangeKnotJoinStyle ( Qt::PenJoinStyle old_pjs, Qt::PenJoinStyle new_pjs, KnotView* kv )
            : old_pjs(old_pjs), new_pjs(new_pjs), kv ( kv )
        {
            setText(KnotView::tr("Change knot point style"));
        }

        void undo() { kv->do_set_join_style(old_pjs); }
        void redo() { kv->do_set_join_style(new_pjs); }
};


#endif // COMMANDS_HPP
