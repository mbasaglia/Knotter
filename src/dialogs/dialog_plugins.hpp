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

class Dialog_Plugins : public QDialog, private Ui::Dialog_Plugins
{
    Q_OBJECT
    
public:
    explicit Dialog_Plugins(QWidget *parent = 0);
    
protected:
    void changeEvent(QEvent *e);

protected slots:
    void load_plugins();
private slots:
    void on_listWidget_currentRowChanged(int currentRow);

    void on_check_enable_clicked(bool checked);

    void on_button_reload_clicked();

private:
    void set_item_enabled(QListWidgetItem* it, bool enabled);
    void set_item_errored(QListWidgetItem* it);
};

#endif // DIALOG_PLUGINS_HPP
