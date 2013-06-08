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

// When angle is large enough, draw the cusp
if ( angle > cusp_angle ) 
{
    // Create a "handle" that on cusp_point with size handle_length to determine the control points
    var handle = new Line(start_handle.p1,finish_handle.p1);
    handle.translate(cusp_point);
    handle.translate(opposite(start_handle.p1));
    handle.length = handle_length;
    var h2 = handle.p2;
    handle.length = -handle_length;
    var h1 = handle.p2;
    // Use the control points to render the cusp
    path.add_cubic ( start_handle.p1, start_handle.p2, h1, cusp_point );
    path.add_cubic ( finish_handle.p1, finish_handle.p2, h2, cusp_point );
}
else
{
    // Don't draw a cusp, just a curve from start_handle to finish_handle
    
    if ( distance(start_handle.p1,finish_handle.p1) < start_handle.length + finish_handle.length )
    {
        // The two edges are very close together, avoid artifacts with a simpler curve
        var midpoint = Point( (start_handle.p2.x+finish_handle.p2.x)/2,
                              (start_handle.p2.y+finish_handle.p2.y)/2 );
        path.add_quad(start_handle.p1,midpoint,finish_handle.p1);
    }
    else
    {
        // There's enough room to draw a cubic curve
        path.add_cubic(start_handle.p1,start_handle.p2,finish_handle.p2,finish_handle.p1);
    }
} 
