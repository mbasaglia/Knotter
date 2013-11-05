/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2013  Mattia Basaglia

Knotter is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Knotter is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "script_qtablewidget.hpp"

Script_QTableWidget::Script_QTableWidget(QTableWidget *wrapped, QObject *parent) :
    QObject(parent), table(wrapped)
{
    connect(table,SIGNAL(itemChanged(QTableWidgetItem*)),
            SLOT(emit_value_changed(QTableWidgetItem*)));
}

void Script_QTableWidget::set_value(int row, int column, QString value)
{

    QTableWidgetItem* item = table->item(row, column);
    if ( !item )
    {
        item = new QTableWidgetItem(value);
        table->setItem(row,column,item);
    }
    else
        item->setText(value);
}

QString Script_QTableWidget::get_value(int row, int column)
{
    QTableWidgetItem* item = table->item(row, column);
    if ( item )
        return item->text();
    return QString();
}

void Script_QTableWidget::append_row( QStringList value)
{
    table->setRowCount(table->rowCount()+1);
    for( int i = 0; i < value.size(); i++ )
    {
        table->setItem(table->rowCount()-1,i,new QTableWidgetItem(value[i]));
    }
}

int Script_QTableWidget::current_row()
{
    return table->currentRow();
}

int Script_QTableWidget::current_column()
{
    return table->currentColumn();
}




QScriptValue script_create_tablewidget_wrapper (QScriptContext *context, QScriptEngine *engine)
{
    if ( context->argumentCount() == 1 ) // copy constructor
    {
        QTableWidget *table = qobject_cast<QTableWidget*>(context->argument(0).toQObject());
        if ( table )
        return engine->newQObject( new Script_QTableWidget(table),QScriptEngine::ScriptOwnership );
    }
    return context->throwError(QScriptContext::TypeError,
                               Script_QTableWidget::tr("Argument is not a QTableWidget object"));
}


void Script_QTableWidget::emit_value_changed(QTableWidgetItem *it)
{
    emit value_changed(it->row(),it->column(),it->text());
}
