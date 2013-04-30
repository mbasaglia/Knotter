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
    QList<Node*>    nodes;
    QPointF         pivot; ///< Pivot point for movement
    QPointF         start_pos;///< Starting pivot point position
    QList<QPointF>  offset; ///< Offset of each node from pivot
public:
    /// Initialize movement
    Node_Mover(QList<Node*> nodes = QList<Node*>(), QPointF pivot = QPointF() );

    /// Apply movement by delta to each node
    void move(QPointF delta);

    /// Create required commands
    void deploy(class Knot_View *view);
};

#endif // NODE_MOVER_HPP
