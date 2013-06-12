
var radius = window.dialog.get_number("Radius","",0,0);

if ( !isNaN(radius) )
{
	for ( var i = 0; i < document.graph.nodes.length; i++ )
	{
		var base = document.graph.nodes[i];
		var colliding = document.graph.nodes_at(base.pos,radius);
		for ( var j = 0; j < colliding.length; j++ )
		{
			if ( colliding[j] != base )
			{
				for ( var k = 0; k < colliding[j].edges.length; k++ )
				{
					if ( colliding[j].edges[k].other(colliding[j]) != base )
						document.graph.connect(base,colliding[j].edges[k].other(colliding[j]))
				}
				document.graph.remove_node(colliding[j]);
			}
		}
	}
}