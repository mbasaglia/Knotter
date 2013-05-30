x = 0;
y = 0;
for ( j = 0; j < columns; j++ )
{
    for ( i = 0; i < rows; i++ )
    {
        node = graph.add_node(x,y);
        if ( i > 0 )
        {
            left_node = graph.node_at(x-size,y);
            graph.connect(left_node,node);
        }
        if ( j > 0 )
        {
            top_node = graph.node_at(x,y-size);
            graph.connect(top_node,node);
        }
        x += size;
    }
    x = 0;
    y += size;
}
