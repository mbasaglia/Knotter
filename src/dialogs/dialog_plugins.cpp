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
#include "json_stuff.hpp"

Dialog_Plugins::Dialog_Plugins(QWidget *parent) :
    QDialog(parent), reply(nullptr)
{
    setupUi(this);

    load_plugins();
    update_network_current_plugin(-1);
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

    list_installed->blockSignals(true);
    list_installed->clear();
    list_installed->blockSignals(false);
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
        list_installed->addItem(item);
    }

    if ( list_installed->count() > 0)
        list_installed->setCurrentRow(0);

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

void Dialog_Plugins::on_list_installed_currentRowChanged(int currentRow)
{
    Plugin* p = list_installed->item(currentRow)->data(Qt::UserRole).value<Plugin*>();
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
    Resource_Manager::plugins()[list_installed->currentIndex().row()]->enable(checked);
    set_item_enabled(list_installed->currentItem(),checked);
}

void Dialog_Plugins::on_button_reload_clicked()
{
    Plugin *old = current_plugin();
    QString file;
    if ( old )
        file = old->string_data("plugin_file");
    Resource_Manager::reload_plugins();
    load_plugins();
    for ( int i = 0; i < list_installed->count(); i++ )
    {
        if ( plugin(i)->string_data("plugin_file") == file )
        {
            list_installed->setCurrentRow(i);
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
    return plugin(list_installed->currentIndex().row());
}

Plugin *Dialog_Plugins::plugin(int i)
{
    if ( i >= 0 && i < list_installed->count() )
        return list_installed->item(i)->data(Qt::UserRole).value<Plugin*>();

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
    for(int i = 0; i < list_installed->count(); i++ )
    {
        QListWidgetItem* item = list_installed->item(i);

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
    Plugin* p = list_installed->item(list_installed->currentRow())->data(Qt::UserRole).value<Plugin*>();
    if ( p )
        p->clear_settings();
}

void Dialog_Plugins::on_button_view_settings_clicked()
{

    Plugin* p = list_installed->item(list_installed->currentRow())->data(Qt::UserRole).value<Plugin*>();
    if ( p )
        emit edit_file(p->settings_file_path());
}

void Dialog_Plugins::network_refresh_all()
{
    label_status_download->setText(tr("Connecting..."));
    progress_download->setValue(0);
    reply = Resource_Manager::network_get(
                QString("%1?v=%2").arg(PLUGIN_REPO).arg(Resource_Manager::program_version())
    );
    frame_online_widget->setEnabled(false);
    connect(reply,SIGNAL(finished()),SLOT(network_reply_all_finished()));
    connect(reply,SIGNAL(destroyed()),SLOT(network_reply_destroyed()));
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),SLOT(network_update(qint64,qint64)));
}


void Dialog_Plugins::network_reply_all_finished()
{
    progress_download->setVisible(false);
    button_cancel_download->setVisible(false);
    frame_online_widget->setEnabled(true);

    if ( reply->error() == QNetworkReply::NoError )
    {
        label_status_download->setText(tr("Finished"));
        update_network_plugins();
    }
    else
        label_status_download->setText(tr("Error %1").arg((int)reply->error() ));

    reply->deleteLater();
}

void Dialog_Plugins::network_reply_destroyed()
{
    reply = nullptr;
}

void Dialog_Plugins::update_network_plugins()
{
    if ( !reply )
        return;

    // update list
    QStringList categories;


    list_online->blockSignals(true);
    list_online->clear();
    list_online->blockSignals(false);

    QList<QVariant> list = json_read_file(*reply).toList();
    foreach(QVariant pv, list)
    {
        QVariantMap map = pv.toMap();

        if ( !categories.contains(map["category"].toString()) )
        {
            categories << map["category"].toString();
        }


        QListWidgetItem *item = new QListWidgetItem(map["name"].toString());
        bool installable = true;
        foreach(Plugin*p, Resource_Manager::plugins())
        {
            if (p->string_data("plugin_shortname") == map["plugin_shortname"].toString() )
            {
                map["plugin_plugin"] = QVariant::fromValue(p);
                if ( p->data("version",0) >= map["version"].toInt() )
                    installable = false;
            }
        }
        set_item_enabled(item,installable);
        item->setData(Qt::UserRole,map);
        list_online->addItem(item);
    }

    // update category combo
    combo_category_online->blockSignals(true);
    combo_category_online->clear();
    combo_category_online->addItem(tr("All"));
    foreach(QString s, categories)
    {
        combo_category_online->addItem(s);
    }
    combo_category_online->blockSignals(false);
    combo_category_online->setCurrentIndex(0);
}


void Dialog_Plugins::network_update(qint64 bytesReceived, qint64 bytesTotal)
{
    if ( bytesTotal > 0 )
    {
        label_status_download->setText(tr("Loading..."));
        progress_download->setValue(bytesReceived*100/bytesTotal);
        progress_download->setVisible(true);
        button_cancel_download->setVisible(true);
    }
}

void Dialog_Plugins::network_abort()
{
    if ( reply )
    {
        reply->abort();
        reply->deleteLater();
        reply = nullptr;
    }
}

void Dialog_Plugins::update_network_current_plugin(int index)
{
    if ( index == -1 )
    {
        frame_plugin_online->setEnabled(false);
        label_title_online->setText(tr("No plugin loaded"));
        tableWidget_online->setRowCount(0);
        text_description_online->setPlainText("Select a plugin to view its data");
        label_plugin_online_status->setText("");
        list_plugin_files_online->clear();
    }
    else
    {
        frame_plugin_online->setEnabled(true);

        QVariantMap p = list_online->item(index)->data(Qt::UserRole).toMap();

        label_title_online->setText(p["name"].toString());
        text_description_online->setPlainText(p["description"].toString());
        label_plugin_online_status->setText(tr("You don't have this plugin"));
        button_install_plugin->setEnabled(true);
        if ( p.contains("plugin_plugin") )
        {
            Plugin* plug = p["plugin_plugin"].value<Plugin*>();
            if ( plug )
            {
                if ( plug->data("version",0) >= p["version"].toInt() )
                {
                    label_plugin_online_status->setText(tr("You already have this plugin"));
                    button_install_plugin->setEnabled(false);
                }
                else
                    label_plugin_online_status->setText(tr("You can upgrade this plugin"));

            }
        }

        tableWidget_online->clearContents();
        tableWidget_online->setRowCount(0);
        foreach(QString k, p.keys())
        {
            if ( k != "plugin_plugin" && k != "plugin_files")
            {
                int row = tableWidget_online->rowCount();
                tableWidget_online->insertRow(row);
                QTableWidgetItem *it = new QTableWidgetItem(k);
                it->setFlags(Qt::ItemIsEnabled);
                tableWidget_online->setItem(row,0,it);
                it = new QTableWidgetItem(p[k].toString());
                it->setFlags(Qt::ItemIsEnabled);
                tableWidget_online->setItem(row,1,it);
            }
        }

        list_plugin_files_online->clear();
        list_plugin_files_online->addItems(p["plugin_files"].toStringList());
    }
}

void Dialog_Plugins::on_tabWidget_currentChanged(int index)
{
    if ( index == 1 && !reply )
    {
        network_refresh_all();
    }
}

void Dialog_Plugins::on_combo_category_online_currentIndexChanged(int index)
{

    for(int i = 0; i < list_online->count(); i++ )
    {
        QListWidgetItem* item = list_online->item(i);

        if ( index == 0 )
        {
            item->setHidden(false);
        }
        else
        {
            QString cat = item->data(Qt::UserRole).toMap()["category"].toString();
            item->setHidden( cat != combo_category_online->currentText() );
        }
    }
}
