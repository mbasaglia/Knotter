function execute_external(program_id)
{
    var found = 0;
    for ( var i = 0; i < plugin.settings.programs.length; i++ )
    {
        item = plugin.settings.programs[i];
        if ( item[0] == program_id )
        {
            found = 1;
            var format_upper = item[1].toUpperCase();
            var format_lower = item[1].toLowerCase();
            var command = item[2];
            var data;
            if ( format_lower == "svg" )
                data = document.render.svg();
            else
                data = document.render.raster(0,0,format_upper);
            var tmpfile = system.unique_temp_file("knotter",format_lower);
            system.write_file(tmpfile,data);
            if ( ! system.exec(command,[tmpfile]) )
            {
                window.dialog.critical("Cannot launch "+program_id+": "+command);
            }
        }
    }
    if ( ! found )
    {
        throw "Program "+program_id+" not found";
    }
}
