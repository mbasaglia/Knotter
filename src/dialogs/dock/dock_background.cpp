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

#include "dock_background.hpp"
#include <QFileDialog>
#include <QImageReader>

Dock_Background::Dock_Background(QWidget *parent) :
    QDockWidget(parent)
{
    setupUi(this);

    connect(button_move,SIGNAL(clicked()),SIGNAL(move_image()));
    connect(spin_scale,SIGNAL(valueChanged(double)),SIGNAL(image_scaled(double)));
    connect(color_selector,SIGNAL(colorChanged(QColor)),SIGNAL(background_changed(QColor)));
    connect(check_image,SIGNAL(toggled(bool)),SIGNAL(image_toogled(bool)));
    connect(text_file,SIGNAL(editingFinished()),SLOT(commit_filename()));
}

void Dock_Background::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

void Dock_Background::commit_filename()
{
    emit image_loaded(text_file->text());
    spin_scale->setValue(100);
}

void Dock_Background::on_button_browse_clicked()
{
    QString supported;
    foreach(QByteArray ba, QImageReader::supportedImageFormats())
        supported += " *."+QString(ba);

    QString filename = QFileDialog::getOpenFileName ( this,
         tr("Open background image"),
         QFileInfo(text_file->text()).dir().absolutePath(),
         tr("All supported images (%1)").arg(supported)+";;"+tr("All files (*)")
    );

    if ( !filename.isEmpty() )
    {
        text_file->setText(filename);
        commit_filename();
    }
}
