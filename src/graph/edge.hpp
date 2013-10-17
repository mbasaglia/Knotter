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
#ifndef EDGE_HPP
#define EDGE_HPP

#include "graph_item.hpp"
#include "node.hpp"
#include <QLineF>
#include "c++.hpp"
#include "edge_style.hpp"

class Edge : public Graph_Item
{
    Q_OBJECT

public:

    /// Color used to display the edge (resting)
    static QColor color_resting;
    /// Color used to display the edge (highlighted)
    static QColor color_highlighted;
    /// Color used to display the edge (selected outline)
    static QColor color_selected;

    enum Handle
    {
        NO_HANDLE   = 0x00,
        TOP_LEFT    = 0x01,
        TOP_RIGHT   = 0x02,
        BOTTOM_LEFT = 0x04,
        BOTTOM_RIGHT= 0x08
    };

    Q_DECLARE_FLAGS(Handle_Flags,Handle)

private:
    Node* v1;
    Node* v2;
    Edge_Style m_style;
    Handle_Flags available_handles;
    const Graph* m_graph;

    static const int shapew = 8; ///< Width ued for shape()
public:
    explicit Edge(Node* v1, Node* v2, Edge_Type *type = nullptr);

    void set_graph(const Graph* g) { m_graph = g; }
    const Graph* graph() const { return m_graph; }

    /// Whether node is one of its vetices
    bool is_vertex ( const Node* node ) const
    {
        return node == v1 or node == v2;
    }


    Node* vertex1() const
    {
        return v1;
    }

    Node* vertex2() const
    {
        return v2;
    }

    /**
     * Get other end node
     *
     * \pre n = vertex1 or n = vertex2
     *
     * Return other node or NULL on failure
     */
    Node* other(const Node* n) const
    {
        return n == v1 ? v2 : ( n == v2 ? v1 : nullptr );
    }

    /**
     * \brief set the edge styles
     *
     * If edge_type is \c nullptr the default style is used instead
     */
    void set_style(Edge_Style st);
    /**
     * @brief Get the crossing style as defined by theis edge
     * @return The style overidden by this edge
     */
    Edge_Style style() const;
    /**
     * @brief Get the crossing style as defined by theis edge
     * @return The style overidden by this edge
     */
    Edge_Style& style();

    /**
     * @brief Defaulted edge style
     * @return The complete edge style, non-overridden features are taken from the graph
     */
    Edge_Style defaulted_style() const;

    QLineF to_line() const { return QLineF(v1->pos(), v2->pos()); }

    /// Get the point in the middle of the edge
    QPointF midpoint() const { return (v1->pos()+v2->pos())/2; }

    /**
     *  Find closest point within the edge
     */
    QPointF snap(QPointF p) const { return project(p,to_line()); }

    //double distance_squared(QPointF to) const override;

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem* =0, QWidget* =0) override;
    int type() const override { return UserType + 0x02; }


    /// Mark all handles as untraversed
    void reset()
    {
        available_handles |= Handle_Flags(TOP_LEFT)|TOP_RIGHT|BOTTOM_LEFT|BOTTOM_RIGHT;
    }

    void mark_traversed(Handle h)
    {
        available_handles &=~ h;
    }

    /// Check if handle has been traversed
    bool traversed(Handle handle) const
    {
        return !(available_handles & handle);
    }

    /// Get a 'random' non-traversed handle
    Handle not_traversed() const
    {
        if ( available_handles & TOP_LEFT )
            return TOP_LEFT;
        else if ( available_handles & TOP_RIGHT )
            return TOP_RIGHT;
        else if ( available_handles & BOTTOM_LEFT )
            return BOTTOM_LEFT;
        else if ( available_handles & BOTTOM_RIGHT )
            return BOTTOM_RIGHT;
        else
            return NO_HANDLE;
    }

    /// Get node in the direction of the handle
    Node* vertex_for(Handle handle) const
    {
        return ( handle == TOP_RIGHT || handle == BOTTOM_RIGHT ) ? v2 : v1;
    }

public slots:
    /**
     *  Make vertices unaware of the edge
     *
     *  Removes this edge from the list of edges of both vertices
     */
    void detach();

    /**
     *  Make vertices aware of the edge
     *
     *  Add this edge to the list of edges of both vertices
     */
    void attach();
    
};

#endif // EDGE_HPP
