
function insert_char(char_file,x,y)
{
    var font = "Knotter";
    document.graph.append(plugin.plugin_dir+"/fonts/"+font+"/"+char_file+".knot",
                         true,Point(x,y),0.5);
    
}


var text = window.dialog.get_text("Insert text");

if ( text.length > 0 )
{
    document.begin_macro("Write \""+text+"\"");
    var x = 0;
    for(var i = 0; i < text.length; i++ )
    {
        if ( ( text[i] == text[i].toUpperCase() && text[i] != text[i].toLowerCase() ) 
            || !isNaN(text[i]*1) )
        {
            insert_char(text[i],x,0);
        }
        else if ( text[i] == text[i].toLowerCase() && text[i] != text[i].toUpperCase() )
        {
            insert_char("lower_"+text[i],x,0);
        }
        print(text[i],text[i].toLowerCase(),text[i].toUpperCase(),text[i]*1);
        x+=192;
    }
    document.end_macro();
}

