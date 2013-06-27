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
      view(view), graph(&view->m_graph), scene(view->scene())
{}

void Knot_Command::set_parent(Knot_Macro *macro)
{
    setParent(macro);
}

void Knot_Command::set_last_node(Node *n)
{
    view->tool_edge_chain.set_last_node(n);
}

void Knot_Command::update_knot() const
{
    if ( qobject_cast<Knot_Macro*>(parent()) == nullptr )
    {
        view->update_knot();
        view->update_selection(false);
    }
}

void Knot_Command::update_view() const
{
    if ( qobject_cast<Knot_Macro*>(parent()) == nullptr )
    {
        graph->update();
        scene->invalidate();
    }
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
    node->set_visible(graph_visible());
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
    update_knot();
}

void Create_Edge::redo()
{
    graph->add_edge(edge);
    scene->addItem(edge);
    edge->set_visible(graph_visible());
    update_knot();
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
    if ( edge->scene() == scene )
    {
        graph->remove_edge(edge);
        scene->removeItem(edge);
        update_knot();
    }
}

void Remove_Edge::undo()
{
    if ( edge->scene() != scene )
    {
        graph->add_edge(edge);
        scene->addItem(edge);
        edge->set_visible(graph_visible());
        update_knot();
    }
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
    update_view();
}

