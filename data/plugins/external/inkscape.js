var svg = document.render.svg();
var tmpfile = system.unique_temp_file("knotter","svg");
system.write_file(tmpfile,svg);
if ( ! system.exec("inkscape",[tmpfile]) )
{
    window.dialog.critical("Cannot open Inkscape");
}