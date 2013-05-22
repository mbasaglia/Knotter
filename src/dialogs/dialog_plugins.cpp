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

#include "dialog_plugins.hpp"
#include "resource_manager.hpp"

Dialog_Plugins::Dialog_Plugins(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    load_plugins();
}

void Dialog_Plugins::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}
Q_DECLARE_METATYPE(Plugin*)

void Dialog_Plugins::load_plugins()
{
    listWidget->clear();
    foreach(Plugin* p, Resource_Manager::plugins())
    {
        QListWidgetItem *item = new QListWidgetItem(p->icon(), p->string_data("name"));
        item->setData(Qt::UserRole,QVariant::fromValue(p));
        set_item_enabled(item,p->enabled());
        listWidget->addItem(item);
    }
}

void Dialog_Plugins::on_listWidget_currentRowChanged(int currentRow)
{
    Plugin* p = listWidget->item(currentRow)->data(Qt::UserRole).value<Plugin*>();
    if ( p )
    {
        label_title->setText(p->string_data("name"));
        text_description->setPlainText(p->string_data("description"));
        check_enable->setChecked(p->enabled());
        tableWidget->clearContents();
        tableWidget->setRowCount(0);
        foreach(QString k, p->metadata().keys())
        {
            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);
            QTableWidgetItem *it = new QTableWidgetItem(k);
            it->setFlags(Qt::ItemIsEnabled);
            tableWidget->setItem(row,0,it);
            it = new QTableWidgetItem(p->string_data(k));
            it->setFlags(Qt::ItemIsEnabled);
            tableWidget->setItem(row,1,it);
        }
    }
}

void Dialog_Plugins::set_item_enabled(QListWidgetItem *it, bool enabled)
{
    QBrush b = it->foreground();
    QColor c = b.color();
    c.setAlpha(enabled?255:128);
    b.setColor(c);
    it->setForeground(b);
    QFont f = it->font();
    f.setItalic(!enabled);
    it->setFont(f);
    //it->setCheckState(enabled?Qt::Checked:Qt::Unchecked);
}

void Dialog_Plugins::on_check_enable_clicked(bool checked)
{
    Resource_Manager::plugins()[listWidget->currentIndex().row()]->enable(checked);
    set_item_enabled(listWidget->currentItem(),checked);
}
