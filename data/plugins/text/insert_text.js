var text = window.dialog.get_text("Insert text");

if ( text.length > 0 )
{
	var graph = document.graph;// new Graph;
	var x = 0;
	var gap = 32; // space between characters
	var space = 192; // width of a space (and unknown) character
	var font = "Knotter";

	for(var i = 0; i < text.length; i++ )
	{
		var char_file = "";

		if ( ( text[i] == text[i].toUpperCase() && text[i] != text[i].toLowerCase() ) 
			|| !isNaN(text[i]*1) )
		{
			char_file = text[i]
		}
		else if ( text[i] == text[i].toLowerCase() && text[i] != text[i].toUpperCase() )
		{
			char_file = "lower_"+text[i];
		}

		if ( char_file != "" )
		{
			var graph_char = new Graph;
			graph_char.append(plugin.plugin_dir+"/fonts/"+font+"/"+char_file+".knot",
					  true, Point(x,0) ); 

			for ( var j = 0; j < graph_char.nodes.length; j++ )
				if ( graph_char.nodes[j].x > x )
					x = graph_char.nodes[j].x;
			print(char_file,x);
			x += gap;
			
			graph.merge_with(graph_char);
		}
		else
			x += space;
	}
	//document.insert(graph,"Type \""+text+"\"");
}

