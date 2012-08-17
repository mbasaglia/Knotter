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
#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "knotview.hpp"
#include <QUndoCommand>

class LastNodeCommand : public QUndoCommand
{
    public:
        virtual Node* get_node() const { return NULL; }
};

class AddNode : public LastNodeCommand
{
    Node*     node;
    node_list links;
    KnotView* kv;

    public:
        AddNode ( Node* node, node_list links, KnotView* kv )
            : node ( node ), links (links), kv ( kv )
        {
            setText("Insert Node");
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


class RemoveNode : public QUndoCommand
{
    Node*     node;
    node_list links;
    KnotView* kv;

    public:
        RemoveNode ( Node* node, node_list links, KnotView* kv )
            : node ( node ), links (links), kv ( kv )
        {
            setText("Remove Node");
        }

        void undo() { kv->do_add_node(node,links); }
        void redo() { kv->do_remove_node(node); }

};

class AddEdge : public LastNodeCommand
{
    Node *n1, *n2;
    KnotView* kv;

    public:
        AddEdge ( Node* n1, Node* n2, KnotView* kv )
            : n1 ( n1 ), n2 ( n2 ), kv ( kv )
        {
            setText("Insert Edge");
        }

        void undo() { kv->unlink(n1,n2); }
        void redo() { kv->link(n1,n2); }

        Node* get_node() const { return n2; }
};

class RemoveEdge : public QUndoCommand
{
    Node *n1, *n2;
    KnotView* kv;

    public:
        RemoveEdge ( Node* n1, Node* n2, KnotView* kv )
            : n1 ( n1 ), n2 ( n2 ), kv ( kv )
        {
            setText("Remove Edge");
        }

        void undo() { kv->link(n1,n2); }
        void redo() { kv->unlink(n1,n2); }
};

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
            setText("Move Node");
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
            setText("Change Edge Type");
        }

        void undo() { kv->do_toggle_edge(n1,n2,from); }
        void redo() { kv->do_toggle_edge(n1,n2,to); }

        int id() const {return 3*14159265;}

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


#endif // COMMANDS_HPP
