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
#include "knot_curve_script.hpp"
#include <QScriptEngine>
#include "basic_knot_graph.hpp"
#include "script_wrappers.hpp"

#include <QDebug>

script_path_builder::script_path_builder(path_builder* pb) : pb(pb)
{
    qRegisterMetaType<script_point>("script_point");
    qRegisterMetaType<script_line>("script_line");
}


QScriptValue build_line (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() == 2 ) // point constructor
    {
        script_point arg1 = qscriptvalue_cast<script_point> (context->argument(0));
        script_point arg2 = qscriptvalue_cast<script_point> (context->argument(1));

        return engine->toScriptValue(script_line(arg1,arg2));
    }
    else if ( context->argumentCount() == 1 ) // copy constructor
    {
        return engine->toScriptValue( qscriptvalue_cast<script_line> (context->argument(0)) );
    }
    else // empty line
        return engine->toScriptValue( script_line() );
}

QScriptValue line_to_script(QScriptEngine *engine, const script_line &l)
{
    QScriptValue obj = engine->newQObject(new script_line(l));
    return obj;
}

void line_from_script(const QScriptValue &obj, script_line &l)
{
    l.setP1 ( obj.property("p1").toVariant().value<script_point>() );
    l.setP2 ( obj.property("p2").toVariant().value<script_point>() );
}

QScriptValue build_point (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() == 2 ) // point constructor
    {
        double x = context->argument(0).toNumber();
        double y = context->argument(1).toNumber();

        return engine->toScriptValue(script_point(x,y));
    }
    else if ( context->argumentCount() == 1 ) // copy constructor
    {
        return engine->toScriptValue( qscriptvalue_cast<script_point> (context->argument(0)) );
    }
    else // empty point
        return engine->toScriptValue( script_point() );
}

QScriptValue point_to_script(QScriptEngine *engine, const script_point &p)
{
    QScriptValue obj = engine->newObject();
    obj.setProperty("x",p.x());
    obj.setProperty("y",p.y());
    return obj;
}

void point_from_script(const QScriptValue &obj, script_point &p)
{
    p.setX ( obj.property("x").toNumber() );
    p.setY ( obj.property("y").toNumber() );
}

QScriptValue subtract_points (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() == 2 )
    {
        script_point arg1 = qscriptvalue_cast<script_point> (context->argument(0));
        script_point arg2 = qscriptvalue_cast<script_point> (context->argument(1));

        return engine->toScriptValue(arg1-arg2);
    }
    else
        return  engine->toScriptValue(script_point());
}

QScriptValue opposite_point (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() >= 1 )
    {
        script_point p = qscriptvalue_cast<script_point> (context->argument(0));

        return engine->toScriptValue(-p);
    }
    else
        return  engine->toScriptValue(script_point());
}


void knot_curve_script::draw_joint(path_builder &pathb, const TraversalInfo &ti, styleinfo style)
{
    script_line start_handle = ti.in.edge->handle_point(ti.in.handle,style.handle_length,style.crossing_distance);
    script_line finish_handle = ti.out.edge->handle_point(ti.out.handle,style.handle_length,style.crossing_distance);
    script_point cusp_point = get_cusp_point(start_handle, finish_handle,ti,style.cusp_distance);
    script_point node_point = ti.middle->pos();
    QScriptEngine engine;

    qScriptRegisterMetaType(&engine, line_to_script, line_from_script);
    qScriptRegisterMetaType(&engine, point_to_script, point_from_script);

    engine.globalObject().setProperty("start_handle",engine.toScriptValue(start_handle));
    engine.globalObject().setProperty("finish_handle",engine.toScriptValue(finish_handle));
    engine.globalObject().setProperty("cusp_point",engine.toScriptValue(cusp_point));
    engine.globalObject().setProperty("node_point",engine.toScriptValue(node_point));
    engine.globalObject().setProperty("angle",ti.angle_delta);
    engine.globalObject().setProperty("handle_length",style.handle_length);
    engine.globalObject().setProperty("cusp_angle",style.cusp_angle);
    engine.globalObject().setProperty("cusp_distance",style.cusp_distance);

    engine.globalObject().setProperty("line", engine.newFunction(build_line));
    engine.globalObject().setProperty("point", engine.newFunction(build_point));
    engine.globalObject().setProperty("diff", engine.newFunction(subtract_points));
    engine.globalObject().setProperty("opposite", engine.newFunction(opposite_point));

    script_path_builder path(&pathb);
    engine.globalObject().setProperty("path",engine.newQObject(&path));

    engine.evaluate(
    "if ( angle > cusp_angle ) { "
        "handle = new line(start_handle.p1,finish_handle.p1);"
        "handle.translate(cusp_point);"
        "handle.translate(opposite(start_handle.p1));"
        "handle.length = handle_length/2;"
        "h2 = new point(handle.p2);"
        "handle.length = -handle_length/2;"
        "h1 = new point(handle.p2);"
        "path.add_cubic ( start_handle.p1, start_handle.p2, h1, cusp_point );"
        "path.add_cubic ( finish_handle.p1, finish_handle.p2, h2, cusp_point );"
    "} else {"
        "path.add_cubic(start_handle.p1,start_handle.p2,finish_handle.p2,finish_handle.p1);"
    "}"
    );

    //if ( ti.angle_delta > style.cusp_angle  )
    {
        /*((script_path_builder*)engine.globalObject().property("path").toQObject())
            -> add_line(
                engine.globalObject().property("finish_handle").property("p2").toVariant().value<script_point>(),
                engine.globalObject().property("start_handle").property("p2").toVariant().value<script_point>()
            );

        ((script_path_builder*)engine.globalObject().property("path").toQObject())
            -> add_line(
                engine.globalObject().property("start_handle").property("p2").toVariant().value<script_point>(),
                //engine.globalObject().property("cusp_point").toVariant().value<script_point>()
                *((script_point*)engine.globalObject().property("cusp_point").toQObject())
            );

        ((script_path_builder*)engine.globalObject().property("path").toQObject())
            -> add_line(
                engine.globalObject().property("finish_handle").property("p2").toVariant().value<script_point>(),
                //engine.globalObject().property("cusp_point").toVariant().value<script_point>()
                *((script_point*)engine.globalObject().property("cusp_point").toQObject())
            );*/

        //path.add_line(start_handle.p1(),start_handle.p2());
        //path.add_line(start_handle.p2(),cusp_point);
        //path.add_line(cusp_point,finish_handle.p2());
        //path.add_line(finish_handle.p2(),finish_handle.p1());

    }
    /*else
    {

        ((script_path_builder*)engine.globalObject().property("path").toQObject())
            -> add_cubic(
                engine.globalObject().property("start_handle").property("p1").toVariant().value<script_point>(),
                engine.globalObject().property("start_handle").property("p2").toVariant().value<script_point>(),
                engine.globalObject().property("finish_handle").property("p1").toVariant().value<script_point>(),
                engine.globalObject().property("finish_handle").property("p2").toVariant().value<script_point>()
            );
    }*/

    //qDebug() << engine.evaluate("cusp_point.x").toNumber();

    /*if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
    {

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
    }*/
}
