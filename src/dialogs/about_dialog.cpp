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

#include "about_dialog.hpp"
#include "resource_manager.hpp"

About_Dialog::About_Dialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    label_title->setText(QString("%1 %2").arg(Resource_Manager::program_name())
                         .arg(Resource_Manager::program_version() ));
    label_icon->setPixmap(QIcon(Resource_Manager::data("img/icon-big.svg")).pixmap(64));
    setWindowTitle(tr("About %1").arg(Resource_Manager::program_name()));
}

void About_Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            setWindowTitle(tr("About %1").arg(Resource_Manager::program_name()));
            break;
        default:
            break;
    }
}

void About_Dialog::on_button_qt_clicked()
{
    QApplication::aboutQt();
}
