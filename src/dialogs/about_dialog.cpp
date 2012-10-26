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
#include "about_dialog.hpp"
#include "resource_loader.hpp"

About_Dialog::About_Dialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    setWindowIcon(load::icon("help-about"));

    QString about_text = tr("Knotter %1").arg(VERSION) + "\n";

    #ifdef NO_BOOST
        about_text += "\n" + tr("Support for Boost libraries is disabled") + "\n" +
                      tr("Reduced command line options only") + "\n";
    #else
        about_text += "\n" + tr("Support for Boost libraries is enabled") + "\n" +
                        tr("Full command line options available") + "\n";
    #endif

    #ifdef TANGO_DEFAULT
        about_text += "\n" + tr("Using Tango! icon theme") + "\n";
    #elif TANGO_FALLBACK
        if ( QIcon::themeName() == "tango-icons" )
            about_text += "\n" + tr("Using Tango! icon theme as fall back") + "\n";
    #endif

    about_text += "\n" + tr("Compiled with support for Qt %1").arg(QT_VERSION_STR) + "\n" +
                       tr("Running on Qt %1").arg(qVersion());

    label_about->setText(about_text);

    label_icon->setPixmap(load::icon("knotter-logo-big").pixmap(64));

}

void About_Dialog::on_pushButton_clicked()
{
    QApplication::aboutQt();
}
