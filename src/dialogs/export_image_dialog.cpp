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
#include "image_exporter.hpp"
#include <QMessageBox>
#include <QFileDialog>

Export_Image_Dialog::Export_Image_Dialog(QWidget *parent) :
    QDialog(parent), view(nullptr)
{
    setupUi(this);
}

void Export_Image_Dialog::set_view(const Knot_View *v)
{
    if ( !view )
        file_name = v->file_name();

    view = v;
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

void Export_Image_Dialog::on_button_svg_clicked()
{

    QString exname = QFileDialog::getSaveFileName(this,tr("Export Knot as SVG"),file_name,
                "SVG Images (*.svg);;XML files (*.xml);;All files (*)" );

    if ( exname.isNull() )
        return;

    QFile quf(exname);

    if ( !file_ok(quf) )
        return;

    file_name = exname;


    export_svg(view->get_graph(),quf);

    quf.close();

}

bool Export_Image_Dialog::file_ok(QFile &file)
{
    if ( ! file.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Could not write to \"%1\".")
                             .arg(file.fileName()));
        return false;
    }
    return true;
}
