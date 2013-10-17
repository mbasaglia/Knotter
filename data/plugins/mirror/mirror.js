function mirror(x,y)
{
	var graph = document.graph;
	var selected = graph.selected_nodes
	var new_nodes = [];

	for ( var i = 0; i < selected.length; i++ )
	{
		var node_pos = selected[i].pos;
		if ( x*node_pos.x != node_pos.x || y*node_pos.y != node_pos.y )
		{
			var node = graph.add_node(x*node_pos.x,y*node_pos.y);
			node.style = selected[i].style;
			new_nodes.push(node);
			
		}
	}

	for ( var i = 0; i < selected.length; i++ )
	{
		var node_pos = selected[i].pos;
		var from = graph.node_at(x*node_pos.x,y*node_pos.y);
		if ( from )
		{
			for ( var j = 0; j < selected[i].edges.length; j++ )
			{
					var other_pos = selected[i].edges[j].other(selected[i]).pos;
					var to = graph.node_at(x*other_pos.x,y*other_pos.y);
					if ( to )
					{
						var edge = graph.connect(from,to);
						edge.style = selected[i].edges[j].style;
					}
			}
		}
	}

	for ( var i = 0; i < new_nodes.length; i++ )
		new_nodes[i].selected = true;
}

if ( Dialog.exec() )
{
	document.begin_macro("Mirror");
	if ( Dialog.check_mirror_x.checked )
		mirror(-1,1);
	if ( Dialog.check_mirror_y.checked )
		mirror(1,-1);
	document.end_macro();
}
