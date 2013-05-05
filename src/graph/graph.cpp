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
#include "graph.hpp"
#include "edge_style.hpp"
#include "resource_manager.hpp"

Graph::Graph() :
    m_default_node_style(225,24,10,32,Resource_Manager::default_cusp_shape(),
                  Node_Style::EVERYTHING),
    m_paint_mode(PAINT_KNOT),
    auto_color(false)
{
    m_colors.push_back(Qt::black);
    set_join_style(Qt::MiterJoin);
    set_width(5);
}

Graph::Graph(const Graph &other)
    : QGraphicsItem()
{
    *this = other;
}

Graph &Graph::operator= ( const Graph &o )
{
    m_edges = o.m_edges;
    m_colors = o.m_colors;
    m_default_node_style = o.m_default_node_style;
    m_nodes = o.m_nodes;
    m_paint_mode = o.m_paint_mode;
    bounding_box = o.bounding_box;
    auto_color = o.auto_color;
    paths = o.paths;
    pen = o.pen;
    setPos(o.pos());
    setTransform(o.transform());
    setVisible(o.isVisible());
    return *this;
}

void Graph::add_node(Node *n)
{
    m_nodes.append(n);
    n->set_visible( m_paint_mode & PAINT_GRAPH );
}

void Graph::add_edge(Edge *e)
{
    m_edges.append(e);
    e->set_visible( m_paint_mode & PAINT_GRAPH );
    e->attach();
}

void Graph::remove_node(Node *n)
{
    m_nodes.removeOne(n);
    //n->setParentItem(nullptr);
}

void Graph::remove_edge(Edge *e)
{
    m_edges.removeOne(e);
    e->detach();
    //e->setParentItem(nullptr);
}

/*void Graph::clear()
{
    foreach(Edge* e, m_edges)
        remove_edge(e);

    foreach(Node* n, m_nodes)
        remove_node(n);
}*/

void Graph::set_paint_mode(Paint_Mode mode)
{
    m_paint_mode = mode;
}

void Graph::toggle_paint_flag(Graph::Paint_Mode_Enum flag)
{
    set_paint_mode ( m_paint_mode ^ flag );
}

void Graph::enable_paint_flag(Graph::Paint_Mode_Enum flag)
{
    set_paint_mode ( m_paint_mode | flag );
}

void Graph::disable_paint_flag(Graph::Paint_Mode_Enum flag)
{
    set_paint_mode ( m_paint_mode & ~flag );
}

void Graph::set_colors(const QList<QColor> &l)
{
    m_colors = l;
}

void Graph::set_join_style(Qt::PenJoinStyle style)
{
    pen.setJoinStyle(style);
}

Qt::BrushStyle Graph::brush_style() const
{
    return Qt::SolidPattern;
}

void Graph::set_brush_style(Qt::BrushStyle)
{
    /// \todo brush style
}

void Graph::set_default_node_style(Node_Style style)
{
    m_default_node_style = style;
}

void Graph::set_width(double w)
{
    pen.setWidthF(w);
}

void Graph::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    const_paint(painter,option,widget,m_paint_mode);
}

void Graph::const_paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                        QWidget *widget, Paint_Mode mode) const
{
    if ( mode & PAINT_GRAPH )
    {
        foreach(Edge* e, m_edges)
            e->paint(painter,option,widget);
        foreach(Node* n, m_nodes)
        {
            painter->translate(n->pos());
            n->paint(painter,option,widget);
            painter->translate(-n->pos());
        }
    }

    if ( mode & PAINT_KNOT && ! m_colors.empty() )
    {
        painter->setBrush(Qt::NoBrush);
        QPen p = pen;
        for ( int i = 0; i < paths.size(); ++i )
        {
            if ( auto_color )
                p.setColor(QColor::fromHsv(i*360/paths.size(),192,170));
            else
                p.setColor(m_colors[i%m_colors.size()]);
            painter->setPen(p);
            painter->drawPath(paths[i]);
        }
    }
}

void Graph::const_paint(QPainter *painter, Paint_Mode mode) const
{
    const_paint(painter,nullptr,nullptr,mode);
}

void Graph::render_knot()
{
    paths.clear();
    Path_Builder path;
    traverse(path);
    paths = path.build();
    update_bounding_box();
    update();
}

Graph Graph::sub_graph(QList<Node *> nodes) const
{
    Graph graph(*this);
    graph.m_nodes.clear();
    graph.m_edges.clear();

    graph.m_nodes.reserve(nodes.size());
    graph.m_edges.reserve(nodes.size());

    graph.set_brush_style(brush_style());
    graph.set_colors(colors());
    graph.set_custom_colors(custom_colors());
    graph.set_default_node_style(default_node_style());
    graph.set_join_style(join_style());
    graph.set_paint_mode(paint_mode());
    graph.set_width(width());


    foreach ( Node* n, nodes )
    {
        graph.m_nodes.push_back(n);

        foreach ( Edge* e, n->connections() )
        {
            if ( nodes.contains(e->other(n)) &&
                 !graph.m_edges.contains(e) )
            {
                graph.m_edges.push_back(e);
            }
        }
    }

    return graph;

}

