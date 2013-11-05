var items =[];
for ( var i = 0; i < plugin.settings.programs.length; i++ )
    items.push(plugin.settings.programs[i][0]);

var item = window.dialog.get_item("Select program","Launch External Program",items);
if (  item != "" )
{
    run_script(plugin.plugin_dir+"/common.js");
    execute_external(item);
}
