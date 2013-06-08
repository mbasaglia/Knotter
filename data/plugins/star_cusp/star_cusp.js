
if ( angle > cusp_angle ) 
{
    cusp_line = Line(node_point,cusp_point);
    //path.add_line(node_point,cusp_point);
    
    tip_line1 = Line(cusp_line)
    tip_line1.angle = cusp_line.angle-direction*angle/4;
    in_line1 = Line(cusp_line);
    in_line1.angle = cusp_line.angle-direction*angle/8;
    in_line1.length /= 2;
    base_line1 = Line(in_line1);
    base_line1.angle = cusp_line.angle-direction*angle*3/8;
    path.add_line(cusp_point,in_line1.p2);
    path.add_line(in_line1.p2,tip_line1.p2);
    path.add_line(tip_line1.p2,base_line1.p2);
    path.add_quad(base_line1.p2,start_handle.p2,start_handle.p1);
    
    tip_line2 = Line(cusp_line)
    tip_line2.angle = cusp_line.angle+direction*angle/4;
    in_line2 = Line(cusp_line);
    in_line2.angle = cusp_line.angle+direction*angle/8;
    in_line2.length /= 2;
    base_line2 = Line(in_line1);
    base_line2.angle = cusp_line.angle+direction*angle*3/8;
    path.add_line(cusp_point,in_line2.p2);
    path.add_line(in_line2.p2,tip_line2.p2);
    path.add_line(tip_line2.p2,base_line2.p2);
    path.add_quad(base_line2.p2,finish_handle.p2,finish_handle.p1);
    
}
else if ( distance(start_handle.p1,finish_handle.p1) < start_handle.length + finish_handle.length )
{
    var midpoint = Point( (start_handle.p2.x+finish_handle.p2.x)/2,
                          (start_handle.p2.y+finish_handle.p2.y)/2 );
    path.add_quad(start_handle.p1,midpoint,finish_handle.p1);
}
else
{
    path.add_cubic(start_handle.p1,start_handle.p2,finish_handle.p2,finish_handle.p1);
}
