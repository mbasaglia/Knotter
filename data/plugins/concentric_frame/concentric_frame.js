
var graph = document.graph;
var nodes = graph.selected_nodes;

if ( nodes.length < 3 )
{
	window.dialog.information("Please select at leat 3 nodes","Concentric Frame");
}
else if ( Dialog.exec() )
{


    var current = nodes[0];
    var previous = current;
    var loop_nodes = [];

    for ( var i = 0; i < nodes.length; i++ )
    {
        loop_nodes.push(current);

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


	document.begin_macro("Concentic Frames");

	/// get outer frame and midpoint
	var outer_frame = [];
	var midpoint = new Point(0,0);
	for ( var i = 0; i < loop_nodes.length; i++ )
	{

		var next_node = graph.add_node(loop_nodes[i].pos);
		outer_frame.push(next_node);

		if ( i > 0 )
			graph.connect(outer_frame[i-1],next_node);

		midpoint.x += loop_nodes[i].x / loop_nodes.length;
		midpoint.y += loop_nodes[i].y / loop_nodes.length;

	}
	graph.connect(outer_frame[outer_frame.length-1],outer_frame[0]);
	
	// get average distance from midpoint
	var base_length = 0;
	for ( var i = 0; i < outer_frame.length; i++ )
	{
		base_length += distance(midpoint,outer_frame[i].pos) / outer_frame.length;
	}

	if ( Dialog.check_delete_original.checked )
		for ( var j = 0; j < nodes.length; j++ )
			graph.remove_node(nodes[j]);
	


	var n_steps = Dialog.spin_iterations.value;
	var width = Dialog.spin_distance.value;
	var outest_frame = outer_frame
	var step = width/n_steps;
	for ( var j = 1; j <= n_steps; j++ )
	{
		// Build the new polygon scaling from the middle point
		var inner_frame = [];
		var factor = (base_length-j*step) / base_length;
		for ( var i = 0; i < outer_frame.length; i++ )
		{
			var offsetter = new Line ( midpoint, outest_frame[i].pos );
			offsetter.length *= factor;
			var next_node = graph.add_node(offsetter.p2);
			inner_frame.push(next_node);
			if ( i > 0 )
				graph.connect(inner_frame[i-1],next_node);
			if ( Dialog.check_connected.checked )
				graph.connect(outer_frame[i],next_node);
		}
		graph.connect(inner_frame[inner_frame.length-1],inner_frame[0]);
	
		outer_frame = inner_frame;
	}

	document.end_macro();

}