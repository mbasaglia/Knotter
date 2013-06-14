var max_distance=window.dialog.get_number("Maximum distance","",document.grid.size/2,0);

if ( !isNaN(max_distance) && document.graph.selected_nodes.length > 0 )
{
	document.begin_macro("Randomize");

	for ( var i = 0; i < document.graph.selected_nodes.length; i++ )
	{
		var node = document.graph.selected_nodes[i];
		var line = new Line(node.pos,Point(node.pos.x+max_distance,node.pos.y));
		line.angle = 360 * Math.random();
		node.pos = line.pointAt(Math.random());
	}

	document.end_macro();
}