/*
	Find the convex hull, references as kept as indices
   Gift wrap algorithm
  */
function convex_hull(nodes, start_index )
{
	 // Start from the index with minimum x, pointing upwards 
    var hull_node = start_index;
    var angle_in = 90;
    var polygon = new Polygon();
    // Keep looping 'till we get back to the starting point
    do
    {
        // Add current node to the hull
        polygon.add_vertex(nodes[hull_node].pos);
        // Pick the next point
        var next_hull_node = (hull_node+1)%nodes.length;
        var angle_delta = 360;
        var next_angle = angle_in;
        // Find the point with minimum angle difference
        for ( var i = 0; i < nodes.length; i++ )
        {
            if ( i != hull_node )
            {
                var angle_out = (new Line(nodes[hull_node].pos,nodes[i].pos)).angle;

                var delta = angle_in - angle_out;

                if ( delta < 0 )
                    delta += 360;

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
    while(hull_node != start_index );

    return polygon;
}

function edge_loop(nodes)
{
    var current = nodes[0];
    var previous = current;
    var poly_indices = [];

    var polygon = new Polygon();

    // Build polygon
    for ( var i = 0; i <= nodes.length; i++ )
    {
        polygon.add_vertex(current.pos);

        var found = false;
        for ( var j = 0; j < current.edges.length; j++ )
        {
            var other = current.edges[j].other(current);
            if ( previous != other )
            {
                previous = current;
                current = other;
                found = true;
                break;
            }
        }
        if ( !found )
            break;
    }

    return polygon;
}

var graph = document.graph;
var nodes = graph.selected_nodes;

Dialog.grid_size.value = document.grid.size
if ( nodes.length < 3 )
{
    window.dialog.information("Please select at leat 3 nodes","Fill Area");
}
else if ( Dialog.exec() )
{

    // build bounding box
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



    // get the polygon
    var polygon = null;
    if ( Dialog.algorithm.currentIndex == 1 )
        polygon = edge_loop(nodes);
    else
        polygon = convex_hull(nodes,min_x_index);

   // If the user wants an outline, get the inset polygon
	var inset_polygon = null;
	if ( Dialog.outline.checked )
	{
		// Find the polygon center
		var midpoint = new Point(0,0);
		for ( var i = 0; i < polygon.vertices.length; i++ )
		{
			midpoint.x += polygon.vertices[i].x / polygon.vertices.length;
			midpoint.y += polygon.vertices[i].y / polygon.vertices.length;
		}

		// Build the new polygon scaling from the middle point
		inset_polygon = new Polygon;
		for ( var i = 0; i < polygon.vertices.length; i++ )
		{
			var offsetter = new Line ( midpoint, polygon.vertices[i] );
			if ( Dialog.outline.outset.checked )
			{
				offsetter.length += Dialog.outline.thickness.value
				// if outset, the new polygon will be larger, thus increase the bounding box			
        		if ( offsetter.p2.x < min_pt.x )
           	 	min_pt.x = offsetter.p2.x;
        		if ( offsetter.p2.y < min_pt.y )
         	   min_pt.y = offsetter.p2.y;
        		if ( offsetter.p2.y > max_pt.y )
      	      max_pt.y = offsetter.p2.y;
   	     	if ( offsetter.p2.x > max_pt.x )
	            max_pt.x = offsetter.p2.x;
			}
			else
				offsetter.length -= Dialog.outline.thickness.value
			inset_polygon.add_vertex(offsetter.p2);
		}
		// If outset, swap external and internal polygons
		if ( Dialog.outline.outset.checked )
		{
			var swap = polygon;
			polygon = inset_polygon;
			inset_polygon = swap;;
		}
	}

    // Start filling the area
    if ( polygon.vertices.length > 2 )
    {
        document.begin_macro("Fill Area");

        var size = Dialog.grid_size.value;
        var new_nodes = new Array();

        var max_steps = ((max_pt.y-min_pt.y)/size)*((max_pt.x-min_pt.y)/size);
        var progress_dialog = window.dialog.progress_dialog(
                    "Filling area",max_steps,"Interrupt");
        progress_dialog.minimumDuration = 2000;
			
			// For each point in the grid
        for ( var y = min_pt.y, yi = 0; y <= max_pt.y; y += size, yi++ )
        {
            new_nodes.push([]);
            for ( var x = min_pt.x, xi = 0; x <= max_pt.x; x += size, xi++ )
            {
                progress_dialog.value = yi*((max_pt.x-min_pt.y)/size)+xi;
                
						// if the point is inside the designated area, draw it
						var outline = !inset_polygon || !inset_polygon.contains(x,y);
                if ( outline && ( 
							polygon.contains(x,y) || polygon.contains(x-1,y-1) ||
                    polygon.contains(x-1,y+1) || polygon.contains(x+1,y-1) 
						)	)
                {
                    var node = graph.add_node(x,y);
                    new_nodes[yi].push(node);
                    if ( yi > 0 )
                    {
                        var node_up = new_nodes[yi-1][xi];
                        if ( node_up )
                            graph.connect(node_up,node);
                    }
                    if ( x > min_pt.x )
                    {
                        var node_left = new_nodes[yi][xi-1];
                        if ( node_left )
                            graph.connect(node_left,node);
                    }
                }
                else
                    new_nodes[yi].push(null);

            }
        }

			// Optionally remove old nodes
        if ( Dialog.delete_original.checked )
        {
            for ( var i = 0; i < nodes.length; i++ )
            {
                graph.remove_node(nodes[i]);
            }
        }

			progress_dialog.value = max_steps;

        document.end_macro();
    }
}
