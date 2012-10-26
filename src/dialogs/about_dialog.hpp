/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012  Mattia Basaglia

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
#ifndef ABOUT_DIALOG_HPP
#define ABOUT_DIALOG_HPP

#include "ui_about_dialog.h"

class About_Dialog : public QDialog, private Ui::About_Dialog
{
        Q_OBJECT
        
    public:
        explicit About_Dialog(QWidget *parent = 0);
    private slots:
        void on_pushButton_clicked();
};

#endif // ABOUT_DIALOG_HPP
