/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#include "node_cusp_shape.hpp"
#include "edge_type.hpp"
#include "traversal_info.hpp"

QPointF Cusp_Shape::cusp_point(const Traversal_Info &ti, double def_dist) const
{

    QLineF bisect(0,0,1,1);
    // place line in the cusp node and give it the right direction
    bisect.setP1(ti.node->pos());
    int d = 1;
    if ( ti.handside == Traversal_Info::LEFT )
        d = -1;
    double delta = ti.in.angle+d*ti.angle_delta/2;
    bisect.setAngle(delta);
    bisect.setLength(def_dist);
    return bisect.p2();
}

void Cusp_Shape::default_path(Path_Builder &path, QLineF start, QLineF finish) const
{
    double distance = point_distance(start.p1(),finish.p1());
    if ( distance < start.length()+finish.length() )
    {
        QPointF sum = finish.p1()+start.p2()-finish.p2();
        QLineF connect (start.p1(),finish.p1());
        QPointF proj = project(sum,connect);
        double proj_distance = point_distance(start.p1(),proj);
        if ( proj_distance >= connect.length() )
        {
            double factor = distance/(start.length()+finish.length());
            start.setLength(start.length()*factor);
            finish.setLength(finish.length()*factor);
        }
    }
    path.add_cubic(start.p1(),start.p2(),finish.p2(),finish.p1());
}


void Cusp_Rounded::draw_joint(Path_Builder &path, const Traversal_Info &ti,
                              const Node_Style &style) const
{

    QLineF start = ti.in.edge->style().edge_type->handle(ti.in.edge,ti.in.handle);
    QLineF finish = ti.out.edge->style().edge_type->handle(ti.out.edge,ti.out.handle);

    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {
        QPointF cusp_pt = cusp_point(ti,style.cusp_distance);

        QLineF handle( start.p1(),finish.p1() );
        handle.translate(cusp_pt-start.p1());
        handle.setLength(style.handle_length);
        QPointF h2 = handle.p2();
        handle.setLength(-style.handle_length);
        QPointF h1 = handle.p2();

        path.add_cubic(start.p1(),start.p2(),h1,cusp_pt);
        path.add_cubic(finish.p1(),finish.p2(),h2,cusp_pt);

    }
    else
    {
        default_path(path,start,finish);
    }
}


void Cusp_Pointed::draw_joint(Path_Builder &path, const Traversal_Info &ti, const Node_Style &style) const
{
    QLineF start = ti.in.edge->style().edge_type->handle(ti.in.edge,ti.in.handle);
    QLineF finish = ti.out.edge->style().edge_type->handle(ti.out.edge,ti.out.handle);

    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {
        QPointF cusp_pt = cusp_point(ti,style.cusp_distance);


        path.add_quad(start.p1(),start.p2(),cusp_pt);
        path.add_quad(cusp_pt,finish.p2(),finish.p1());

    }
    else
    {
        default_path(path,start,finish);
    }
}


void Cusp_Ogee::draw_joint(Path_Builder &path, const Traversal_Info &ti, const Node_Style &style) const
{
    QLineF start = ti.in.edge->style().edge_type->handle(ti.in.edge,ti.in.handle);
    QLineF finish = ti.out.edge->style().edge_type->handle(ti.out.edge,ti.out.handle);

    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {
        QPointF cusp_pt = cusp_point(ti,style.cusp_distance);

        QLineF bar (start.p1(), finish.p1()); // join handle base
        bar.setLength(bar.length()/5); // get 1 fifth
        QLineF handlebs1(cusp_pt,bar.p2()); // join to cusp
        handlebs1.setLength(style.handle_length); // proper length

        bar.setP1(finish.p1()); // move p1 to finish ( the other 4/5ths )
        bar.setLength(bar.length()/4); // back to 1 fifth
        QLineF handlebs2(cusp_pt,bar.p2());
        handlebs2.setLength(style.handle_length);

        path.add_cubic(start.p1(),start.p2(),handlebs1.p2(),handlebs1.p1());
        path.add_cubic(handlebs1.p1(),handlebs2.p2(),finish.p2(),finish.p1());

    }
    else
    {
        default_path(path,start,finish);
    }
}


void Cusp_Polygonal::draw_joint(Path_Builder &path, const Traversal_Info &ti, const Node_Style &style) const
{
    QLineF start = ti.in.edge->style().edge_type->handle(ti.in.edge,ti.in.handle);
    QLineF finish = ti.out.edge->style().edge_type->handle(ti.out.edge,ti.out.handle);

    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {
        QPointF cusp_pt = cusp_point(ti,style.cusp_distance);

        path.add_line(start.p1(),start.p2());
        path.add_line(start.p2(),cusp_pt);
        path.add_line(cusp_pt,finish.p2());
        path.add_line(finish.p2(),finish.p1());


    }
    else
    {
        default_path(path,start,finish);
    }
}
