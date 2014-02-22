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

#ifndef DOCK_BACKGROUND_HPP
#define DOCK_BACKGROUND_HPP

#include "ui_dock_background.h"
#include <QDockWidget>

class Dock_Background : public QDockWidget, private Ui::Dock_Background
{
    Q_OBJECT
    
public:
    explicit Dock_Background(QWidget *parent = 0);
    
protected:
    void changeEvent(QEvent *e);

private slots:
    void commit_filename();

    void on_button_browse_clicked();

signals:
    /// Background has to change to the given filenamee
    void image_loaded(QString);

    void image_toogled(bool);

    /// Sale image by percentage 100 = normal size
    void image_scaled(double);

    void move_image();

    void background_changed(QColor);
};

#endif // DOCK_BACKGROUND_HPP
