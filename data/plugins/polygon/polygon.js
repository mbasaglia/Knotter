if ( Dialog_Insert_Polygon.exec() )
{
    var graph = new Graph();
    
    var last = null;
    var first = null;

    var radius = (document.grid.enabled ? document.grid.size : 32)*3;
    
    var sides = Dialog_Insert_Polygon.spin_sides.value;

    var middle = null;
    if ( Dialog_Insert_Polygon.check_middle_node.checked )
        middle = graph.add_node(0,0);

    for ( var i = 0; i < sides; i++ )
    {
        var angle = 2*Math.PI*i/sides;

        var next = graph.add_node(radius*Math.cos(angle),-radius*Math.sin(angle));

        if ( !first )
            first = next;

        if ( middle )
            graph.connect(middle,next);

        if ( last )
            graph.connect(last,next);

        last = next;

    }
    graph.connect(last,first);

    
    document.insert(graph,"Insert Polygon");
}
