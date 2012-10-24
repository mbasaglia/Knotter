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
#include "edge.hpp"
#include <QPainter>
#include "node.hpp"

Edge::Edge(Node *start, Node *end, type_type type)
    :  CustomItem(false), start(start), end(end), type ( type )
{}

QRectF Edge::boundingRect() const
{
    //return QRectF(start->pos(),end->pos()).normalized();
    double x1 = std::min( start->pos().x(), end->pos().x() );
    double x2 = std::max( start->pos().x(), end->pos().x() );
    double y1 = std::min( start->pos().y(), end->pos().y() );
    double y2 = std::max( start->pos().y(), end->pos().y() );
    double dx = x2-x1;
    double dy = y2-y1;
    if ( dx < 8 )
    {
        x1-=4;
        dx+=8;
    }
    if ( dy < 8 )
    {
        y1-=4;
        dy+=8;
    }
    return QRectF ( x1, y1, dx, dy );
}

void Edge::paint(QPainter *painter,
                const QStyleOptionGraphicsItem *,
                QWidget *)
{
    if ( !is_visible() && !highlight)
        return;

    QPen pen(QColor("#0088ff"),2);
    pen.setCosmetic(true);

    if ( highlight && is_visible() )
    {
        pen.setColor(QColor("#00ccff"));
        pen.setWidth(pen.width()*2);
    }

    if ( type == WALL )
    {
        pen.setWidth(pen.width()*3);
        pen.setCapStyle(Qt::FlatCap);
    }
    else if ( type == INVERTED )
        pen.setStyle(Qt::DashLine);

    painter->setPen(pen);

    //if ( type != WALL )
        painter->drawLine(start->pos(),end->pos());
    /*else
    {
        QPainterPathStroker pps;
        pps.setWidth(pen.width()+1);
        QPainterPath pp(start->pos());
        pp.lineTo(end->pos());
        painter->drawPath(pps.createStroke(pp));
    }*/

    if ( type == WALL )
    {
        painter->setPen(QPen(Qt::white,pen.width()/3));
        painter->drawLine(start->pos(),end->pos());
    }

    if ( type == HOLE )
    {
        QLineF l = QLineF( start->pos(),end->pos() ).normalVector();
        l.setLength(5);
        l.translate((end->pos()-start->pos())/2);
        painter->drawLine(l);
        l.setLength(-5);
        painter->drawLine(l);
    }

}

bool Edge::is_vertex(const Node *n) const
{
    return n == start || n == end;
}

Node *Edge::other(const Node *which) const
{
    if ( which == start )
        return end;
    else if ( which == end )
        return start;
    return NULL;
}

Node* Edge::vertex1() const { return start; }
Node* Edge::vertex2() const { return end; }

QPainterPath Edge::shape() const
{
    QPointF startp = start->pos();
    QPointF endp = end->pos();

    QPainterPath line;
    line.moveTo(startp);
    line.lineTo(endp);
    QPainterPathStroker pps;
    pps.setWidth(8);


    return pps.createStroke(line);
}


void Edge::traverse(Edge::handle_type handle)
{
    traversed_handles |= handle;
}

bool Edge::traversed(Edge::handle_type handle) const
{
    return traversed_handles & handle;
}


Edge::handle_type Edge::not_traversed() const
{
    if ( ! ( traversed_handles & TOPRIGHT ) )
        return TOPRIGHT;
    else if ( ! ( traversed_handles & TOPLEFT ) )
        return TOPLEFT;
    else if ( ! ( traversed_handles & BOTTOMLEFT ) )
        return BOTTOMLEFT;
    else if ( ! ( traversed_handles & BOTTOMRIGHT ) )
        return BOTTOMRIGHT;
    else
        return NOHANDLE;
}

bool Edge::connected(Edge::handle_type a, Edge::handle_type b) const
{

    switch ( type )
    {
        case CROSSING:
            return  ( a == TOPLEFT && b == BOTTOMRIGHT ) ||
                    ( b == TOPLEFT && a == BOTTOMRIGHT );
        case INVERTED:
            return  ( a == BOTTOMLEFT && b == TOPRIGHT ) ||
                    ( b == BOTTOMLEFT && a == TOPRIGHT );
        case WALL:
            return  ( a == TOPLEFT && b == TOPRIGHT ) ||
                    ( b == TOPLEFT && a == TOPRIGHT ) ||
                    ( a == BOTTOMLEFT && b == BOTTOMRIGHT ) ||
                    ( b == BOTTOMLEFT && a == BOTTOMRIGHT );
            break;
        case HOLE: // handle endpoints are merged
            return false;

    }
    return false;
}

