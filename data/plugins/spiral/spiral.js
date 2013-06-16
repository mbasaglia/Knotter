
if ( Dialog.exec() )
{
	var prev_node = null;
	var graph = new Graph;
	for ( var a = Dialog.start_angle.value; a <= Dialog.end_angle.value; a+= Dialog.step_angle.value )
	{
		var theta = Math.PI*a/180;
		var radius = theta*Dialog.turn_size.value/(Math.PI*2);
		var node = graph.add_node(radius*Math.cos(theta),-radius*Math.sin(theta));
		if ( prev_node )
			graph.connect(prev_node,node);
		prev_node = node;
	}

	document.insert(graph,"Insert Spiral");
}