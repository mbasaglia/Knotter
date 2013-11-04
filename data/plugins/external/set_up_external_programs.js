var items = plugin.settings.programs;

function add()
{
    var item = ["New Program","PNG",""];
    items.push(item);
    gui.table_append_row(Dialog.tableWidget, item);
}
function remove()
{ 
    var current_row = gui.table_current_column(Dialog.tableWidget);
	if ( current_row >= 0 )
    {
		Dialog.tableWidget.removeRow(current_row);
        items.splice(current_row,1);
    }
}
function item_changed()
{
    var r = gui.table_current_row(Dialog.tableWidget);
    var c = gui.table_current_column(Dialog.tableWidget);
    items[r][c] = gui.table_get(Dialog.tableWidget,r,c);
}

// populate teble
Dialog.tableWidget.clearContents();
Dialog.tableWidget.rowCount = 0;
for(var i = 0; i < items.length; i++)
{
    gui.table_append_row(Dialog.tableWidget,items[i]);
}

Dialog.tableWidget.itemChanged.connect(item_changed);
Dialog.button_add.clicked.connect(add);
Dialog.button_remove.clicked.connect(remove);
Dialog.exec();
// Prevent multiple slot calls on re-execution:
Dialog.button_add.clicked.disconnect(add);
Dialog.button_remove.clicked.disconnect(remove);
Dialog.tableWidget.itemChanged.disconnect(item_changed);

//print(JSON.stringify(Dialog.tableWidget,null,4));
/*var items = [];
for ( var i = 0; i < Dialog.tableWidget.rowCount; i++ )
{
	items.push({"name":Dialog.tableWidget.item(i,0)});
}*/

print(JSON.stringify(items,null,4));
