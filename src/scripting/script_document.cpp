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
    QObject(parent), wrapped(wrapped),m_graph(wrapped->graph())
{
    connect(&m_graph,SIGNAL(edge_added(Script_Edge*)),SLOT(add_edge(Script_Edge*)));
    connect(&m_graph,SIGNAL(node_added(Script_Node*)),SLOT(add_node(Script_Node*)));
    connect(&m_graph,SIGNAL(node_moved(Script_Node*,Script_Point)),
            SLOT(move_node(Script_Node*,Script_Point)));
}

QString Script_Document::filename() const
{
    return wrapped->file_name();
}

Script_Graph *Script_Document::graph()
{
    return &m_graph;
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

void Script_Document::add_edge(Script_Edge *e)
{
    wrapped->add_edge(e->vertex1()->wrapped_node(), e->vertex2()->wrapped_node() );
}

void Script_Document::move_node(Script_Node *n, Script_Point p)
{
    Node* real = n->wrapped_node();
    wrapped->push_command(new Move_Node(real,real->pos(),p,wrapped));
}

void Script_Document::update()
{
    m_graph.from_graph(wrapped->graph());
}
