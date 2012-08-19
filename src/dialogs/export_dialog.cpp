#include "export_dialog.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include <QSvgGenerator>
Export_Dialog::Export_Dialog(QWidget *parent) :
    QDialog(parent), canvas ( 0 )
{
    setupUi(this);
}

void Export_Dialog::set_knot_view(KnotView *kn)
{
    canvas = kn;
}

void Export_Dialog::on_export_svg_button_clicked()
{
    QString exname = QFileDialog::getSaveFileName(this,tr("Export Knot as SVG"),filename,
                "SVG Images (*.svg);;XML files (*.xml);;All files (*)" );


    QFile quf(exname);
    if ( ! quf.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Could not write to \"%1\".").arg(exname));
        return;
    }

    filename = exname;

    QSvgGenerator gen;
    gen.setOutputDevice(&quf);

    canvas->paint_knot ( &gen, get_area(), only_shape_check->isChecked() );

    quf.close();
}

void Export_Dialog::on_export_raster_button_clicked()
{


    QStringList filters = QStringList()
        << tr("PNG Images (*.png)")         // 0
        << tr("Jpeg Images (*.jpg *.jpeg)") // 1
        << tr("Bitmap (*.bmp)")             // 2
        << tr("All files (*)")              // 3
        ;
    static const int png = 0;

    QFileDialog export_dialog(this,tr("Export Knot as Image"),filename);
    export_dialog.setAcceptMode ( QFileDialog::AcceptSave );
    export_dialog.setNameFilters(filters);

    if ( !export_dialog.exec() )
        return;

    QString exname = export_dialog.selectedFiles()[0];


    /*QString exname = QFileDialog::getSaveFileName(this,tr("Export Knot as SVG"),filename,
                "SVG Images (*.svg);;XML files (*.xml);;All files (*)" );*/


    QFile quf(exname);
    if ( ! quf.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Could not write to \"%1\".").arg(exname));
        return;
    }

    filename = exname;

    int name_filter = filters.indexOf(export_dialog.selectedFilter());


    // pixmap
    int back_alpha = name_filter == png ? 0 : 255;

    if ( antialias_check->isChecked() )
    {
        /// Letting QPainter handle antialiasing is not enough... :^(
        QImage original (get_size()*2, QImage::Format_ARGB32);
        original.fill(QColor(255,255,255,back_alpha).rgba());


        QPainter painter;
        painter.begin(&original);

        QSizeF actual_size = get_area().size();
        double scale_x = 2 * get_size().width() / actual_size.width();
        double scale_y = 2 * get_size().height() / actual_size.height();

        painter.scale(scale_x,scale_y);

        canvas->paint_knot ( &painter, get_area(), false );

        painter.end();


        QImage supersampled(get_size(), QImage::Format_ARGB32 );
        for ( int y = 0; y < supersampled.height(); y++ )
            for ( int x = 0; x < supersampled.width(); x++ )
            {
                QColor p1 = QColor::fromRgba(original.pixel(2*x,2*y));
                QColor p2 = QColor::fromRgba(original.pixel(2*x+1,2*y));
                QColor p3 = QColor::fromRgba(original.pixel(2*x,2*y+1));
                QColor p4 = QColor::fromRgba(original.pixel(2*x+1,2*y+1));
                QColor color ( (p1.red()+p2.red()+p3.red()+p4.red())/4,
                                (p1.green()+p2.green()+p3.green()+p4.green())/4,
                                (p1.blue()+p2.blue()+p3.blue()+p4.blue())/4,
                                (p1.alpha()+p2.alpha()+p3.alpha()+p4.alpha())/4
                              );
                supersampled.setPixel(x,y,color.rgba());
            }
        supersampled.save(&quf,0,100-quality_slider->value());
    }
    else
    {
        QPixmap pix (get_size());
        pix.fill(QColor(255,255,255,back_alpha));

        //canvas->paint_knot ( &pix, get_area(), false );
        QPainter painter;
        painter.begin(&pix);

        QSizeF actual_size = get_area().size();
        double scale_x = get_size().width() / actual_size.width();
        double scale_y = get_size().height() / actual_size.height();

        painter.scale(scale_x,scale_y);

        canvas->paint_knot ( &painter, get_area(), false );

        painter.end();

        pix.save(&quf,0,100-quality_slider->value());
    }

    quf.close();
}

QSize Export_Dialog::get_size() const
{
    return QSize ( width_spin->value(), height_spin->value() );
}

QRectF Export_Dialog::get_area()
{
    if ( area_everything->isChecked() )
        return canvas->scene()->itemsBoundingRect();
    else
        return canvas->sceneRect();
}

void Export_Dialog::on_quality_slider_valueChanged(int value)
{
    quality_label->setText(QString("%1%").arg(value));
}

void Export_Dialog::reset_size()
{

    height_spin->blockSignals(true);
    width_spin->blockSignals(true);

    width_spin->setValue(get_area().width());
    height_spin->setValue(get_area().height());

    height_spin->blockSignals(false);
    width_spin->blockSignals(false);

    ratio = double ( height_spin->value() ) / width_spin->value();

}

void Export_Dialog::on_width_spin_valueChanged(int arg1)
{
    if ( ratio_check->isChecked() )
    {
        height_spin->blockSignals(true);
        height_spin->setValue(ratio*arg1);
        height_spin->blockSignals(false);
    }
}

void Export_Dialog::on_height_spin_valueChanged(int arg1)
{
    if ( ratio_check->isChecked() )
    {
        width_spin->blockSignals(true);
        width_spin->setValue(arg1/ratio);
        width_spin->blockSignals(false);
    }
}

void Export_Dialog::on_ratio_check_toggled(bool)
{
    ratio = double ( height_spin->value() ) / width_spin->value();
}
