/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#ifndef SCRIPT_GRAPH_STYLE_HPP
#define SCRIPT_GRAPH_STYLE_HPP

#include "script_node_style.hpp"
#include "script_edge_style.hpp"
#include "script_color.hpp"


/**
 * \brief Graph Style class
 *
 *  Reflects the /graph/style element in XML
 */
class Script_Graph_Style : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject* crossing READ crossing_style WRITE set_crossing_style )
    Q_PROPERTY(QObject* cusp READ cusp_style WRITE set_cusp_style )
    Q_PROPERTY(QObjectList colors READ colors)

    //friend class Script_Graph;

    Script_Edge_Style m_crossing_style;
    Script_Node_Style m_cusp_style;
    QList<Script_Color*> m_colors;

public:
    Script_Graph_Style(Edge_Style crossing_style,
                       Node_Style cusp_style, QList<QColor> colors);
    Script_Graph_Style(const Script_Graph_Style &copy);

    /**
     * \brief copy style from graph
     * \param copy Graph to copy from
     */
    void copy_style(const class Graph &copy);

    void set_crossing_style(QObject* style);
    QObject* crossing_style() { return &m_crossing_style; }
    void set_cusp_style(QObject* style);
    QObject* cusp_style () { return &m_cusp_style; }
    QObjectList colors();
    QList<QColor> internal_colors() const;

private slots:
    void emit_style_changed(Node_Style before,Node_Style after);
    void emit_style_changed(Edge_Style before,Edge_Style after);

signals:
    void style_changed(Node_Style bef_node, Edge_Style bef_edge,
                       Node_Style aft_node, Edge_Style aft_edge );
    void colors_changed();

private:
    void add_color(QColor c);
};

#endif // SCRIPT_GRAPH_STYLE_HPP
