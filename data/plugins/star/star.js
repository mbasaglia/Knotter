if ( Dialog_Insert_Star.exec() )
{
	var graph = new Graph();
    
	var radius = Dialog_Insert_Star.spin_inner_radius.value;
	var out_radius = Dialog_Insert_Star.spin_outer_radius.value;
    
	var sides = Dialog_Insert_Star.spin_sides.value;

	var edges = [];

	var first = null;
	var last = null;

	for ( var i = 0; i < sides; i++ )
	{
		var angle = 2*Math.PI*i/sides;

		var next = graph.add_node(radius*Math.cos(angle),-radius*Math.sin(angle));

		if ( !first )
			first = next;

		if ( last )
			edges.push(graph.connect(last,next));

		last = next;

	}

	edges.push(graph.connect(last,first));

	for ( var i = 0; i < edges.length; i++ )
	{
		
		var angle = 2*Math.PI*i/edges.length+Math.PI/edges.length;

		var node = graph.add_node(out_radius*Math.cos(angle),
											-out_radius*Math.sin(angle));

		graph.connect(edges[i].vertex1,node);
		graph.connect(node,edges[i].vertex2);

	}		


	document.insert(graph,"Insert Star");
}
