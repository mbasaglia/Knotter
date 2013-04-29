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

#include "export_image_dialog.hpp"

Export_Image_Dialog::Export_Image_Dialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void Export_Image_Dialog::changeEvent(QEvent *e)
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

void Export_Image_Dialog::on_button_ratio_toggled(bool checked)
{
    button_ratio->setIcon(QIcon::fromTheme(
        checked ? "object-locked" : "object-unlocked"
    ));

}

void Export_Image_Dialog::on_slider_compression_valueChanged(int value)
{
    //: Compression percentage
    label_compression->setText(tr("%1%").arg(value));
}
