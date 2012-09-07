/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012  Mattia Basaglia

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

#include "custom_item.hpp"

/**
\brief Edge between two nodes

This class is responsible to draw the edge in the KnotView
and to perform the logic of the edge

Handle layout:
<pre>
           TOPLEFT    TOPRIGHT
                  \  /
    Node1 o-------------------o Node2
                  /  \
        BOTTOMLEFT    BOTTOMRIGHT

</pre>


    \todo move logic in lib
*/
class Edge : public CustomItem
{
    public:
        enum type_type { CROSSING, WALL, HOLE, INVERTED };

    protected:
        Node* start;
        Node* end;

    public:
        type_type type;

        Edge( Node* start, Node* end, type_type type = CROSSING);

        QRectF boundingRect() const;

        void paint(QPainter *painter,
                const QStyleOptionGraphicsItem *,
                QWidget *);

        /// Whether node is one of its vetices
        bool is_vertex ( const Node* node ) const;

        /// Return the vertex that is not which
        Node* other ( const Node *which ) const;

        Node* vertex1() const;
        Node* vertex2() const;

        QPainterPath shape () const;

    // curve-building part:
    public:

        enum handle_type {  NOHANDLE=0,
                            TOPRIGHT=1,
                            TOPLEFT=2,
                            BOTTOMLEFT=4,
                            BOTTOMRIGHT=8
                         };
    protected:
        long traversed_handles; ///< made up by or-ing handle_type constants

    public:
        /// Mark handle as traversed
        void traverse ( handle_type handle );
        /// Whether handle has been traversed
        bool traversed ( handle_type handle ) const;

        /// \return lines to be connected in the path
        QList<QLineF> connected( double handle_length,
                                 double crossing_distance) const;

        /**
            \return one of the handles that has not been traversed or
            NOHANDLE if all have been visited
        */
        handle_type not_traversed () const;

        /**
            \return whether all handles have been traversed
        */
        bool completed() const;

        /**
            \brief reset traversal
            \post  traversed == NOHANDLE && completed() == false
        */
        void reset();

        /**
            \brief determine the physcal position of the handle
        */
        QLineF handle_point( handle_type handle,
                            double handle_length,
                            double crossing_distance ) const;

};


#endif // EDGE_HPP
