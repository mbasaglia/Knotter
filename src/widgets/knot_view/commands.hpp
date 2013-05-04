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

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <QUndoCommand>
#include "knot_view.hpp"
#include "graph.hpp"

// basic

class Knot_Macro;

class Knot_Command : public QObject, public QUndoCommand
{
    Q_OBJECT

private:
    static int auto_id;
protected:
    static int generate_id();
    Knot_View*      view;
    Graph*          graph;
    QGraphicsScene* scene;

    /**
     *  Adjust view->last_node and view->guide
     */
    void set_last_node(Node* n);

    /**
     *  Update knot only if has no parent
     */
    void update_knot() const;

public:
    Knot_Command(Knot_View* view, Knot_Macro* parent );

    void set_parent(Knot_Macro* macro);

};

/**
 *  \brief Smart macro that updates the view after all the children have been executed
 *
 *  \note Uses QObject children system as the one for QUndoCommand is too limited
 */
class Knot_Macro : public Knot_Command
{
    Q_OBJECT

public:
    Knot_Macro(QString text,Knot_View*kv, Knot_Macro* parent = nullptr)
        : Knot_Command(kv,parent)
    {
        setText(text);
    }
    void undo() override;
    void redo() override;
};

// graph editing
class Create_Node : public Knot_Command
{
    Q_OBJECT

    Node*          node;
public:
    Create_Node(Node* node, Knot_View* kv, Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
    ~Create_Node();
};

class Create_Edge : public Knot_Command
{
    Q_OBJECT

    Edge*          edge;
public:
    Create_Edge(Edge* edge, Knot_View* kv, Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
    ~Create_Edge();
};

/**
 *  Used to change view->last_node
 */
class Last_Node : public Knot_Command
{
    Q_OBJECT

    Node* node_before;
    Node* node_after;
public:
    Last_Node(Node* node_before, Node* node_after, Knot_View* kv,
              Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;

};

class Remove_Edge : public Knot_Command
{
    Q_OBJECT

    Edge*          edge;
public:
    Remove_Edge(Edge* edge, Knot_View* kv, Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
};


class Remove_Node : public Knot_Command
{
    Q_OBJECT

    Node* node;
public:
    Remove_Node(Node* node, Knot_View* kv, Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
};

class Move_Node : public Knot_Command
{
    Q_OBJECT

    Node*   node;
    QPointF before;
    QPointF after;
public:
    Move_Node(Node* node, QPointF before, QPointF after, Knot_View* kv,
              Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
};

// knot display
class Change_Colors : public Knot_Command
{
    Q_OBJECT
    static int m_id;
    QList<QColor> before;
    QList<QColor> after;
public:
    Change_Colors(QList<QColor> before, QList<QColor> after, Knot_View* kv,
                  Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;
};


class Knot_Width : public Knot_Command
{
    Q_OBJECT

    static int m_id;

    double before;
    double after;

public:
    Knot_Width(double before, double after, Knot_View* kv, Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;
};

class Pen_Join_Style : public Knot_Command
{
    Q_OBJECT

    static int m_id;

    Qt::PenJoinStyle before;
    Qt::PenJoinStyle after;

public:
    Pen_Join_Style(Qt::PenJoinStyle before, Qt::PenJoinStyle after, Knot_View* kv,
               Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
    int id() const override;
    bool mergeWith(const QUndoCommand *other) override;
};

class Custom_Colors : public Knot_Command
{
    Q_OBJECT


    bool before;
    bool after;

public:
    Custom_Colors(bool before, bool after, Knot_View* kv, Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
};

// knot style
/**
 *  \brief Base class for Knot style commands
 *
 *  Simplifies the implementation of similar commands
 */
class Knot_Style_Basic_Double_Parameter : public Knot_Command
{
    Q_OBJECT

    double before;
    double after;

protected:
    /// Apply the value to the style
    virtual void apply(double value) = 0;

public:
    Knot_Style_Basic_Double_Parameter(double before, double after, Knot_View* kv,
               Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand *other) override;
};

class Knot_Style_Handle_Lenght : public Knot_Style_Basic_Double_Parameter
{
    Q_OBJECT

    static int m_id;

public:
    Knot_Style_Handle_Lenght(double before, double after, Knot_View* kv,
               Knot_Macro* parent = nullptr)
        : Knot_Style_Basic_Double_Parameter(before,after,kv,parent)
    { setText(tr("Change Curve Control")); }

    void apply(double value) override;
    int id() const override { return m_id; }

};

class Knot_Style_Crossing_Distance : public Knot_Style_Basic_Double_Parameter
{
    Q_OBJECT

    static int m_id;

public:
    Knot_Style_Crossing_Distance(double before, double after, Knot_View* kv,
               Knot_Macro* parent = nullptr)
        : Knot_Style_Basic_Double_Parameter(before,after,kv,parent)
    { setText(tr("Change Crossing Gap")); }

    void apply(double value) override;
    int id() const override { return m_id; }

};

class Knot_Style_Cusp_Angle : public Knot_Style_Basic_Double_Parameter
{
    Q_OBJECT

