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
#include "knot_curve_style.hpp"
#include "basic_knot_graph.hpp"

void knot_curve_ogee::draw_joint ( path_builder& path,
                    const TraversalInfo& ti,
                    styleinfo style )
{
    QLineF start = ti.in.edge->handle_point(ti.in.handle,style.handle_length,style.crossing_distance);
    QLineF finish = ti.out.edge->handle_point(ti.out.handle,style.handle_length,style.crossing_distance);

    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {
        QPointF cusp_point = get_cusp_point(start, finish,ti,style.cusp_distance);


        QLineF bar (start.p1(), finish.p1()); // join handle base
        bar.setLength(bar.length()/5); // get 1 fifth
        QLineF handlebs1(cusp_point,bar.p2()); // join to cusp
        handlebs1.setLength(style.handle_length); // proper length

        bar.setP1(finish.p1()); // move p1 to finish ( the other 4/5ths )
        bar.setLength(bar.length()/4); // back to 1 fifth
        QLineF handlebs2(cusp_point,bar.p2());
        handlebs2.setLength(style.handle_length);

        /*
        old ogee algorithm
        QLineF handlebs1(cusp_point,start.p1());
        QLineF handlebs2(cusp_point,finish.p1());

        handlebs1.setLength(style.handle_length);
        handlebs2.setLength(style.handle_length);*/

        path.add_cubic(start.p1(),start.p2(),handlebs1.p2(),handlebs1.p1());
        path.add_cubic(handlebs1.p1(),handlebs2.p2(),finish.p2(),finish.p1());

    }
    else // draw cubic
    {
        /*path.moveTo(start.p1());
        path.cubicTo(start.p2(),finish.p2(),finish.p1());*/
        path.add_cubic(start.p1(),start.p2(),finish.p2(),finish.p1());
    }
}

void knot_curve_pointed::draw_joint ( path_builder& path,
                    const TraversalInfo& ti,
                    styleinfo style )
{
    QLineF start = ti.in.edge->handle_point(ti.in.handle,style.handle_length,style.crossing_distance);
    QLineF finish = ti.out.edge->handle_point(ti.out.handle,style.handle_length,style.crossing_distance);

    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {
        QPointF cusp_point = get_cusp_point(start, finish,ti,style.cusp_distance);


        path.add_quad(start.p1(),start.p2(),cusp_point);
        path.add_quad(cusp_point,finish.p2(),finish.p1());
        /*path.moveTo(start.p1());
        path.quadTo(start.p2(),cusp_point);
        path.quadTo(finish.p2(),finish.p1());*/

    }
    else // draw cubic
    {
        /*path.moveTo(start.p1());
        path.cubicTo(start.p2(),finish.p2(),finish.p1());*/
        path.add_cubic(start.p1(),start.p2(),finish.p2(),finish.p1());
    }
}

void knot_curve_polygonal::draw_joint ( path_builder& path,
                    const TraversalInfo& ti,
                    styleinfo style )
{
    QLineF start = ti.in.edge->handle_point(ti.in.handle,style.handle_length,style.crossing_distance);
    QLineF finish = ti.out.edge->handle_point(ti.out.handle,style.handle_length,style.crossing_distance);

    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {
        QPointF cusp_point = get_cusp_point(start, finish,ti,style.cusp_distance);

        path.add_line(start.p1(),start.p2());
        path.add_line(start.p2(),cusp_point);
        path.add_line(cusp_point,finish.p2());
        path.add_line(finish.p2(),finish.p1());

        /*path.moveTo(start.p1());
        path.lineTo(start.p2());
        //path.lineTo(handlebs1.p2());
        path.lineTo(cusp_point);
        //path.lineTo(handlebs2.p2());
        path.lineTo(finish.p2());
        path.lineTo(finish.p1());*/

    }
    else // draw cubic
    {
        /*path.moveTo(start.p1());
        path.cubicTo(start.p2(),finish.p2(),finish.p1());*/
        path.add_cubic(start.p1(),start.p2(),finish.p2(),finish.p1());
    }
}

knot_curve_style *knot_curve_styler::style(style_id id)
{
    return styles[id];
}

knot_curve_style *knot_curve_styler::style(QString name)
{
    return styles[names[name]];
}

knot_curve_styler::style_id knot_curve_styler::register_style(knot_curve_style *style)
{
    style_id id = styles.size() + 1;
    styles[id] = style;
    return id;
}

void knot_curve_styler::register_style(knot_curve_style *style, QString name)
{
    register_alias(register_style(style),name);
}

void knot_curve_styler::register_alias(knot_curve_styler::style_id id, QString name)
{
    names[name] = id;
}

knot_curve_styler::style_id knot_curve_styler::idof(QString name)
{
    if ( !names.contains(name) )
        return styles.begin().key(); // return default rather than 0
    return names[name];
}

QString knot_curve_styler::name(knot_curve_styler::style_id id)
{
    return names.key(id);
}


void knot_curve_styler::clear()
{
    foreach( knot_curve_style* sty, styles.values() )
        delete sty;
}
QMap<knot_curve_styler::style_id,knot_curve_style*> knot_curve_styler::styles;
QMap<QString,knot_curve_styler::style_id> knot_curve_styler::names;

QPointF knot_curve_style:: get_cusp_point ( QLineF start,
                                 QLineF finish,
                                 const TraversalInfo& ti,
                                 double def_dist ) const
{

    QLineF bisect(0,0,1,1);
    // place line in the cusp node and give it the right direction
    bisect.setP1(ti.middle->pos());
    bisect.setAngle((ti.in.angle+ti.out.angle)/2);

    // make a line parallel to the input edge and place it in the start handle endpoint
    QLineF parallel(ti.in.node->pos(),ti.middle->pos());
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

    (void)finish;


    return bisect.p2();
}

void knot_curve_round::draw_joint(path_builder &path, const TraversalInfo &ti, styleinfo style)
{
    QLineF start = ti.in.edge->handle_point(ti.in.handle,style.handle_length,style.crossing_distance);
    QLineF finish = ti.out.edge->handle_point(ti.out.handle,style.handle_length,style.crossing_distance);

    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {
        QPointF cusp_point = get_cusp_point(start, finish,ti,style.cusp_distance);

        QLineF handle( start.p1(),finish.p1() );
        handle.translate(cusp_point-start.p1());
        handle.setLength(style.handle_length/2);
        QPointF h2 = handle.p2();
        handle.setLength(-style.handle_length/2);
        QPointF h1 = handle.p2();

        path.add_cubic(start.p1(),start.p2(),h1,cusp_point);
        path.add_cubic(finish.p1(),finish.p2(),h2,cusp_point);

    }
    else // draw cubic
    {
        path.add_cubic(start.p1(),start.p2(),finish.p2(),finish.p1());
    }
}

styleinfo styleinfo::default_to(const styleinfo &other) const
{
    return styleinfo (
                (enabled_style & CURVE_STYLE ) ? curve_style : other.curve_style,
                (enabled_style & CUSP_ANGLE ) ? cusp_angle : other.cusp_angle,
                (enabled_style & HANDLE_LENGTH ) ? handle_length : other.handle_length,
                (enabled_style & CROSSING_DISTANCE ) ? crossing_distance : other.crossing_distance,
                (enabled_style & CUSP_DISTANCE ) ? cusp_distance : other.cusp_distance,
                EVERYTHING
            );

}

