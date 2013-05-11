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
/**
 *  Class that manages all the stuff needed to transform nodes
*/
class Node_Mover
{
private:
    QList<Node*>    m_nodes;
    QPointF         pivot; ///< Pivot point for movement
    QPointF         start_pos;///< Starting pivot point position
    QList<QPointF>  offset; ///< Offset of each node from pivot
    double          scale_factor;
    int             scale_count; ///< Number of size units when using fixed_scale
    double          rotate_angle;
    QRectF          initial_box; ///< Bounding box of the selection on initialization

public:
    /// Initialize movement
    Node_Mover();

    void initialize_movement(QPointF pivot = QPointF());

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
    void deploy(class Knot_View *view);

    QSizeF initial_size() const { return initial_box.size(); }

    void set_nodes(QList<Node*> nodes);
    QList<Node*> nodes() const { return m_nodes; }
};

#endif // NODE_MOVER_HPP
