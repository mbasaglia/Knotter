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

#ifndef WIZARD_CREATE_PLUGIN_HPP
#define WIZARD_CREATE_PLUGIN_HPP

#include "ui_wizard_create_plugin.h"

class Wizard_Create_Plugin : public QWizard, private Ui::Wizard_Create_Plugin
{
    Q_OBJECT
    
public:
    explicit Wizard_Create_Plugin(QWidget *parent = 0);
    
protected:
    void changeEvent(QEvent *e);
    bool validateCurrentPage();
private:
    /**
     *  \brief plugin file identifier
     */
    QString plugin_id();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_button_directory_clicked();
    void on_text_name_editingFinished();
    void on_text_directory_editingFinished();

public slots:
    void accept();
};

#endif // WIZARD_CREATE_PLUGIN_HPP
