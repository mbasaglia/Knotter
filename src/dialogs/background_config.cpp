#include "background_config.hpp"
#include "resource_loader.hpp"
#include <QFileDialog>
#include <QImageWriter>

Background_Config::Background_Config(QWidget *parent) :
    QDialog(parent), kv ( NULL )
{
    setupUi(this);
    setWindowIcon(load::icon("preferences-desktop-wallpaper"));
    color_select->set_color(Qt::transparent);
}

void Background_Config::set_view(KnotView *kview)
{
    kv = kview;
}

void Background_Config::on_color_select_color_changed(const QColor & col)
{
    if ( !kv ) return;

    kv->setBackgroundBrush(col);
    kv->redraw(false);
}

void Background_Config::on_image_check_toggled(bool arg1)
{
    if ( !kv ) return;

    kv->background().set_active(arg1);
    kv->redraw(false);
}

void Background_Config::on_file_name_editingFinished()
{
    if ( !kv ) return;

    kv->background().load(file_name->text());
    kv->redraw(false);
}

void Background_Config::on_pushButton_clicked()
{
    if ( !kv ) return;

    QString supported;
    foreach(QByteArray ba, QImageWriter::supportedImageFormats())
        supported += " *."+QString(ba);

    QString filename = QFileDialog::getOpenFileName ( this,tr("Open image"),"",
         tr("All supported images (%1)").arg(supported)+";;"+tr("All files (*)") );

    if ( !filename.isEmpty() )
    {
        file_name->setText(filename);
        kv->background().load(filename);
        kv->redraw(false);
    }
}

void Background_Config::on_zoom_percent_valueChanged(double arg1)
{
    if ( !kv ) return;

    kv->background().set_zoom(arg1/100);
    kv->redraw(false);
}
