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
#include "wizard_create_plugin.hpp"

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
            reload_combo();
            break;
        default:
            break;
    }
}

void Dialog_Plugins::load_plugins()
{
    frame_info->setVisible(!Resource_Manager::plugins().empty());
    label_empty->setVisible(Resource_Manager::plugins().empty());

    listWidget->blockSignals(true);
    listWidget->clear();
    listWidget->blockSignals(false);
    foreach(Plugin* p, Resource_Manager::plugins())
    {
        QListWidgetItem *item = new QListWidgetItem(p->icon(), p->string_data("name"));
        if ( item->icon().isNull() )
            item->setIcon(QIcon::fromTheme("text-x-script"));
        item->setData(Qt::UserRole,QVariant::fromValue(p));
        item->setHidden(false);
        set_item_enabled(item,p->is_enabled());
        if ( !p->is_valid() )
            set_item_errored(item);
        listWidget->addItem(item);
    }

    if ( listWidget->count() > 0)
        listWidget->setCurrentRow(0);

    reload_combo();
}

void Dialog_Plugins::reload_combo()
{
    combo_category->blockSignals(true);
    combo_category->clear();

    combo_category->addItem(tr("All"));

    QStringList categories;

    foreach(Plugin* p, Resource_Manager::plugins())
    {
        if ( !categories.contains(p->string_data("category")) )
            categories << p->string_data("category");
    }

    foreach(QString s, categories)
    {
        combo_category->addItem(s);
    }

    combo_category->blockSignals(false);
    combo_category->setCurrentIndex(0);
}

void Dialog_Plugins::on_listWidget_currentRowChanged(int currentRow)
{
    Plugin* p = listWidget->item(currentRow)->data(Qt::UserRole).value<Plugin*>();
    if ( p )
    {
        label_icon->setPixmap(p->icon().pixmap(22));
        label_title->setText(p->string_data("name"));
        text_description->setPlainText(p->string_data("description"));


        check_enable->setEnabled(p->is_valid());
        check_enable->setChecked(p->is_enabled());

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

void Dialog_Plugins::set_item_errored(QListWidgetItem *it)
{
    it->setForeground(Qt::darkRed);
}

void Dialog_Plugins::on_check_enable_clicked(bool checked)
{
    Resource_Manager::plugins()[listWidget->currentIndex().row()]->enable(checked);
    set_item_enabled(listWidget->currentItem(),checked);
}

void Dialog_Plugins::on_button_reload_clicked()
{
    Plugin *old = current_plugin();
    QString file;
    if ( old )
        file = old->string_data("plugin_file");
    Resource_Manager::reload_plugins();
    load_plugins();
    for ( int i = 0; i < listWidget->count(); i++ )
    {
        if ( plugin(i)->string_data("plugin_file") == file )
        {
            listWidget->setCurrentRow(i);
            break;
        }
    }
}

void Dialog_Plugins::on_button_reload_2_clicked()
{
    Plugin* p = current_plugin();
    if ( p )
        p->reload_script_file();
}


Plugin *Dialog_Plugins::current_plugin()
{
    return plugin(listWidget->currentIndex().row());
}

Plugin *Dialog_Plugins::plugin(int i)
{
    if ( i >= 0 && i < listWidget->count() )
        return listWidget->item(i)->data(Qt::UserRole).value<Plugin*>();

    return nullptr;
}

void Dialog_Plugins::on_button_edit_clicked()
{
    emit edit_plugin(current_plugin());
}

void Dialog_Plugins::on_button_create_clicked()
{
    Wizard_Create_Plugin(this).exec();
}

void Dialog_Plugins::on_check_enable_toggled(bool checked)
{
    check_enable->setText(checked?tr("Enabled"):tr("Disabled"));
}

void Dialog_Plugins::on_combo_category_currentIndexChanged(int index)
{
    for(int i = 0; i < listWidget->count(); i++ )
    {
        QListWidgetItem* item = listWidget->item(i);

        if ( index == 0 )
        {
            item->setHidden(false);
        }
        else
        {
            Plugin* p = item->data(Qt::UserRole).value<Plugin*>();
            item->setHidden( p->string_data("category") != combo_category->currentText() );
        }
    }
}

void Dialog_Plugins::on_button_clear_settings_clicked()
{
    Plugin* p = listWidget->item(listWidget->currentRow())->data(Qt::UserRole).value<Plugin*>();
    if ( p )
        p->clear_settings();
}

void Dialog_Plugins::on_button_view_settings_clicked()
{

    Plugin* p = listWidget->item(listWidget->currentRow())->data(Qt::UserRole).value<Plugin*>();
    if ( p )
        emit edit_file(p->settings_file_path());
}
