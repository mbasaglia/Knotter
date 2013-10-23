var png = document.render.raster();
var tmpfile = system.unique_temp_file("knotter","png");
system.write_file(tmpfile,png);
if ( ! system.exec("gimp",[tmpfile]) )
{
    window.dialog.critical("Cannot open GIMP");
}