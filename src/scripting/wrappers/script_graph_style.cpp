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

#include "script_graph_style.hpp"
#include "script_graph.hpp"

Script_Graph_Style::Script_Graph_Style(Script_Graph *owner,
                                       Edge_Style crossing_style,
                                       Node_Style cusp_style,
                                       QList<QColor> colors )
    : owner(owner), m_crossing_style(crossing_style), m_cusp_style(cusp_style)
{
    connect(&m_crossing_style,SIGNAL(changed(Edge_Style,Edge_Style)),
            SLOT(emit_style_changed(Edge_Style,Edge_Style)));
    connect(&m_cusp_style,SIGNAL(changed(Node_Style,Node_Style)),
            SLOT(emit_style_changed(Node_Style,Node_Style)));

    foreach(QColor c, colors)
        add_color(c);

}

Script_Graph_Style::Script_Graph_Style(Script_Graph *owner, const Script_Graph_Style &copy)
    : owner(owner), m_crossing_style(copy.m_crossing_style.style()),
      m_cusp_style(copy.m_cusp_style.style())
{
    connect(&m_crossing_style,SIGNAL(changed(Edge_Style,Edge_Style)),
            SLOT(emit_style_changed(Edge_Style,Edge_Style)));
    connect(&m_cusp_style,SIGNAL(changed(Node_Style,Node_Style)),
            SLOT(emit_style_changed(Node_Style,Node_Style)));

    foreach(Script_Color* c, copy.m_colors)
        add_color(*c);
}



void Script_Graph_Style::add_color(QColor c)
{
    Script_Color* cc = new Script_Color(c,this);
    m_colors << cc;
    connect(cc,SIGNAL(changed(QColor)),SIGNAL(colors_changed()));
}


void Script_Graph_Style::set_crossing_style(QObject *object)
{
    Script_Edge_Style* style = qobject_cast<Script_Edge_Style*>(object);
    if ( style )
        m_crossing_style.from_style(style->style());
}

void Script_Graph_Style::set_cusp_style(QObject *object)
{
    Script_Node_Style* style = qobject_cast<Script_Node_Style*>(object);
    if ( style )
        m_cusp_style.from_style(style->style());
}

QObjectList Script_Graph_Style::colors()
{
    QObjectList l;
    foreach(Script_Color* c, m_colors)
        l << c;
    return l;
}

QList<QColor> Script_Graph_Style::internal_colors() const
{
    QList<QColor> cl;
    foreach(Script_Color* c, m_colors)
        cl << *c;
    return cl;
}


void Script_Graph_Style::emit_style_changed(Edge_Style before, Edge_Style after)
{
    Node_Style ns_before = m_cusp_style.style();
    m_cusp_style.blockSignals(true);
    m_cusp_style.set_handle_length(after.handle_length);
    m_cusp_style.blockSignals(false);
    emit style_changed(ns_before,before,m_cusp_style.style(),after);
}


void Script_Graph_Style::emit_style_changed(Node_Style before, Node_Style after)
{
    Edge_Style es_before = m_crossing_style.style();
    m_crossing_style.blockSignals(true);
    m_crossing_style.set_handle_length(after.handle_length);
    m_crossing_style.blockSignals(false);
    emit style_changed(before,es_before,after,m_crossing_style.style());
}
