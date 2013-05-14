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

#ifndef NODE_MOVER_HPP
#define NODE_MOVER_HPP

#include "node.hpp"
#include <QUndoStack>
#include "transform_handle.hpp"
/**
 *  Class that manages all the stuff needed to transform nodes
*/
class Node_Mover
{
private:
    QList<Node*>        m_nodes;
    QPointF             pivot; ///< Pivot point for movement
    QPointF             start_pos;///< Starting pivot point position
    QList<QPointF>      offset; ///< Offset of each node from pivot
    double              scale_factor;
    int                 scale_count; ///< Number of size units when using fixed_scale
    double              rotate_angle;
    QRectF              m_initial_box; ///< Bounding box of the selection on initialization
    static const int    n_handles = 4; ///< size of transform_handles
    Transform_Handle    transform_handles[n_handles];
    Transform_Handle*   dragged_handle; ///< Transform_Handle being dragged

public:
    /// Initialize movement
    Node_Mover();

    void initialize_movement(QPointF pivot );

    /// Apply movement by delta to each node
    void move(QPointF delta);

    /**
     *  \brief Rotate nodes by angle
     *
     *  \param angle Angle in degrees
     */
    void rotate(double angle);

    void scale(double factor);

    /**
     *  \brief Scale the selection so that \[ width = width_{start} + n * step-size \]
     *
     *  \param increase  If true increase \i n otherwise decrease it
     *
     *  \param step_size Size of the step
     */
    void fixed_scale(bool increase, double step_size);

    /// Create required commands
    void deploy(class Knot_View *view, QString message);

    QSizeF initial_size() const { return m_initial_box.size(); }
    QRectF initial_box() const { return m_initial_box; }

    void set_nodes(QList<Node*> nodes);
    QList<Node*> nodes() const { return m_nodes; }

    /// Update position and visibility of the transform handles
    void update_transform_handles();
    void add_handles_to_scene(QGraphicsScene* scene);

    void set_mode(Transform_Handle::Mode mode);
    Transform_Handle::Mode mode() const { return transform_handles[0].mode(); }

    /**
     *  \brief Initialize handle drag
     *
     *  Will initialize movement to the bounding box center
     *
     *  \param handle       Dragged hadle
     *  \param anchor_angle Whether to anchor the transformation to the bounding box vertices
     */
    void set_dragged_handle(Transform_Handle* handle, bool anchor_angle);

    /**
     *  \brief Drag dragged_handle
     *
     *  \param p            Suggested new position
     *  \param fixed        Whether to thransform using fixed steps
     *  \param step_size    Fixed scale size
     */
    void drag_handle(QPointF p, bool fixed, double step_size);

private:
    /// Initialize movement without updating handles
    void initialize_movement_internal(QPointF pivot );


};

#endif // NODE_MOVER_HPP