void Change_Colors::redo()
{
    graph->set_colors(after);
    update_view();
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


int Change_Borders::m_id = generate_id();
Change_Borders::Change_Borders(Border_List before, Border_List after, Knot_View *kv,
                             Knot_Macro* parent )
    : Knot_Command(kv,parent),before(before),after(after)
{
    setText(tr("Change Border"));
}

void Change_Borders::undo()
{
    graph->set_borders(before);
    update_view();
}

void Change_Borders::redo()
{
    graph->set_borders(after);
    update_view();
}

int Change_Borders::id() const
{
    return m_id;
}

bool Change_Borders::mergeWith(const QUndoCommand *other)
{
    const Change_Borders* cc = static_cast<const Change_Borders*>(other);
    if ( cc->graph == graph )
    {
        after = cc->after;
        return true;
    }
    return false;
}



Custom_Colors::Custom_Colors(bool before, bool after, Knot_View *kv, Knot_Macro *parent)
    : Knot_Command(kv,parent), before(before), after(after)
{
    if ( after )
        setText(tr("Custom Colors"));
    else
        setText(tr("Auto Color"));

}

void Custom_Colors::undo()
{
    graph->set_custom_colors(before);
    update_view();
}

void Custom_Colors::redo()
{
    graph->set_custom_colors(after);
    update_view();
}

Display_Border::Display_Border(bool before, bool after, Knot_View *kv, Knot_Macro *parent)
    : Knot_Command(kv,parent), before(before), after(after)
{
    if ( after )
        setText(tr("Display Borders"));
    else
        setText(tr("Hide Borders"));

}

void Display_Border::undo()
{
    graph->set_paint_border(before);
    update_view();
}

void Display_Border::redo()
{
    graph->set_paint_border(after);
    update_view();
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
    update_view();
}

void Knot_Width::redo()
{
    graph->set_width(after);
    update_view();
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
    scene->addItem(node);
    node->set_visible(graph_visible());
    update_knot();
}

void Remove_Node::redo()
{
    graph->remove_node(node);
    scene->removeItem(node);
    update_knot();
}


int Pen_Join_Style::m_id = generate_id();

Pen_Join_Style::Pen_Join_Style(Qt::PenJoinStyle before, Qt::PenJoinStyle after,
                               Knot_View *kv, Knot_Macro *parent)
    : Knot_Command(kv,parent), before(before), after(after)
{
    setText(tr("Change Joint Style"));
}

void Pen_Join_Style::undo()
{
    graph->set_join_style(before);
    update_view();
}

void Pen_Join_Style::redo()
{
    graph->set_join_style(after);
    update_view();
}

int Pen_Join_Style::id() const
{
    return m_id;
}

bool Pen_Join_Style::mergeWith(const QUndoCommand *other)
{
    const Pen_Join_Style* cc = static_cast<const Pen_Join_Style*>(other);
    if ( cc->graph == graph )
    {
        after = cc->after;
        return true;
    }
    return false;
}


int Brush_Style::m_id = generate_id();

Brush_Style::Brush_Style(Qt::BrushStyle before, Qt::BrushStyle after,
                               Knot_View *kv, Knot_Macro *parent)
    : Knot_Command(kv,parent), before(before), after(after)
{
    setText(tr("Change Brush Style"));
}

void Brush_Style::undo()
{
    graph->set_brush_style(before);
    update_view();
}

void Brush_Style::redo()
{
    graph->set_brush_style(after);
    update_view();
}

int Brush_Style::id() const
{
    return m_id;
}

bool Brush_Style::mergeWith(const QUndoCommand *other)
{
    const Brush_Style* cc = static_cast<const Brush_Style*>(other);
    if ( cc->graph == graph )
    {
        after = cc->after;
        return true;
    }
    return false;
}


Knot_Style_Basic_Double_Parameter::Knot_Style_Basic_Double_Parameter
        (double before, double after, Knot_View *kv, Knot_Macro *parent)
    : Knot_Command(kv,parent), before(before), after(after)
{}
void Knot_Style_Basic_Double_Parameter::undo()
{
    apply(before);
    update_knot();
}
void Knot_Style_Basic_Double_Parameter::redo()
{
    apply(after);
    update_knot();
}
bool Knot_Style_Basic_Double_Parameter::mergeWith(const QUndoCommand *other)
{
    const Knot_Style_Basic_Double_Parameter* cc = static_cast<const Knot_Style_Basic_Double_Parameter*>(other);
    if ( cc->graph == graph )
    {
        after = cc->after;
        return true;
    }
    return false;
}

int Knot_Style_Handle_Lenght::m_id = generate_id();
void Knot_Style_Handle_Lenght::apply(double value)
{
    graph->default_node_style_reference().handle_length = value;
}

int Knot_Style_Crossing_Distance::m_id = generate_id();
void Knot_Style_Crossing_Distance::apply(double value)
{
    graph->default_node_style_reference().crossing_distance = value;
}

int Knot_Style_Cusp_Angle::m_id = generate_id();
void Knot_Style_Cusp_Angle::apply(double value)
{
    graph->default_node_style_reference().cusp_angle = value;
}

int Knot_Style_Cusp_Distance::m_id = generate_id();
void Knot_Style_Cusp_Distance::apply(double value)
{
    graph->default_node_style_reference().cusp_distance = value;
}

int Knot_Style_Edge_Slide::m_id = generate_id();
void Knot_Style_Edge_Slide::apply(double value)
{
    graph->default_node_style_reference().edge_slide = value;
}


Knot_Style_Cusp_Shape::Knot_Style_Cusp_Shape(Cusp_Shape *before, Cusp_Shape *after,
                                             Knot_View *kv, Knot_Macro *parent)
    :Knot_Command(kv,parent), before(before), after(after)
{
    setText(tr("Change Cusp Shape"));
}

void Knot_Style_Cusp_Shape::undo()
{
    graph->default_node_style_reference().cusp_shape = before;
    update_knot();
}

void Knot_Style_Cusp_Shape::redo()
{
    graph->default_node_style_reference().cusp_shape = after;
    update_knot();
}





Node_Style_Base::Node_Style_Base(QList<Node *> nodes, Knot_View *kv, Knot_Macro *parent)
    : Knot_Command(kv,parent), nodes(nodes)
{
}

Node_Style_Basic_Double_Parameter::Node_Style_Basic_Double_Parameter
        (QList<Node *> nodes, QList<double> before, QList<double> after, Knot_View *kv, Knot_Macro *parent)
    : Node_Style_Base(nodes,kv,parent), before(before), after(after)
{}
void Node_Style_Basic_Double_Parameter::undo()
{
    for( int i = 0; i < nodes.size(); i++)
        apply(nodes[i], before[i]);
    update_knot();
}
void Node_Style_Basic_Double_Parameter::redo()
{
    for( int i = 0; i < nodes.size(); i++)
        apply(nodes[i], after[i]);
    update_knot();
}
bool Node_Style_Basic_Double_Parameter::mergeWith(const QUndoCommand *other)
{
    const Node_Style_Basic_Double_Parameter* cc =
            static_cast<const Node_Style_Basic_Double_Parameter*>(other);
    if ( cc->nodes == nodes &&
         cc->metaObject()->className() == metaObject()->className() )
    {
        after = cc->after;
        return true;
    }
    return false;
}


int Node_Style_Handle_Lenght::m_id = generate_id();
void Node_Style_Handle_Lenght::apply(Node* node, double value)
{
    node->style().handle_length = value;
}
int Node_Style_Cusp_Distance::m_id = generate_id();
void Node_Style_Cusp_Distance::apply(Node* node, double value)
{
    node->style().cusp_distance = value;
}
int Node_Style_Cusp_Angle::m_id = generate_id();
void Node_Style_Cusp_Angle::apply(Node* node, double value)
{
    node->style().cusp_angle = value;
}
int Node_Style_Crossing_Distance::m_id = generate_id();
void Node_Style_Crossing_Distance::apply(Node* node, double value)
{
    node->style().crossing_distance = value;
}


Node_Style_Cusp_Shape::Node_Style_Cusp_Shape(
        QList<Node *> nodes, QList<Cusp_Shape*> before, QList<Cusp_Shape*> after,
        Knot_View *kv,Knot_Macro *parent)
    :Node_Style_Base(nodes,kv,parent), before(before), after(after)
{
     setText(tr("Change Selection Cusp Shape"));
}
void Node_Style_Cusp_Shape::undo()
{
    for( int i = 0; i < nodes.size(); i++)
        nodes[i]->style().cusp_shape = before[i];
    update_knot();
}
void Node_Style_Cusp_Shape::redo()
{
    for( int i = 0; i < nodes.size(); i++)
        nodes[i]->style().cusp_shape = after[i];
    update_knot();
}


Node_Style_Enable::Node_Style_Enable(
        QList<Node *> nodes, QList<Knot_Style::Enabled_Styles> before,
        QList<Knot_Style::Enabled_Styles> after,
        Knot_View *kv, Knot_Macro *parent)
    :Node_Style_Base(nodes,kv,parent), before(before), after(after)
{
    setText(tr("Toggle Selection Style Property"));
}

Node_Style_Enable::Node_Style_Enable(Node *node,
                                     Knot_Style::Enabled_Styles before,
                                     Knot_Style::Enabled_Styles after,
                                     QString text,
                                     Knot_View *kv, Knot_Macro *parent)
    : Node_Style_Base(QList<Node*>()<<node,kv,parent),
      before(QList<Knot_Style::Enabled_Styles>()<<before),
      after(QList<Knot_Style::Enabled_Styles>()<<after)
{
    setText(text);
}

void Node_Style_Enable::undo()
{
    for( int i = 0; i < nodes.size(); i++)
        nodes[i]->style().enabled_style = before[i];
    update_knot();
}
void Node_Style_Enable::redo()
{
    for( int i = 0; i < nodes.size(); i++)
        nodes[i]->style().enabled_style = after[i];
    update_knot();
}


Knot_Style_All::Knot_Style_All(Knot_Style before, Knot_Style after, Knot_View *kv, Knot_Macro *parent)
    : Knot_Command(kv,parent), before(before), after(after)
{}

void Knot_Style_All::undo()
{
    graph->set_default_node_style(before);
    update_knot();
}

void Knot_Style_All::redo()
{
    graph->set_default_node_style(after);
    update_knot();
}


Node_Style_All::Node_Style_All(Node *node, Knot_Style before, Knot_Style after,
                               Knot_View *kv, Knot_Macro *parent)
    : Knot_Command(kv,parent), before(before), after(after), node(node)
{
    setText(tr("Change Node Style"));
}

void Node_Style_All::undo()
{
    node->set_style(before);
    update_knot();
}

void Node_Style_All::redo()
{
    node->set_style(after);
    update_knot();
}


int Change_Edge_Type::m_id = generate_id();
Change_Edge_Type::Change_Edge_Type(Edge *edge, Edge_Type *before, Edge_Type *after,
                                     Knot_View *kv, Knot_Macro *parent)
    :Knot_Command(kv,parent), edge(edge), before(before), after(after)
{
    setText(tr("Change Edge Type"));
}

void Change_Edge_Type::undo()
{
    Knot_Style es = edge->style();
    es.edge_type = before;
    edge->set_style(es);
    update_knot();
}

void Change_Edge_Type::redo()
{
    Knot_Style es = edge->style();
    es.edge_type = after;
    edge->set_style(es);
    update_knot();
}

bool Change_Edge_Type::mergeWith(const QUndoCommand *other)
{
    const Change_Edge_Type* cc = static_cast<const Change_Edge_Type*>(other);
    if ( cc->edge == edge )
    {
        after = cc->after;
        return true;
    }
    return false;
}


int Knot_Insert_Macro::m_id = generate_id();
bool Knot_Insert_Macro::mergeWith(const QUndoCommand *other)
{

    const Knot_Insert_Macro* cc = static_cast<const Knot_Insert_Macro*>(other);
    if ( cc->mergeable )
    {
        return true;
    }
    return false;
}
