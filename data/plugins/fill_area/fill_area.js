

function convex_hull(nodes, graph, start_index )
{
    var hull_node = min_x_index;
    var angle_in = 90;
    var hull_indices = [];
    do
    {
        hull_indices.push(hull_node);
        var next_hull_node = (hull_node+1)%nodes.length;
        var angle_delta = 360;
        var next_angle = angle_in;
        //print("Hull node",hull_node,"angle:",angle_in);
        for ( var i = 0; i < nodes.length; i++ )
        {
            if ( i != hull_node )
            {
                var angle_out = (new Line(nodes[hull_node].pos,nodes[i].pos)).angle;

                var delta = angle_in - angle_out;

                if ( delta < 0 )
                    delta += 360;

                //print(i,": angle:",angle_out, "delta: ",delta);

                if ( delta < angle_delta )
                {
                    angle_delta = delta;
                    next_hull_node = i;
                    next_angle = angle_out;
                }
            }
        }
        angle_in = next_angle;
        hull_node = next_hull_node;
    }
    while(hull_node != min_x_index );

    return hull_indices;
}

var graph = document.graph;
var nodes = graph.selected_nodes;
if ( nodes.length > 2 &&  Dialog.exec() )
{

    var min_pt = new Point(Number.MAX_VALUE,Number.MAX_VALUE);
    var max_pt = new Point(Number.MIN_VALUE,Number.MIN_VALUE);
    var min_x_index = 0;
    for ( var j = 0; j < nodes.length; j++ )
    {
        if ( nodes[j].x < min_pt.x )
        {
            min_x_index = j;
            min_pt.x = nodes[j].x;
        }
        if ( nodes[j].y < min_pt.y )
            min_pt.y = nodes[j].y;
        if ( nodes[j].y > max_pt.y )
            max_pt.y = nodes[j].y;
        if ( nodes[j].x > max_pt.x )
            max_pt.x = nodes[j].x;
    }



    document.begin_macro("Fill Area");

    var polygon_points = convex_hull(nodes,graph)

    var polygon = new Polygon();
    for ( var i = 0; i < polygon_points.length; i++ )
        polygon.add_vertex(nodes[i].pos);

    //print(polygon);
    /*for ( var i = 0; i < polygon_points.length; i++ )
    {
        graph.connect(nodes[polygon_points[i]],
                      nodes[polygon_points[(i+1)%polygon_points.length]]);
    }*/

    var size = Dialog.grid_size.value;
    for ( var y = min_pt.y; y < max_pt.y; y += size )
        for ( var x = min_pt.x; x < max_pt.x; x += size )
        {
            if ( polygon.contains(x,y) )
            {
                var node = graph.add_node(x,y);
                if ( y > min_pt.y )
                {
                    var node_up = graph.node_at(x,y-size);
                    if ( node_up )
                        graph.connect(node_up,node);
                }
                if ( x > min_pt.x )
                {
                    var node_left = graph.node_at(x-size,y);
                    if ( node_left )
                        graph.connect(node_left,node);
                }
            }
        }

    document.end_macro();
}