Edge::handle_type Edge::next_handle(handle_type from) const
{
    switch ( type )
    {
        case CROSSING:
        case INVERTED:
            switch ( from )
            {
                case TOPLEFT: return BOTTOMRIGHT;
                case TOPRIGHT: return BOTTOMLEFT;
                case BOTTOMLEFT: return TOPRIGHT;
                case BOTTOMRIGHT: return TOPLEFT;
                default: return NOHANDLE;
            }
        case WALL:
            switch ( from )
            {
                case TOPLEFT: return TOPRIGHT;
                case TOPRIGHT: return TOPLEFT;
                case BOTTOMLEFT: return BOTTOMRIGHT;
                case BOTTOMRIGHT: return BOTTOMLEFT;
                default: return NOHANDLE;
            }
        case HOLE:
            switch ( from )
            {
                case TOPLEFT: return BOTTOMLEFT;
                case TOPRIGHT: return BOTTOMRIGHT;
                case BOTTOMLEFT: return TOPLEFT;
                case BOTTOMRIGHT: return TOPRIGHT;
                default: return NOHANDLE;
            }
    }
    return NOHANDLE;
}

bool Edge::completed() const
{
    static const long ored = TOPRIGHT | BOTTOMLEFT | TOPLEFT | BOTTOMRIGHT;
    return (traversed_handles & ored) == ored;
}

void Edge::reset()
{
    traversed_handles = NOHANDLE;
}

QLineF Edge::handle_point(Edge::handle_type handle,
                            double handle_length,
                            double crossing_distance) const
{
    if (  ( handle == TOPRIGHT || handle == BOTTOMRIGHT ) &&
            end && end->has_custom_style() )
    {
        handle_length = end->get_custom_style().handle_length;
    }
    else if ( ( handle == TOPLEFT || handle == BOTTOMLEFT ) &&
            start && start->has_custom_style() )
    {
        handle_length = start->get_custom_style().handle_length;
    }

    // get angle of handle point, relative to the line direction
    double handle_angle = 0;
    if ( handle == TOPRIGHT )
        handle_angle = 45;
    else if ( handle == TOPLEFT )
        handle_angle = 135;
    else if ( handle == BOTTOMLEFT )
        handle_angle = 225;
    else if ( handle == BOTTOMRIGHT )
        handle_angle = 315;

    // this line is set to start at the midpoint and end at the 45deg starting handlepoint
    QLineF line((start->pos()+end->pos())/2,end->pos());
    line.setAngle(line.angle()+handle_angle);
    line.setLength(crossing_distance);

    // special case, merge top-bottom hadles on the edge line
    if ( type == HOLE )
    {
        int direction = 0;
        if ( handle == TOPRIGHT || handle == BOTTOMRIGHT )
            direction = 1;
        else if ( handle == TOPLEFT || handle == BOTTOMLEFT )
            direction = -1;

        // this line follows the end line and is off by crossing_distance
        QLineF lineh((start->pos()+end->pos())/2,end->pos());
        lineh.setLength(crossing_distance*direction);

        // this line follows the 45deg direction for handle ctrl points
        QLineF lineh2;
        lineh2.setP1(lineh.p2());
        lineh2.setAngle(line.angle());
        lineh2.setLength(handle_length);
        return lineh2;

    }

    // standard control point, follows the 45deg andle,
    // starts at crossing_distance and ends at handle_length
    QLineF line2;
    line2.setP1(line.p2());
    line2.setAngle(line.angle());
    line2.setLength(handle_length);

    // special case, make control point parallel to the edge line
    if ( type == WALL )
    {
        // this line starts at handle_length+crossing_distance on the 45deg
        QLineF line3;
        line2.setLength(line2.length()/2);
        line3.setP1(line2.p2());

        // adjust angle
        if ( handle == TOPRIGHT || handle == BOTTOMLEFT )
            line3.setAngle(line2.angle()-45);
        else if ( handle == TOPLEFT || handle == BOTTOMRIGHT )
            line3.setAngle(line2.angle()+45);

        // set shorter handle control point distance, otherwise looks ugly
        line3.setLength(handle_length/2);

        return line3;
    }

    return line2;
}
