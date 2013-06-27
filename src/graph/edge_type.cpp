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

#include "edge_type.hpp"
#include "edge.hpp"

Edge_Type::Edge_Type()
{
}

void Edge_Type::paint_regular(QPainter *painter, const Edge &edge)
{
    QPen pen(QColor("#0088ff"),2);
    pen.setCosmetic(true);
    painter->setPen(pen);

    paint(painter,edge);
}

void Edge_Type::paint_highlighted(QPainter *painter, const Edge &edge)
{
    QPen pen(QColor("#00ccff"),4);
    pen.setCosmetic(true);
    painter->setPen(pen);

    paint(painter,edge);
}

void Edge_Type::paint(QPainter *painter, const Edge &edge)
{
    painter->drawLine(edge.to_line());
}

QLineF Edge_Normal::handle(const Edge *edge, Edge::Handle handle,
                          const Knot_Style &default_style) const
{

    Knot_Style style = edge->style().default_to(default_style);

    long double handle_angle = 0;
    if ( handle == Edge::TOP_RIGHT )
        handle_angle = pi()/4.0;
    else if ( handle == Edge::TOP_LEFT )
        handle_angle = pi()*3.0/4.0;
    else if ( handle == Edge::BOTTOM_LEFT )
        handle_angle = pi()*5.0/4.0;
    else if ( handle == Edge::BOTTOM_RIGHT )
        handle_angle = pi()*7.0/4.0;

    handle_angle += deg2rad(edge->to_line().angle());
    QPointF p1 = edge->to_line().pointAt(style.edge_slide);
    p1.setX(p1.x()+style.crossing_distance/2*qCos(handle_angle));
    p1.setY(p1.y()-style.crossing_distance/2*qSin(handle_angle));

    QPointF p2;
    p2.setX(p1.x()+style.handle_length*qCos(handle_angle));
    p2.setY(p1.y()-style.handle_length*qSin(handle_angle));

    return QLineF(p1,p2);
}



Edge::Handle Edge_Normal::traverse(Edge *edge, Edge::Handle hand,
                                   Path_Builder &path,
                                   const Knot_Style &default_style) const
{

    Edge::Handle next = Edge::NO_HANDLE;
    if ( hand == Edge::TOP_RIGHT )
        next = Edge::BOTTOM_LEFT;
    else if ( hand == Edge::BOTTOM_RIGHT )
        next = Edge::TOP_LEFT;
    else if ( hand == Edge::BOTTOM_LEFT )
        next = Edge::TOP_RIGHT;
    else if ( hand == Edge::TOP_LEFT )
        next = Edge::BOTTOM_RIGHT;


   if ( hand == Edge::TOP_LEFT || next == Edge::TOP_LEFT )
       path.add_line(handle(edge,hand,default_style).p1(),
                     handle(edge,next,default_style).p1()
                     );

   return next ;
}

QString Edge_Normal::name() const
{
    return QObject::tr("Regular");
}

QString Edge_Normal::machine_name() const
{
    return "regular";
}


QString Edge_Inverted::machine_name() const
{
    return "inverted";
}


QString Edge_Inverted::name() const
{
    return QObject::tr("Inverted");
}



void Edge_Inverted::paint(QPainter *painter, const Edge &edge)
{
    QPen pen = painter->pen();
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);

    painter->drawLine(edge.to_line());
}

Edge::Handle Edge_Inverted::traverse(Edge *edge, Edge::Handle hand,
                    Path_Builder &path, const Knot_Style &default_style) const
{
    Edge::Handle next = Edge::NO_HANDLE;
    if ( hand == Edge::TOP_RIGHT )
        next = Edge::BOTTOM_LEFT;
    else if ( hand == Edge::BOTTOM_RIGHT )
        next = Edge::TOP_LEFT;
    else if ( hand == Edge::BOTTOM_LEFT )
        next = Edge::TOP_RIGHT;
    else if ( hand == Edge::TOP_LEFT )
        next = Edge::BOTTOM_RIGHT;


   if ( hand == Edge::TOP_RIGHT || next == Edge::TOP_RIGHT )
       path.add_line(handle(edge,hand,default_style).p1(),
                     handle(edge,next,default_style).p1()
                     );

   return next ;
}



void Edge_Wall::paint(QPainter *painter, const Edge &edge)
{
    QPen pen = painter->pen();
    pen.setWidth(pen.width()*3);
    painter->setPen(pen);

    painter->drawLine(edge.to_line());


    QPen p(Qt::white,pen.width()/3);
    p.setCosmetic(true);
    painter->setPen(p);

    painter->drawLine(edge.to_line());
}

