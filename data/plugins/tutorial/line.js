
// Ask the user for a number. Default value is 4 and minimum is 1
var length = window.dialog.get_integer("Number of segments","Segmented line", 4, 1 );

// If the user canceled the dialog  length is set to NaN
if ( !isNaN(length) )
{
    // Place all the edits to the document in a macro called "Segmented line"
    document.begin_macro("Segmented line");
    
    for ( var i = 0; i < length; i++ )
    {
        // Create a new node
        var current_node  = document.graph.add_node(i*document.grid.size,0);
        
        // From the second iteration connect to the previous node
        if ( i > 0 )
        {
            var previous_node = document.graph.node_at((i-1)*document.grid.size,0);
            document.graph.connect(current_node,previous_node);
        }
    }
    
    // End the macro
    document.end_macro();
}