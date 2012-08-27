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
#include "node.hpp"

void knot_curve_ogee::draw_joint ( QPainterPath& path,
                    Node* node,
                    const TraversalInfo& ti,
                    styleinfo style )
{
    QLineF start = ti.edge_in->handle_point(ti.handle_in,style.handle_length,style.crossing_distance);
    QLineF finish = ti.edge_out->handle_point(ti.handle_out,style.handle_length,style.crossing_distance);

    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {
        QPointF cusp_point = get_cusp_point(start, finish,node,ti,style.cusp_distance);

        QLineF handlebs1(cusp_point,start.p1());
        QLineF handlebs2(cusp_point,finish.p1());

        handlebs1.setLength(32);
        handlebs2.setLength(32);

        path.moveTo(start.p1());
        path.cubicTo(start.p2(),handlebs1.p2(),handlebs1.p1());
        path.cubicTo(handlebs2.p2(),finish.p2(),finish.p1());
        /*pathb.add_cubic(start.p1(),start.p2(),handlebs1.p2(),handlebs1.p1());
        pathb.add_cubic(handlebs1.p1(),handlebs2.p2(),finish.p2(),finish.p1());*/

    }
    else // draw cubic
    {
        path.moveTo(start.p1());
        path.cubicTo(start.p2(),finish.p2(),finish.p1());
        //pathb.add_cubic(start.p1(),start.p2(),finish.p2(),finish.p1());
    }
}

void knot_curve_pointed::draw_joint ( QPainterPath& path,
                    Node *node,
                    const TraversalInfo& ti,
                    styleinfo style )
{
    QLineF start = ti.edge_in->handle_point(ti.handle_in,style.handle_length,style.crossing_distance);
    QLineF finish = ti.edge_out->handle_point(ti.handle_out,style.handle_length,style.crossing_distance);

    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {
        QPointF cusp_point = get_cusp_point(start, finish,node,ti,style.cusp_distance);


        /*pathb.add_quad(start.p1(),start.p2(),bisect.p2());
        pathb.add_quad(bisect.p2(),finish.p2(),finish.p1());*/
        path.moveTo(start.p1());
        path.quadTo(start.p2(),cusp_point);
        path.quadTo(finish.p2(),finish.p1());

    }
    else // draw cubic
    {
        path.moveTo(start.p1());
        path.cubicTo(start.p2(),finish.p2(),finish.p1());
        //pathb.add_cubic(start.p1(),start.p2(),finish.p2(),finish.p1());
    }
}

void knot_curve_polygonal::draw_joint ( QPainterPath& path,
                    Node *node,
                    const TraversalInfo& ti,
                    styleinfo style )
{
    QLineF start = ti.edge_in->handle_point(ti.handle_in,style.handle_length,style.crossing_distance);
    QLineF finish = ti.edge_out->handle_point(ti.handle_out,style.handle_length,style.crossing_distance);

    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {
        QPointF cusp_point = get_cusp_point(start, finish,node,ti,style.cusp_distance);

        /*pathb.add_line(start.p1(),start.p2());
        pathb.add_line(start.p2(),handlebs1.p2());
        pathb.add_line(handlebs1.p2(),intersect);
        pathb.add_line(intersect,handlebs2.p2());
        pathb.add_line(handlebs2.p2(),finish.p2());
        pathb.add_line(finish.p2(),finish.p1());*/

        path.moveTo(start.p1());
        path.lineTo(start.p2());
        //path.lineTo(handlebs1.p2());
        path.lineTo(cusp_point);
        //path.lineTo(handlebs2.p2());
        path.lineTo(finish.p2());
        path.lineTo(finish.p1());

    }
    else // draw cubic
    {
        path.moveTo(start.p1());
        path.cubicTo(start.p2(),finish.p2(),finish.p1());
        //pathb.add_cubic(start.p1(),start.p2(),finish.p2(),finish.p1());
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


knot_curve_styler::knot_curve_styler()
{
    register_alias ( register_style(new knot_curve_pointed), "pointed" );
    register_alias ( register_style(new knot_curve_ogee), "ogee" );
    style_id poly = register_style(new knot_curve_polygonal);

    register_alias ( poly, "polygonal" );
}

knot_curve_styler::~knot_curve_styler()
{
    foreach( knot_curve_style* sty, styles.values() )
        delete sty;
}
QMap<knot_curve_styler::style_id,knot_curve_style*> knot_curve_styler::styles;
QMap<QString,knot_curve_styler::style_id> knot_curve_styler::names;
knot_curve_styler knot_curve_styler::singleton;

QPointF knot_curve_style:: get_cusp_point ( QLineF start,
                                 QLineF finish,
                                 Node *node,
                                 const TraversalInfo& ti,
                                 double def_dist ) const
{
    QLineF bisect(0,0,1,1);
    bisect.setP1(node->pos());
    bisect.setAngle((ti.angle_in+ti.angle_out)/2);

    QLineF handlebs(ti.edge_in->other(node)->pos(),node->pos());
    handlebs.setLength(handlebs.length()/2);
    handlebs.translate(start.p2()-handlebs.p1());
    QPointF intersect = bisect.p2();
    if ( ti.angle_delta < 350 )
    {
        handlebs.intersect(bisect,&intersect);
        bisect.setP2(intersect);
    }
    else
        bisect.setAngle((ti.angle_in+ti.angle_out)/2+180);

    bisect.setLength(def_dist);

    (void)finish;


    return bisect.p2();
}

