Dialog.cell_size.value = document.grid.size;

if ( Dialog.exec() )
{
    graph = new Graph();
    var x = 0;
    var y = 0;
    var size = Dialog.cell_size.value
    for ( var j = 0; j < Dialog.columns.value; j++ )
    {
        for ( var i = 0; i < Dialog.rows.value; i++ )
        {
            var node = graph.add_node(x,y);
            if ( i > 0 )
            {
                var left_node = graph.node_at(x-size,y);
                graph.connect(left_node,node);
            }
            if ( j > 0 )
            {
                var top_node = graph.node_at(x,y-size);
                graph.connect(top_node,node);
            }
            x += size;
        }
        x = 0;
        y += size;
    }
    document.insert(graph);
}
