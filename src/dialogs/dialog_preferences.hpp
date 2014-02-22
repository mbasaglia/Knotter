/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#ifndef DIALOG_PREFERENCES_HPP
#define DIALOG_PREFERENCES_HPP

#include "ui_dialog_preferences.h"
#include <QMainWindow>

class Dialog_Preferences : public QDialog, private Ui::Dialog_Preferences
{
    Q_OBJECT
    
public:
    explicit Dialog_Preferences(QMainWindow *parent);

private:
    void init_combos();

protected slots:
    void set_preferences();

public slots:
    void retranslate();
private slots:
    void on_button_clear_recent_clicked();
    void on_button_clear_settings_clicked();
    void on_combo_widget_style_currentIndexChanged(int);
};

#endif // DIALOG_PREFERENCES_HPP
