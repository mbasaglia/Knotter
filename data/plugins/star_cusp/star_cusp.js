/*
The scripting language is QtScript aka ECMAScript aka JavaScript 

Functions:
    * line() or new line() to create a line object (see QLine for reference)
    * point() or new point() to create a point (has properties 'x' and 'y')
    * opposite(p) returns point(-p.x,-p.y)

Variables:
    * angle         [real] - The angle between input and output edge
    * cusp_angle    [real] - Style setting, if angle > cusp_angle, draw a cusp
    * handle_length [real] - Style setting, "curve" in the knot style dialog
    * start_handle  [line] - starting point for lines, start_handle.p1 will be connected to the edge crossing
    * finish_handle [line] - ending point for lines, finish_handle.p1 will be connected to the edge crossing
    * cusp_point    [point]- pre-computed cusp point location
    * node_point    [point]- position of the node between the two edges
    * path          [path] - object used to build the path
*/
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
else
{
    // Don't draw a cusp, just a cubic curve from start_handle to finish_handle
    path.add_cubic(start_handle.p1,start_handle.p2,finish_handle.p2,finish_handle.p1);
} 
