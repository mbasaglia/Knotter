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

#include "script_document.hpp"
#include "commands.hpp"

Script_Document::Script_Document(Knot_View *wrapped, QObject *parent) :
    QObject(parent), wrapped(wrapped), m_graph(wrapped->graph()),
    m_grid(&wrapped->grid()), macro_count(0)
{
    connect(&m_graph,SIGNAL(edge_added(Script_Edge*)),SLOT(add_edge(Script_Edge*)));
    connect(&m_graph,SIGNAL(node_added(Script_Node*)),SLOT(add_node(Script_Node*)));
    connect(&m_graph,SIGNAL(node_moved(Script_Node*,Script_Point)),
            SLOT(move_node(Script_Node*,Script_Point)));
    connect(&m_graph,SIGNAL(edge_type_changed(Script_Edge*,Edge_Style*)),
            SLOT(change_edge_type(Script_Edge*,Edge_Style*)));
    connect(&m_graph,SIGNAL(node_removed(Script_Node*)),SLOT(remove_node(Script_Node*)));
    connect(&m_graph,SIGNAL(edge_removed(Script_Edge*)),SLOT(remove_edge(Script_Edge*)));
}

Script_Document::~Script_Document()
{
    clean_macros();
}

QString Script_Document::filename() const
{
    return wrapped->file_name();
}

Script_Graph *Script_Document::graph()
{
    return &m_graph;
}

QObject *Script_Document::grid()
{
    return &m_grid;
}

bool Script_Document::insert(const Script_Graph &graph, QString message)
{
    Graph* g = graph.create_graph();
    bool b = wrapped->insert(*g,message.isEmpty()?tr("Script Insert"):message);
    delete g;
    return b;
}

QString Script_Document::toString() const
{
    QString file = filename();
    if ( !file.isEmpty() )
        file = ' '+file;
    return QString("[Knot%1]").arg(file);
}

void Script_Document::add_node(Script_Node *n)
{
    wrapped->push_command(new Create_Node(n->wrapped_node(),wrapped));
}

void Script_Document::remove_node(Script_Node *n)
{
    wrapped->remove_node(n->wrapped_node());
}

void Script_Document::remove_edge(Script_Edge *e)
{
    wrapped->remove_edge(e->wrapped_edge());
}

void Script_Document::add_edge(Script_Edge *e)
{
    wrapped->push_command(new Create_Edge(e->wrapped_edge(),wrapped));
}

void Script_Document::move_node(Script_Node *n, Script_Point p)
{
    Node* real = n->wrapped_node();
    wrapped->push_command(new Move_Node(real,real->pos(),p,wrapped));
}

void Script_Document::change_edge_type(Script_Edge *edge, Edge_Style *type)
{
    Edge* real = edge->wrapped_edge();
    wrapped->push_command(new Change_Edge_Style(real,real->style(),type,wrapped));
}

void Script_Document::begin_macro(QString message)
{
    wrapped->begin_macro(message);
    macro_count++;
}

void Script_Document::end_macro()
{
    wrapped->end_macro();
    macro_count--;
}

void Script_Document::clean_macros()
{
    while(macro_count > 0)
        end_macro();
}
