#include "knot_curve_script.hpp"
#include <QScriptEngine>
#include "basic_knot_graph.hpp"

void knot_curve_script::draw_joint(path_builder &path, const TraversalInfo &ti, styleinfo style)
{
    QLineF start = ti.in.edge->handle_point(ti.in.handle,style.handle_length,style.crossing_distance);
    QLineF finish = ti.out.edge->handle_point(ti.out.handle,style.handle_length,style.crossing_distance);
    QPointF cusp_point = get_cusp_point(start, finish,ti,style.cusp_distance);

    QScriptEngine engine;
    engine.globalObject().setProperty("start_handle",engine.newVariant(start));
    engine.globalObject().setProperty("finish_handle",engine.newVariant(finish));
    engine.globalObject().setProperty("cusp_point",engine.newVariant(cusp_point));
    engine.globalObject().setProperty("node_point",engine.newVariant(ti.middle->pos()));
    engine.globalObject().setProperty("angle",ti.angle_delta);
    engine.globalObject().setProperty("handle_length",style.handle_length);
    engine.globalObject().setProperty("cusp_angle",style.cusp_angle);
    engine.globalObject().setProperty("cusp_distance",style.cusp_distance);
    //engine.globalObject().setProperty("path",engine.newVariant(path));

    //engine.evaluate("path.add_cubic(start_handle.p1(),start_handle.p2(),finish_handle.p2(),finish_handle.p1());");


    if ( ti.angle_delta > style.cusp_angle  ) // draw cusp
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
    }
}
