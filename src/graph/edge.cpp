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
#include "edge.hpp"
#include "graph.hpp"
//#include <QVector2D>
#include "edge_type.hpp"
#include "resource_manager.hpp"
#include "edge_style.hpp"

Edge::Edge(Node *v1, Node *v2, Edge_Type *type) :
    v1(v1), v2(v2),
    available_handles(TOP_LEFT|TOP_RIGHT|BOTTOM_LEFT|BOTTOM_RIGHT),
    m_graph(nullptr)
{
    attach();
    setZValue(1);
    setFlag(QGraphicsItem::ItemIsSelectable);

    m_style.enabled_style |= Edge_Style::EDGE_TYPE;
    m_style.edge_type = type ? type : Resource_Manager::default_edge_type();
}

QRectF Edge::boundingRect() const
{
    QRectF bb = QRectF(v1->pos(),v2->pos()).normalized();
    bb.setTop(bb.top()-shapew/2);
    bb.setLeft(bb.left()-shapew/2);
    bb.setWidth(bb.width()+shapew);
    bb.setHeight(bb.height()+shapew);
    return bb;
}

void Edge::detach()
{
    v1->remove_edge(this);
    v2->remove_edge(this);
}

void Edge::attach()
{
    v1->add_edge(this);
    v2->add_edge(this);
}


void Edge::set_style(Edge_Style st)
{
    m_style = st;
    m_style.enabled_style |= Edge_Style::EDGE_TYPE;
    m_style.edge_type = st.edge_type ? st.edge_type :
                                       Resource_Manager::default_edge_type();
}

Edge_Style Edge::style() const
{
    return m_style;
}

Edge_Style &Edge::style()
{
    return m_style;
}

Edge_Style Edge::defaulted_style() const
{
    return m_style.default_to(m_graph->default_edge_style());
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if ( isSelected() )
    {
        QPen pen(QColor(128,128,128,128),2);
        pen.setCosmetic(true);
        painter->setPen(pen);
        QLineF nv = to_line().normalVector();
        nv.setLength((Node::external_radius()-2)/painter->matrix().m11());
        painter->drawLine(to_line().translated(nv.dx(),nv.dy()));
        painter->drawLine(to_line().translated(-nv.dx(),-nv.dy()));
    }

    if ( visible && highlighted )
        m_style.edge_type->paint_highlighted(painter,*this);
    else if ( visible || highlighted )
        m_style.edge_type->paint_regular(painter,*this);

}

/*double Edge::distance_squared(QPointF to) const
{
     return line_point_distance_squared(v1->pos(),v2->pos(),to);
}*/

QPainterPath Edge::shape() const
{

    QPointF startp = v1->pos();
    QPointF endp = v2->pos();

    QPainterPath line;
    line.moveTo(startp);
    line.lineTo(endp);
    QPainterPathStroker pps;
    pps.setWidth(shapew);

    return pps.createStroke(line);
}
