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
*/
#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "knotview.hpp"
#include <QUndoCommand>

namespace commands {

/**
    \brief base class for undo commands that change the state of a KnotView
*/
class KnotViewUndoCommand : public QObject, public QUndoCommand
{
        Q_OBJECT

    protected:
        KnotView* kv;

    public:
        KnotViewUndoCommand ( KnotView* kv );

    protected slots:
        /// override to set text
        virtual void retranslate() = 0;

    protected:
        /// ensure unique id();
        static int get_id();
};

/**
    \brief command that alters last_node

    Used to backtrack when in edge list mode
*/
class LastNodeCommand : public KnotViewUndoCommand
{
    public:
        LastNodeCommand ( KnotView* kv );
        virtual Node* get_node() const = 0;
};

/**
    \brief Inserts the node specified

    This class takes ownership of the node pointer, when destructed it will
    delete the node if that is not owned by a scene.
*/
class AddNode : public LastNodeCommand
{
        Q_OBJECT

    Node*     node;
    node_list links;

    public:
        AddNode ( Node* node, node_list links, KnotView* kv );

        void undo();
        void redo();

        ~AddNode();

        Node* get_node() const;

    protected:
        void retranslate();

};

/**
    \brief Removes a node from the scene (doesn't delete it)
*/
class RemoveNode : public KnotViewUndoCommand
{
        Q_OBJECT

    Node*     node;
    node_list links;

    public:
        RemoveNode ( Node* node, node_list links, KnotView* kv );

        void undo();
        void redo();

    protected:
        void retranslate();

};

/**
    \brief adds an edge between two nodes
*/
class AddEdge : public LastNodeCommand
{
        Q_OBJECT

    Node *n1, *n2;

    public:
        AddEdge ( Node* n1, Node* n2, KnotView* kv );

        void undo();
        void redo();

        Node* get_node() const;

    protected:
        void retranslate();
};

/**
    \brief removes an edge between two nodes
*/
class RemoveEdge : public KnotViewUndoCommand
{
        Q_OBJECT

    Node *n1, *n2;
    Edge::type_type type;

    public:
        RemoveEdge ( Node* n1, Node* n2, KnotView* kv );

        void undo();
        void redo();

    protected:
        void retranslate();
};

/**
    \brief Move a node from start_pos to end_pos
*/
class MoveNode : public KnotViewUndoCommand
{
        Q_OBJECT

    Node* node;
    QPointF start_pos;
    QPointF end_pos;

    public:
        MoveNode ( Node* node, QPointF start_pos, QPointF end_pos, KnotView* kv );

        void undo();
        void redo();

    protected:
        void retranslate();
};

/**
    \brief Changes edge type
*/
class ChangeEdgeType : public KnotViewUndoCommand
{
        Q_OBJECT

    Node *n1, *n2;
    Edge::type_type from;
    Edge::type_type to;

    public:
        ChangeEdgeType ( Node* n1, Node* n2, Edge::type_type from,
                    Edge::type_type to, KnotView* kv );

        void undo();
        void redo();

        int id() const;

        bool mergeWith(const QUndoCommand *other);

    protected:
        void retranslate();

    private:
        static int id_;


};


/**
    \brief Change default node style
*/
class ChangeDefaultNodeStyle : public KnotViewUndoCommand
{
        Q_OBJECT

    styleinfo style_old, style_new;

    public:
        ChangeDefaultNodeStyle ( styleinfo style_old, styleinfo  style_new, KnotView* kv );

        void undo();
        void redo();

        int id() const;

        bool mergeWith(const QUndoCommand *other);

    protected:
        void retranslate();

    private:
        static int id_;
};

/**
    \brief Change (single) node style

    This class is intended to be used for the node style dialog, never merges
*/
class ChangeCustomNodeStyle : public KnotViewUndoCommand
{
        Q_OBJECT

    Node *node;
    styleinfo style_old, style_new;

    public:
        ChangeCustomNodeStyle ( Node *node, styleinfo  style_new, KnotView* kv );

        void undo();
        void redo();

    protected:
        void retranslate();
};

/**
    \brief Change node styles
*/
class ChangeMultiNodeStyle : public KnotViewUndoCommand
{
        Q_OBJECT

    bool was_disabled;
    node_list nodes;
    QList<styleinfo> style_old;
    styleinfo style_new;

    public:
        ChangeMultiNodeStyle ( node_list nodes, styleinfo  style_new, KnotView* kv );

        void undo();
        void redo();

        int id() const;

        bool mergeWith(const QUndoCommand *other);

    protected:
        void retranslate();

    private:
        static int id_;
};

/**
    \brief Change knot width
*/
class ChangeKnotWidth : public KnotViewUndoCommand
{
        Q_OBJECT

    double old_width, new_width;
    bool accepted;

    public:
        ChangeKnotWidth ( double old_width, double new_width, KnotView* kv );
        ChangeKnotWidth ( double old_width, double new_width, bool accepted, KnotView* kv );

        void undo();
        void redo();

        int id() const;

        bool mergeWith(const QUndoCommand *other);

    protected:
        void retranslate();

    private:
        static int id_;
};
/**
    \brief Change knot color
*/
class ChangeKnotBrush : public KnotViewUndoCommand
{
        Q_OBJECT

    QBrush old_brush, new_brush;
    bool accepted;

    public:
        ChangeKnotBrush ( QBrush old_brush, QBrush new_brush, KnotView* kv );

        ChangeKnotBrush ( QBrush old_brush, QBrush new_brush, bool accepted, KnotView* kv );

        void undo();
        void redo();

        int id() const;

        bool mergeWith(const QUndoCommand *other);

        void retranslate();

    private:
        static int id_;
};
/**
    \brief Change knot outline
*/
class ChangeKnotPen : public KnotViewUndoCommand
{
        Q_OBJECT

    QPen old_pen, new_pen;

    public:
        ChangeKnotPen ( QPen old_pen, QPen new_pen, KnotView* kv );

        void undo();
        void redo();

        int id() const;

        bool mergeWith(const QUndoCommand *other);


    protected:
        void retranslate();

    private:
        static int id_;
};
/**
    \brief Change knot join style
*/
class ChangeKnotJoinStyle : public KnotViewUndoCommand
{
        Q_OBJECT

    Qt::PenJoinStyle old_pjs, new_pjs;

    public:
        ChangeKnotJoinStyle ( Qt::PenJoinStyle old_pjs, Qt::PenJoinStyle new_pjs, KnotView* kv );

        void undo();
        void redo();

    protected:
        void retranslate();
};


} // namespace commands

#endif // COMMANDS_HPP
