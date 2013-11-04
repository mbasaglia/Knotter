var items = plugin.settings.programs;

function add()
{
	Dialog.tableWidget.rowCount += 1;
}
var current_row = -1;
function remove()
{
	if ( current_row >= 0 )
		Dialog.tableWidget.removeRow(current_row);
}
function current_cell_changed(row,col,row0,col0)
{
	current_row = row;
}
function item_changed(it)
{
	print(JSON.stringify(it,null,4));
}

Dialog.tableWidget.itemChanged.connect(item_changed);
Dialog.tableWidget.currentCellChanged.connect(current_cell_changed);
Dialog.button_add.clicked.connect(add);
Dialog.button_remove.clicked.connect(remove);
Dialog.exec();
// Prevent multiple slot calls on re-execution:
Dialog.button_add.clicked.disconnect(add);
Dialog.button_remove.clicked.disconnect(remove);
Dialog.tableWidget.currentCellChanged.disconnect(current_cell_changed);
Dialog.tableWidget.itemChanged.disconnect(item_changed);

//print(JSON.stringify(Dialog.tableWidget,null,4));
/*var items = [];
for ( var i = 0; i < Dialog.tableWidget.rowCount; i++ )
{
	items.push({"name":Dialog.tableWidget.item(i,0)});
}

print(JSON.stringify(items,null,4));*/