    static int m_id;

public:
    Knot_Style_Cusp_Angle(double before, double after, Knot_View* kv,
               Knot_Macro* parent = nullptr)
        : Knot_Style_Basic_Double_Parameter(before,after,kv,parent)
    { setText(tr("Change Minimum Cusp Angle")); }

    void apply(double value) override;
    int id() const override { return m_id; }

};

class Knot_Style_Cusp_Distance : public Knot_Style_Basic_Double_Parameter
{
    Q_OBJECT

    static int m_id;

public:
    Knot_Style_Cusp_Distance(double before, double after, Knot_View* kv,
               Knot_Macro* parent = nullptr)
        : Knot_Style_Basic_Double_Parameter(before,after,kv,parent)
    { setText(tr("Change Cusp Distance")); }

    void apply(double value) override;
    int id() const override { return m_id; }

};


class Knot_Style_Cusp_Shape : public Knot_Command
{
    Q_OBJECT

    Cusp_Shape* before;
    Cusp_Shape* after;

public:
    Knot_Style_Cusp_Shape(Cusp_Shape* before, Cusp_Shape* after, Knot_View* kv,
               Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
};


// selection style

class Node_Style_Base : public Knot_Command
{
    Q_OBJECT

protected:
    QList<Node*> nodes;

public:
    Node_Style_Base(QList<Node*> nodes, Knot_View* kv,
               Knot_Macro* parent = nullptr);
};
/**
 *  \brief Base class for selection style commands
 *
 *  Simplifies the implementation of similar commands
 */
class Node_Style_Basic_Double_Parameter : public Node_Style_Base
{
    Q_OBJECT

    QList<double> before;
    QList<double> after;

protected:

    /// Apply the value to the style
    virtual void apply(Node* node, double value) = 0;

public:
    Node_Style_Basic_Double_Parameter(QList<Node*> nodes, QList<double> before,
                                      QList<double> after, Knot_View* kv,
               Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
    bool mergeWith(const QUndoCommand *other) override;
};


class Node_Style_Handle_Lenght : public Node_Style_Basic_Double_Parameter
{
    Q_OBJECT

    static int m_id;

public:
    Node_Style_Handle_Lenght(
            QList<Node*> nodes, QList<double> before, QList<double> after,
            Knot_View* kv, Knot_Macro* parent = nullptr)
        : Node_Style_Basic_Double_Parameter(nodes, before,after,kv,parent)
    { }

    void apply(Node* node, double value) override;
    int id() const override { return m_id; }

};
class Node_Style_Cusp_Distance : public Node_Style_Basic_Double_Parameter
{
    Q_OBJECT

    static int m_id;

public:
    Node_Style_Cusp_Distance(
            QList<Node*> nodes, QList<double> before, QList<double> after,
            Knot_View* kv, Knot_Macro* parent = nullptr)
        : Node_Style_Basic_Double_Parameter(nodes, before,after,kv,parent)
    { }

    void apply(Node* node, double value) override;
    int id() const override { return m_id; }

};
class Node_Style_Cusp_Angle : public Node_Style_Basic_Double_Parameter
{
    Q_OBJECT

    static int m_id;

public:
    Node_Style_Cusp_Angle(
            QList<Node*> nodes, QList<double> before, QList<double> after,
            Knot_View* kv, Knot_Macro* parent = nullptr)
        : Node_Style_Basic_Double_Parameter(nodes, before,after,kv,parent)
    { }

    void apply(Node* node, double value) override;
    int id() const override { return m_id; }

};
class Node_Style_Crossing_Distance : public Node_Style_Basic_Double_Parameter
{
    Q_OBJECT

    static int m_id;

public:
    Node_Style_Crossing_Distance(
            QList<Node*> nodes, QList<double> before, QList<double> after,
            Knot_View* kv, Knot_Macro* parent = nullptr)
        : Node_Style_Basic_Double_Parameter(nodes, before,after,kv,parent)
    { }

    void apply(Node* node, double value) override;
    int id() const override { return m_id; }

};

class Node_Style_Cusp_Shape : public Node_Style_Base
{
    Q_OBJECT

    QList<Cusp_Shape*> before;
    QList<Cusp_Shape*> after;

public:
    Node_Style_Cusp_Shape(
            QList<Node*> nodes, QList<Cusp_Shape*> before, QList<Cusp_Shape*> after,
            Knot_View* kv, Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
};

class Node_Style_Enable : public Node_Style_Base
{
    Q_OBJECT

    QList<Node_Style::Enabled_Styles> before;
    QList<Node_Style::Enabled_Styles> after;

public:
    Node_Style_Enable(QList<Node*> nodes,
                      QList<Node_Style::Enabled_Styles> before,
                      QList<Node_Style::Enabled_Styles> after,
                      Knot_View* kv, Knot_Macro* parent = nullptr);
    void undo() override;
    void redo() override;
};


// node style
#endif // COMMANDS_HPP