void Graph::traverse(Path_Builder &path)
{
    QList<Edge*> traversed_edges;
    traversed_edges.reserve(m_edges.size());

    for(QList<Edge*>::iterator i = m_edges.begin(); i != m_edges.end(); ++i)
        (*i)->reset();

    // cycle while there are edges with untraversed handles
    while(!m_edges.empty())
    {
        // pick first edge/handle
        Edge* edge = m_edges.front();
        Edge::Handle handle = edge->not_traversed();

        if ( handle == Edge::NO_HANDLE )
        {
            // removed completed edge
            traversed_edges.push_back(edge);
            m_edges.pop_front();
            continue;
        }

        // begin new knot thread
        path.new_group();

        // loop around a knotline loop item
        while ( ! edge->traversed(handle) )
        {
            edge->mark_traversed(handle);
            Traversal_Info ti = traverse(edge,handle,path);

            //ti.node->style().build(ti,path,default_node_style);

            edge = ti.out.edge;
            edge->mark_traversed(ti.out.handle);
            // Don't mark handle as traversed but render and get next handle
            handle = edge->style()->traverse(edge,ti.out.handle,path,m_default_node_style);

        }
    }

    m_edges.swap(traversed_edges);
}


Traversal_Info Graph::traverse(Edge *edge, Edge::Handle handle,
                               Path_Builder &path)
{
    // set input values
    Traversal_Info ti;
    ti.in.edge = edge;
    ti.in.handle = handle;
    ti.node = edge->vertex_for(handle);
    ti.in.angle = QLineF ( ti.node->pos(), ti.in.edge->other(ti.node)->pos() ).angle();
    edge->mark_traversed(handle);


    // select handside (as viewed from the edge)
    if ( ti.in.edge->vertex1() == ti.node )
    {
        // RH = TL,  LH = BL
        if ( handle == Edge::TOP_LEFT )
            ti.handside = Traversal_Info::RIGHT;
        else if ( handle == Edge::BOTTOM_LEFT )
            ti.handside = Traversal_Info::LEFT;
        else
            return Traversal_Info(); // Wrong handle
    }
    else if ( ti.in.edge->vertex2() == ti.node )
    {
        // RH = BR,  LH = TR
        if ( handle == Edge::BOTTOM_RIGHT )
            ti.handside = Traversal_Info::RIGHT;
        else if ( handle == Edge::TOP_RIGHT )
            ti.handside = Traversal_Info::LEFT;
        else
            return Traversal_Info(); // Wrong handle
    }
    else
        return Traversal_Info(); // Wrong edge


    ti.out.angle = ti.in.angle;
    ti.angle_delta = 360;
    ti.out.edge = ti.in.edge;
    // select the next edge as the one with the smallest angle difference
    // angle direction is based on handside
    foreach(Edge* i, ti.node->connections() )
    {
        if ( i != ti.in.edge )
        {
            double angle_out = QLineF ( ti.node->pos(),i->other(ti.node)->pos() ).angle();
            double delta = ti.in.angle - angle_out;
            if ( delta < 0 )
                delta += 360;
            if ( ti.handside == Traversal_Info::RIGHT )
                delta = 360-delta;
            if ( delta < ti.angle_delta )
            {
                ti.angle_delta = delta;
                ti.out.edge = i;
                ti.out.angle = angle_out;
            }
        }
    }


    // output values

    // select output handles based on handside
    if ( ti.out.edge->vertex1() == ti.node )
    {
        // RH -> BL,  LH -> TL
        ti.out.handle = ti.handside == Traversal_Info::RIGHT ?
                    Edge::BOTTOM_LEFT :
                    Edge::TOP_LEFT;
    }
    else if ( ti.out.edge->vertex2() == ti.node )
    {
        // RH -> TR,  LH -> BR
        ti.out.handle = ti.handside == Traversal_Info::RIGHT ?
                    Edge::TOP_RIGHT :
                    Edge::BOTTOM_RIGHT;
    }

    ti.success = true;

    ti.node->style().build(ti,path,m_default_node_style);

    return ti;
}

void Graph::update_bounding_box()
{
    bounding_box = QRectF();
    if ( ! paths.empty() )
    {
        foreach(const QPainterPath&pp, paths)
            bounding_box |= pp.controlPointRect();
    }
}