Edge::Handle Edge_Wall::traverse(Edge *edge, Edge::Handle hand,
                        Path_Builder &path, const Knot_Style &default_style) const
{
    Edge::Handle next = Edge::NO_HANDLE;
    if ( hand == Edge::TOP_RIGHT )
        next = Edge::TOP_LEFT;
    else if ( hand == Edge::BOTTOM_RIGHT )
        next = Edge::BOTTOM_LEFT;
    else if ( hand == Edge::BOTTOM_LEFT )
        next = Edge::BOTTOM_RIGHT;
    else if ( hand == Edge::TOP_LEFT )
        next = Edge::TOP_RIGHT;

    /*Q_UNUSED(edge);
    Q_UNUSED(path);
    Q_UNUSED(default_style);*/

    QLineF h1 = handle(edge,hand,default_style);
    QLineF h2 = handle(edge,next,default_style);
    if ( !qFuzzyCompare(h1.p1(),h2.p1()) )
    {
        path.add_line(h1.p1(),h2.p1());
    }

   return next ;
}


QString Edge_Wall::name() const
{
    return QObject::tr("Wall");
}

QString Edge_Wall::machine_name() const
{
    return "wall";
}

QLineF Edge_Wall::handle(const Edge *edge, Edge::Handle handle,
                         const Knot_Style &default_style) const
{

    Knot_Style style = edge->style().default_to(default_style);

    long double handle_angle = 0;
    if ( handle == Edge::TOP_RIGHT || handle == Edge::TOP_LEFT )
        handle_angle = pi()/2.0;
    else
        handle_angle = pi()*3.0/2.0;

    long double edge_angle = deg2rad(edge->to_line().angle());
    handle_angle += edge_angle;

    QPointF p1 = edge->to_line().pointAt(style.edge_slide);
    p1.setX(p1.x()+style.crossing_distance/2*qCos(handle_angle));
    p1.setY(p1.y()-style.crossing_distance/2*qSin(handle_angle));


    if ( handle == Edge::TOP_RIGHT || handle == Edge::BOTTOM_RIGHT )
        handle_angle = 0;
    else
        handle_angle = pi();
    handle_angle += edge_angle;

    QPointF p2;
    p2.setX(p1.x()+style.handle_length*qCos(handle_angle));
    p2.setY(p1.y()-style.handle_length*qSin(handle_angle));


    /*double cd1 = style.crossing_distance;
    double cd2 = edge->other(edge->vertex_for(handle))->style()
                    .default_to(default_style).crossing_distance;
    if ( !qFuzzyCompare(cd1,cd2) )
    {
        p1 = p2;
        p2.setX(p1.x()+style.handle_length*qCos(handle_angle));
        p2.setY(p1.y()-style.handle_length*qSin(handle_angle));
    }*/

    return QLineF(p1,p2);
}


QString Edge_Hole::name() const
{
    return QObject::tr("Hole");
}

QString Edge_Hole::machine_name() const
{
    return "hole";
}

QLineF Edge_Hole::handle(const Edge *edge, Edge::Handle handle,
                         const Knot_Style &default_style) const
{
    Knot_Style style = edge->style().default_to(default_style);

    long double handle_angle = 0;
    if ( handle == Edge::BOTTOM_LEFT || handle == Edge::TOP_LEFT )
        handle_angle = pi();

    long double edge_angle = deg2rad(edge->to_line().angle());
    handle_angle += edge_angle;

    QPointF p1 = edge->to_line().pointAt(style.edge_slide);
    p1.setX(p1.x()+style.crossing_distance/2*qCos(handle_angle));
    p1.setY(p1.y()-style.crossing_distance/2*qSin(handle_angle));


    if ( handle == Edge::TOP_RIGHT || handle == Edge::TOP_LEFT )
        handle_angle = pi()/2.0;
    else
        handle_angle = pi()*3.0/2.0;
    handle_angle += edge_angle;

    QPointF p2;
    p2.setX(p1.x()+style.handle_length*qCos(handle_angle));
    p2.setY(p1.y()-style.handle_length*qSin(handle_angle));

    return QLineF(p1,p2);

}


void Edge_Hole::paint(QPainter *painter, const Edge &edge)
{

    painter->drawLine(edge.to_line());

    long double angle = deg2rad(edge.to_line().angle()) + pi() / 2;
    const double length = 5;
    QPointF p = edge.to_line().pointAt(
                edge.style().enabled_style & Knot_Style::EDGE_SLIDE ?
                edge.style().edge_slide : 0.5
    );

    painter->drawLine( p.x()+length*qCos(angle), p.y()-length*qSin(angle),
                       p.x()-length*qCos(angle), p.y()+length*qSin(angle)  );
}

Edge::Handle Edge_Hole::traverse(Edge *edge, Edge::Handle handle,
                                 Path_Builder &path,
                                 const Knot_Style &default_style) const
{

    Edge::Handle next = Edge::NO_HANDLE;
    if ( handle == Edge::TOP_RIGHT )
        next = Edge::BOTTOM_RIGHT;
    else if ( handle == Edge::BOTTOM_RIGHT )
        next = Edge::TOP_RIGHT;
    else if ( handle == Edge::BOTTOM_LEFT )
        next = Edge::TOP_LEFT;
    else if ( handle == Edge::TOP_LEFT )
        next = Edge::BOTTOM_LEFT;

    Q_UNUSED(edge);
    Q_UNUSED(path);
    Q_UNUSED(default_style);

   return next ;
}
