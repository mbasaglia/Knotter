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

class Knot_View : public QGraphicsView
{
    Q_OBJECT

    friend class Knot_Command;

    QPoint      move_center;            ///< Point aligned to the cursor during movement
    Graph       graph;
    QUndoStack  undo_stack;

public:

    /**
     *  \param file File name, if empty no file is loaded
    */
    Knot_View ( QString file );

    /// Overload QGraphicsView::translate
    void translate(QPointF d) { QGraphicsView::translate(d.x(),d.y()); }

    /**
     *  \brief Translate and resize sceneRect
     *
     *  Translate the scene, if the result is not contained within sceneRect,
     *  the sceneRect is expanded
     */
    void translate_view(QPointF delta);

    void add_node(QPointF pos);


public slots:
    /// Zoom view by factor ( 1 = don't zoom )
    void zoom_view(double factor);
    
signals:

    /// emitted when the graph structure or style is changed
    void graph_changed();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);

    /**
     *  \brief Expand sceneRect to contain the visible area
     *  \param margin optional margin to grow the visible area
    */
    void expand_scene_rect(int margin=0);
    
private slots:
    /// Expand scene rect when scrollbars reach margin
    void update_scrollbars() { expand_scene_rect(2); }

};

#endif // KNOT_VIEW_HPP
