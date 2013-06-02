if ( Dialog.exec() )
{
    graph = new graph();
    x = 0;
    y = 0;
    for ( j = 0; j < Dialog.columns.value; j++ )
    {
        for ( i = 0; i < Dialog.rows.value; i++ )
        {
            node = graph.add_node(x,y);
            if ( i > 0 )
            {
                left_node = graph.node_at(x-Dialog.size.value,y);
                graph.connect(left_node,node);
            }
            if ( j > 0 )
            {
                top_node = graph.node_at(x,y-Dialog.size.value);
                graph.connect(top_node,node);
            }
            x += Dialog.size.value;
        }
        x = 0;
        y += Dialog.size.value;
    }
    document.insert(graph);
}
