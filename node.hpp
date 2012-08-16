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
#ifndef NODE_HPP
#define NODE_HPP

#include "custom_item.hpp"
#include "edge.hpp"

struct TraversalInfo
{
    enum handside_type { LEFT, RIGHT };

    Edge* edge_in;
    Edge::handle_type handle_in;
    Edge* edge_out;
    Edge::handle_type handle_out;
    handside_type handside;
    double angle_in;
    double angle_out;
    double angle_delta;
    bool success;
    QString error_message;

    TraversalInfo ( QString error_message )
        : edge_in ( NULL ), handle_in ( Edge::NOHANDLE ),
            edge_out ( NULL ), handle_out ( Edge::NOHANDLE ),
            angle_in(0), angle_out(0), angle_delta(0),
            success(false), error_message ( error_message )
    {}

    TraversalInfo()
        : edge_in ( NULL ), handle_in ( Edge::NOHANDLE ),
            edge_out ( NULL ), handle_out ( Edge::NOHANDLE ),
            angle_in(0), angle_out(0), angle_delta(0),
            success(false), error_message ()
    {}


};

class Node : public CustomItem
{
    protected:
        static const int radius = 5;
        QList<Edge*> edges;

    public:

        Node(QPointF position);

        QRectF boundingRect() const;

        QRectF actualRect() const;

        void paint( QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget);

        Edge* get_link(Node* other) const;
        void add_link ( Edge* edge );
        void remove_link ( Edge* which );
        QList<Edge*> links() const { return edges; }
        QList<Node*> linked_to() const;


        // curve-building functions:

        /**
            \brief find next edge end handle in traversal
            \details Modifies the parameters so that the restult will be the
            next edge/handle pair.
        */
        TraversalInfo next_edge ( Edge* edge, Edge::handle_type handle ) const;

};


typedef QList<Node*> node_list;

#endif // NODE_HPP
