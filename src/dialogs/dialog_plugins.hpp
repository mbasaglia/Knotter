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

#ifndef DIALOG_PLUGINS_HPP
#define DIALOG_PLUGINS_HPP

#include "ui_dialog_plugins.h"

#include "plugin.hpp"
#include <QNetworkReply>

class Dialog_Plugins : public QDialog, private Ui::Dialog_Plugins
{
    Q_OBJECT

    QNetworkReply* reply;
    
public:
    explicit Dialog_Plugins(QWidget *parent = 0);
    
protected:
    void changeEvent(QEvent *e);

protected slots:
    void load_plugins();

private slots:
    void network_refresh_all();
    void network_reply_all_finished();
    void network_reply_destroyed();
    void network_update ( qint64 bytesReceived, qint64 bytesTotal ) ;
    void network_abort();
    void update_network_current_plugin(int index);


    void reload_combo();

    void on_list_installed_currentRowChanged(int currentRow);

    void on_check_enable_clicked(bool checked);

    void on_button_reload_clicked();

    void on_button_reload_2_clicked();

    void on_button_edit_clicked();

    void on_button_create_clicked();

    void on_check_enable_toggled(bool checked);

    void on_combo_category_currentIndexChanged(int index);

    void on_button_clear_settings_clicked();

    void on_button_view_settings_clicked();


    void on_tabWidget_currentChanged(int index);

    void on_combo_category_online_currentIndexChanged(int index);

    void on_button_install_plugin_clicked();

private:
    void set_item_enabled(QListWidgetItem* it, bool enabled);
    void set_item_errored(QListWidgetItem* it);
    /**
     *  \brief Get currently selected plugin
     */
    Plugin* current_plugin();
    /**
     *  \brief Get plugin at given index
     */
    Plugin* plugin(int i);

    void update_network_plugins();

signals:
    /**
     * \brief Emitted when the user presses the "Edit Script" button
     */
    void edit_plugin(Plugin*);
    /**
     * \brief Emitted when the user wants to view a file (like the plugin settings and such)
     * \note This is not called for the plugin script
     * \see edit_plugin()
     */
    void edit_file(QString);
};

#endif // DIALOG_PLUGINS_HPP
