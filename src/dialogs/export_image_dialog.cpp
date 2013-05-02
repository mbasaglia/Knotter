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
#include <QImageWriter>
#include <QShowEvent>

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
    reset_size();
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

void Export_Image_Dialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    if ( !event->spontaneous() )
        reset_size();
}

void Export_Image_Dialog::on_button_ratio_toggled(bool checked)
{
    button_ratio->setIcon(QIcon::fromTheme(
        checked ? "object-locked" : "object-unlocked"
    ));

    ratio = double ( spin_height->value() ) / spin_width->value();
}

void Export_Image_Dialog::on_slider_quality_valueChanged(int value)
{
    //: Quality percentage
    label_quality->setText(tr("%1%").arg(value));
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


    export_svg(view->get_graph(),quf,check_graph->isChecked());

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

void Export_Image_Dialog::on_button_image_clicked()
{
    QString supported;
    foreach(QByteArray ba, QImageWriter::supportedImageFormats())
        supported += " *."+QString(ba);

    QStringList filters = QStringList()
        << tr("PNG Images (*.png)")         // 0
        << tr("Jpeg Images (*.jpg *.jpeg)") // 1
        << tr("Bitmap (*.bmp)")             // 2
        << tr("All supported images (%1)").arg(supported) // 3
        << tr("All files (*)")              // 4
        ;
    static const int png = 0;

    QFileDialog export_dialog(this,tr("Export Knot as Image"),file_name);
    export_dialog.setAcceptMode ( QFileDialog::AcceptSave );
    export_dialog.setNameFilters(filters);

    if ( !export_dialog.exec() )
        return;

    QString exname = export_dialog.selectedFiles()[0];


    QFile quf(exname);

    if ( !file_ok(quf) )
        return;

    file_name = exname;

    int name_filter = filters.indexOf(export_dialog.
#ifdef HAS_QT_5
        selectedNameFilter
#else
        selectedFilter
#endif
      ());


    // pixmap
    QColor back = Qt::white;
    if ( name_filter == png )
        back = Qt::transparent;

    export_raster(view->get_graph(),quf,back,check_antialiasing->isChecked(),
                  QSize(spin_width->value(),spin_height->value()),
                  slider_quality->value(), check_graph->isChecked());


}

void Export_Image_Dialog::reset_size()
{
    button_ratio->setChecked(false);
    QSize sz = view->get_graph().boundingRect().size().toSize();
    spin_width->setValue(sz.width());
    spin_height->setValue(sz.height());
    button_ratio->setChecked(true);
}

void Export_Image_Dialog::on_spin_width_valueChanged(int arg1)
{
    if ( button_ratio->isChecked() )
    {
        spin_height->blockSignals(true);
        spin_height->setValue(ratio*arg1);
        spin_height->blockSignals(false);
    }
}

void Export_Image_Dialog::on_spin_height_valueChanged(int arg1)
{
    if ( button_ratio->isChecked() )
    {
        spin_width->blockSignals(true);
        spin_width->setValue(arg1/ratio);
        spin_width->blockSignals(false);
    }
}
