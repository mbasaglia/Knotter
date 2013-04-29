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

#include "node_cusp_shape.hpp"
#include "edge_style.hpp"
#include "traversal_info.hpp"

QPointF Node_Cusp_Shape::cusp_point(QLineF start, QLineF finish,
                                    const Traversal_Info &ti,
                                    double def_dist) const
{
    /// \todo try to find a cleaner algorithm

    QLineF bisect(0,0,1,1);
    // place line in the cusp node and give it the right direction
    bisect.setP1(ti.node->pos());
    bisect.setAngle((ti.in.angle+ti.out.angle)/2);

    // make a line parallel to the input edge and place it in the start handle endpoint
    QLineF parallel = ti.in.edge->to_line();
    parallel.setLength(parallel.length()/2);
    parallel.translate(start.p2()-parallel.p1());

    QPointF intersect = bisect.p2();
    if ( ti.angle_delta < 350 )
    {
        // find the intersection between the bisector and the parallel line
        parallel.intersect(bisect,&intersect);
        bisect.setP2(intersect);
    }
    else
        bisect.setAngle((ti.in.angle+ti.out.angle)/2+180);

    bisect.setLength(def_dist);

    Q_UNUSED(finish);


    return bisect.p2();
}


void Cusp_Rounded::draw_joint(Path_Builder &path, const Traversal_Info &ti,
                              const Node_Style &style) const
{

    QLineF start = ti.in.edge->style()->handle(ti.in.edge,ti.in.handle,style);
    QLineF finish = ti.out.edge->style()->handle(ti.out.edge,ti.out.handle,style);

    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {
        QPointF cusp_pt = cusp_point(start, finish,ti,style.cusp_distance);

        QLineF handle( start.p1(),finish.p1() );
        handle.translate(cusp_pt-start.p1());
        handle.setLength(style.handle_length/2);
        QPointF h2 = handle.p2();
        handle.setLength(-style.handle_length/2);
        QPointF h1 = handle.p2();

        path.add_cubic(start.p1(),start.p2(),h1,cusp_pt);
        path.add_cubic(finish.p1(),finish.p2(),h2,cusp_pt);

    }
    else // draw cubic
    {
        path.add_cubic(start.p1(),start.p2(),finish.p2(),finish.p1());
    }
}
