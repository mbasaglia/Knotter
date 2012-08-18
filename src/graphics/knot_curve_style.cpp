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

void knot_curve_ogee::draw_joint ( QPainterPath& path,
                    Node* node,
                    TraversalInfo ti,
                    double threshold_angle,
                    double handle_length,
                    double crossing_distance )
{
    QLineF start = ti.edge_in->handle_point(ti.handle_in,handle_length,crossing_distance);
    QLineF finish = ti.edge_out->handle_point(ti.handle_out,handle_length,crossing_distance);

    if ( ti.angle_delta > threshold_angle  ) // draw cusp
    {
        QLineF join(start.p1(),finish.p1());
        QLineF bisect((join.p1()+join.p2())/2,node->pos());
        bisect.setLength(bisect.length()+24);

        QLineF handlebs1(bisect.p2(),start.p1());
        QLineF handlebs2(bisect.p2(),finish.p1());

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
                    TraversalInfo ti,
                    double threshold_angle,
                    double handle_length,
                    double crossing_distance )
{
    QLineF start = ti.edge_in->handle_point(ti.handle_in,handle_length,crossing_distance);
    QLineF finish = ti.edge_out->handle_point(ti.handle_out,handle_length,crossing_distance);

    if ( ti.angle_delta > threshold_angle  ) // draw cusp
    {
        QLineF join(start.p1(),finish.p1());
        QLineF bisect((join.p1()+join.p2())/2,node->pos());
        bisect.setLength(bisect.length()+24);

        /*pathb.add_quad(start.p1(),start.p2(),bisect.p2());
        pathb.add_quad(bisect.p2(),finish.p2(),finish.p1());*/
        path.moveTo(start.p1());
        path.quadTo(start.p2(),bisect.p2());
        path.quadTo(finish.p2(),finish.p1());

    }
    else // draw cubic
    {
        path.moveTo(start.p1());
        path.cubicTo(start.p2(),finish.p2(),finish.p1());
        //pathb.add_cubic(start.p1(),start.p2(),finish.p2(),finish.p1());
    }
}

void knot_curve_simple_poly::draw_joint ( QPainterPath& path,
                    Node *node,
                    TraversalInfo ti,
                    double threshold_angle,
                    double handle_length,
                    double crossing_distance )
{
    QLineF start = ti.edge_in->handle_point(ti.handle_in,handle_length,crossing_distance);
    QLineF finish = ti.edge_out->handle_point(ti.handle_out,handle_length,crossing_distance);

    if ( ti.angle_delta > threshold_angle  ) // draw cusp
    {
        QLineF join(start.p1(),finish.p1());
        QLineF bisect((join.p1()+join.p2())/2,node->pos());
        bisect.setLength(bisect.length()+24);

        /*pathb.add_line(start.p1(),start.p2());
        pathb.add_line(start.p2(),bisect.p2());
        pathb.add_line(bisect.p2(),finish.p2());
        pathb.add_line(finish.p2(),finish.p1());*/

        path.moveTo(start.p1());
        path.lineTo(start.p2());
        path.lineTo(bisect.p2());
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


void knot_curve_advanced_poly::draw_joint ( QPainterPath& path,
                    Node *node,
                    TraversalInfo ti,
                    double threshold_angle,
                    double handle_length,
                    double crossing_distance )
{
    QLineF start = ti.edge_in->handle_point(ti.handle_in,handle_length,crossing_distance);
    QLineF finish = ti.edge_out->handle_point(ti.handle_out,handle_length,crossing_distance);

    if ( ti.angle_delta > threshold_angle  ) // draw cusp
    {
        QLineF join(start.p1(),finish.p1());
        QLineF bisect((join.p1()+join.p2())/2,node->pos());
        bisect.setLength(bisect.length()+24);

        QLineF handlebs1(start.p2(),bisect.p2());
        QLineF handlebs2(finish.p2(),bisect.p2());
        handlebs1.setAngle(ti.angle_in+180);
        handlebs2.setAngle(ti.angle_out+180);
        QPointF intersect = bisect.p2();
        if ( ti.angle_delta < 350 )
            handlebs1.intersect(handlebs2,&intersect);

        handlebs1.setLength(32);
        handlebs2.setLength(32);


        /*pathb.add_line(start.p1(),start.p2());
        pathb.add_line(start.p2(),handlebs1.p2());
        pathb.add_line(handlebs1.p2(),intersect);
        pathb.add_line(intersect,handlebs2.p2());
        pathb.add_line(handlebs2.p2(),finish.p2());
        pathb.add_line(finish.p2(),finish.p1());*/

        path.moveTo(start.p1());
        path.lineTo(start.p2());
        path.lineTo(handlebs1.p2());
        path.lineTo(intersect);
        path.lineTo(handlebs2.p2());
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
    register_alias ( register_style(new knot_curve_simple_poly), "simple_poly" );
    register_alias ( register_style(new knot_curve_advanced_poly), "advanced_poly" );
}

knot_curve_styler::~knot_curve_styler()
{
    foreach( knot_curve_style* sty, styles.values() )
        delete sty;
}
QMap<knot_curve_styler::style_id,knot_curve_style*> knot_curve_styler::styles;
QMap<QString,knot_curve_styler::style_id> knot_curve_styler::names;
knot_curve_styler knot_curve_styler::singleton;

