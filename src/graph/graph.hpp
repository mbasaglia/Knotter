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

/**
 *  \brief Class that represents the knot (as a graph) and renders it
 *
 *  \note Never takes ownership of edges and nodes
 */
class Graph : public QGraphicsItem
{
    Q_FLAGS(Paint_Mode_Enum Paint_Mode)
public:
    enum Paint_Mode_Enum {
        PAINT_GRAPH = 0x01, ///< renders the graph
        PAINT_KNOT  = 0x02  ///< renders the knot
    };
    Q_DECLARE_FLAGS(Paint_Mode,Paint_Mode_Enum)

private:
    QList<Node*>        m_nodes;
    QList<Edge*>        m_edges;
    Node_Style          m_default_node_style;
    QRectF              bounding_box;
    Paint_Mode          m_paint_mode;
    QList<QColor>       m_colors;
    bool                auto_color;
    QList<QPainterPath> paths;    ///< Rendered knot (one per loop)
    QPen                pen;

public:
    explicit Graph();
    Graph(const Graph& other);
    Graph& operator= (const Graph& other);

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

    /*/// Remove all edges and nodes from the graph
    void clear();*/

    QList<Node*> nodes() const { return m_nodes; }
    QList<Edge*> edges() const { return m_edges; }

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

    /// Get paint mode
    Paint_Mode paint_mode() const { return m_paint_mode; }

    const QList<QColor>& colors() const { return m_colors; }
    void set_colors(const QList<QColor>& l);

    /// get stroke width
    double width() const { return pen.widthF();}
    /// set stroke width
    void set_width(double w);

    Qt::PenJoinStyle join_style() const { return pen.joinStyle(); }
    void set_join_style ( Qt::PenJoinStyle style );

    Qt::BrushStyle brush_style() const;
    void set_brush_style(Qt::BrushStyle);

    bool custom_colors() const { return !auto_color; }
    void set_custom_colors(bool b) { auto_color = !b; }


    Node_Style default_node_style() const { return m_default_node_style; }
    Node_Style& default_node_style_reference() { return m_default_node_style; }
    void set_default_node_style( Node_Style style );

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option=nullptr,
               QWidget *widget=nullptr);
    void const_paint(QPainter *painter,
                     const QStyleOptionGraphicsItem *option=nullptr,
                     QWidget *widget=nullptr, Paint_Mode mode = PAINT_KNOT ) const;
    void const_paint(QPainter *painter, Paint_Mode mode) const;
    QRectF boundingRect() const override { return bounding_box; }
    int type() const override { return UserType+0x03; }


    /// Traverse graph and update internal painter paths
    void render_knot();

    /**
     *  \brief Get a subgraph
     *
     *  \returns A graph with the same settings as this but containing only the
     *           nodes in the list
     *
     *  \note This will copy the pointers to nodes/edges so any change to them will
     *          occur on both graphs.
     */
    Graph sub_graph(QList<Node*> nodes) const;


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
    
};

#endif // GRAPH_HPP
