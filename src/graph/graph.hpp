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
#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <QObject>
#include "node.hpp"
#include "edge.hpp"
#include <QPainter>
#include "path_builder.hpp"
#include "traversal_info.hpp"

class Graph : public QObject, public QAbstractGraphicsShapeItem
{
    Q_OBJECT
    Q_FLAGS(Paint_Mode_Enum Paint_Mode)
public:
    enum Paint_Mode_Enum {
        Paint_Graph = 0x01, ///< renders the graph
        Paint_Knot  = 0x02  ///< renders the knot
    };
    Q_DECLARE_FLAGS(Paint_Mode,Paint_Mode_Enum)

private:
    QList<Node*>        nodes;
    QList<Edge*>        edges;
    Node_Style          default_node_style;
    QRectF              bounding_box;
    Paint_Mode          paint_mode;
    QList<QColor>       colors;     ///< \todo
    bool                auto_color; ///< \todo
    QList<QPainterPath> paths;    ///< Rendered knot (one per loop)

public:
    explicit Graph(QObject *parent = 0);

    /**
     *  Add node to graph
     *  \param n Node to be added
     *  \pre n is not already in the graph
     *  \post n is in the graph
     *  \note Signals from n are connected to this graph
     */
    void add_node(Node* n);
    /**
     *  Add edge to graph
     *  \param e Edge to be added
     *  \pre e is not already in the graph
     *  \post e is in the graph
     */
    void add_edge(Edge* e);
    /**
     *  Remove node from graph
     *  \param n Node to be removed
     *  \pre n is in the graph and has no connected edges
     *  \post n is not in the graph
     */
    void remove_node(Node* n);
    /**
     *  Remove edge from graph
     *  \param e Edge to be removed
     *  \pre e is in the graph
     *  \post e is not in the graph
     */
    void remove_edge(Edge* e);

    /**
     *  \brief Set the paint mode to the given paint mode
    */
    void set_paint_mode( Paint_Mode mode );
    /**
     *  \brief Toggles the given flag from the paint mode
     *
     *  Performs paint_mode XOR flag
    */
    void toggle_paint_flag ( Paint_Mode_Enum flag );
    /**
     *  \brief Enables the given flag in the paint mode
     *
     *  Performs paint_mode OR flag
    */
    void enable_paint_flag ( Paint_Mode_Enum flag );
    /**
     *  \brief Disables the given flag from the paint mode
     *
     *  Performs paint_mode NAND flag
    */
    void disable_paint_flag ( Paint_Mode_Enum flag );

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option=0, QWidget *widget=0);
    QRectF boundingRect() const override { return bounding_box; }
    int type() const override { return UserType+0x03; }


public slots:

    /// Traverse graph and update internal painter paths
    void render_knot();
private:

    void draw_segment( Path_Builder& path, const Traversal_Info& ti ) const;

    /// Traverse the entire graph
    void traverse(Path_Builder& path);

    /** Mark source and destionation handles as traversed,
     * get proper vertices and render
     */
    Traversal_Info traverse(Edge *edge, Edge::Handle handle,
                            Path_Builder& path);

    void update_bounding_box();
    
signals:
    /// Emitted when nodes or edges have changed and requires redrawing
    void graph_changed();
    /// Emitted when style is changed and requires redrawing but not re-traversing
    void style_changed();
    
};

#endif // GRAPH_HPP
