function traverse(edge,handle,style)
{
    var next = 0;
    if ( handle == TOP_RIGHT )
        next = BOTTOM_LEFT;
    else if ( handle == BOTTOM_RIGHT )
        next = TOP_LEFT;
    else if ( handle == BOTTOM_LEFT )
        next = TOP_RIGHT;
    else if ( handle == TOP_LEFT )
        next = BOTTOM_RIGHT;

   if ( handle == TOP_LEFT || next == TOP_LEFT )
       path.add_line(handle_line(edge,handle,style).p1,
                     handle_line(edge,next,style).p1
                     );
    return next;
}

function deg2rad(deg)
{
    return deg / 180 * Math.PI;
}

function handle_line(edge,handle,style)
{

    var handle_angle = 0;
    if ( handle == TOP_RIGHT )
        handle_angle = Math.PI/4.0;
    else if ( handle == TOP_LEFT )
        handle_angle = Math.PI*3.0/4.0;
    else if ( handle == BOTTOM_LEFT )
        handle_angle = Math.PI*5.0/4.0;
    else if ( handle == BOTTOM_RIGHT )
        handle_angle = Math.PI*7.0/4.0;

    handle_angle += deg2rad(edge.line.angle);
    var p1 = edge.line.pointAt(style.slide);
    p1.x = p1.x+style.gap/2*Math.cos(handle_angle);
    p1.y = p1.y-style.gap/2*Math.sin(handle_angle);

    var p2 = new Point(
            p1.x+style.curve*Math.cos(handle_angle),
            p1.y-style.curve*Math.sin(handle_angle)
    );

    return new Line(p1,p2);
}