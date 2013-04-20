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

#ifndef KNOT_VIEW_HPP
#define KNOT_VIEW_HPP

#include <QGraphicsView>
#include "graph.hpp"
#include <QUndoStack>
#include "snapping_grid.hpp"

class Knot_View : public QGraphicsView
{
    Q_OBJECT

    friend class Knot_Command;

    enum Mouse_Mode_Enum
    {
        EDIT_GRAPH = 0x001, ///< Move and select
        EDGE_CHAIN = 0x002, ///< Keep inserting connected node, RMB selects
        MOVE_GRID  = 0x110, ///< \todo Move grid origin
        MOVE_BG_IMG= 0x120, ///< \todo Move background image
        MOVE_BACK  = 0x100, ///< Moving either grid or image
        MOVE_BACK_M= MOVE_GRID|MOVE_BG_IMG ///< Mask with both MOVE_GRID and MOVE_BG_IMG
    };
    Q_DECLARE_FLAGS(Mouse_Mode,Mouse_Mode_Enum)


    QPoint              move_center; ///< Point aligned to the cursor during movement
    Graph               graph;
    QUndoStack          undo_stack;
    Snapping_Grid       grid;
    Mouse_Mode          mouse_mode;
    Node*               last_node;   ///< Last node in a chain
    QGraphicsLineItem   guide;       ///< Tiny line showing the edge being edited
    QGraphicsRectItem   rubberband;  ///< Draggable selection rectangle

public:

    /**
     *  \param file File name, if empty no file is loaded
    */
    Knot_View ( QString file );

    QUndoStack* undo_stack_pointer() { return &undo_stack; }

    /// Overload QGraphicsView::translate
    void translate(QPointF d) { QGraphicsView::translate(d.x(),d.y()); }

    /**
     *  \brief Translate and resize sceneRect
     *
     *  Translate the scene, if the result is not contained within sceneRect,
     *  the sceneRect is expanded
     */
    void translate_view(QPointF delta);

    /**
     *  \brief Creates a node in the given location
     *
     *  \return The created node
    */
    Node* add_node(QPointF pos);

    /**
     *  \brief Creates an edge connecting the given nodes
     *
     *  \pre n1 and n2 must be in the scene and in the graph
     *
     *  \return The newly created edge
     */
    Edge* add_edge(Node*n1,Node*n2);

    /**
     *  \brief Get the grobal zoom factor
     *
     *  \return A value representing the scaling factor, 1 = 100%
    */
    double get_zoom_factor() const { return transform().m11(); }

public slots:
    /**
     * \brief Zoom view by factor
     *
     *  The zooming is performed relative to the current transformation
     *
     *  \param factor scaling factor ( 1 = don't zoom )
     */
    void zoom_view(double factor);
    /**
     * \brief Set zoom factor
     *
     *  The zooming is performed absolutely
     *
     *  \param factor scaling factor ( 1 = no zoom )
     */
    void set_zoom(double factor);

    /**
     *  Sets mouse mode to edit graph
     */
    void set_mode_edit_graph();

    /**
     *  Sets mouse mode to edge chain
     */
    void set_mode_edge_chain();

signals:

    /// emitted when the graph structure or style is changed
    void graph_changed();

    /**
     *  Emit when zoom is changed
     *  \param percent Zoom percentage
     */
    void zoomed(double percent);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

    /**
     *  \brief Expand sceneRect to contain the visible area
     *  \param margin optional margin to grow the visible area
    */
    void expand_scene_rect(int margin=0);
    
private slots:
    /// Expand scene rect when scrollbars reach margin
    void update_scrollbars() { expand_scene_rect(2); }

private:
    /**
     *  \brief Get node at location
     *  \return The found node or NULL
    */
    Node* node_at(QPointF p) const;

};

#endif // KNOT_VIEW_HPP
