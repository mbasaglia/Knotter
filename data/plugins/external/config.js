var items = plugin.settings.programs;
var script_table = gui.table_widget(Dialog.tableWidget);

function add()
{
    var item = ["New Program","PNG",""];
    items.push(item);
    script_table.append_row(item);
}
function remove()
{ 
    var current_row = script_table.current_column();
	if ( current_row >= 0 )
    {
		Dialog.tableWidget.removeRow(current_row);
        items.splice(current_row,1);
    }
}
function item_changed( r, c, text )
{
    items[r][c] = text;
}

// populate teble
Dialog.tableWidget.clearContents();
Dialog.tableWidget.rowCount = 0;
for(var i = 0; i < items.length; i++)
{
    script_table.append_row(items[i]);
}

script_table.value_changed.connect(item_changed);
Dialog.button_add.clicked.connect(add);
Dialog.button_remove.clicked.connect(remove);
Dialog.exec();
// Prevent multiple slot calls on re-execution:
Dialog.button_add.clicked.disconnect(add);
Dialog.button_remove.clicked.disconnect(remove);

//print(JSON.stringify(items,null,4));
