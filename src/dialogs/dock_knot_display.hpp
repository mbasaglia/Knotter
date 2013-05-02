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

#ifndef DOCK_KNOT_DISPLAY_HPP
#define DOCK_KNOT_DISPLAY_HPP

#include <QDockWidget>
#include "ui_dock_knot_display.h"
#include "pen_join_style_metatype.hpp"

class Dock_Knot_Display : public QDockWidget, private Ui::Dock_Knot_Display
{
    Q_OBJECT

public:
    explicit Dock_Knot_Display(QWidget *parent = 0);

    void set_colors(const QList<QColor>& c);
    
protected:
    void changeEvent(QEvent *e);

private slots:
    void on_list_colors_removed(int arg1);

    void on_combo_joint_currentIndexChanged(int index);

signals:

    void colors_changed(const QList<QColor>&);

    void width_changed(double);

    void join_style_changed(Qt::PenJoinStyle);
};

#endif // DOCK_KNOT_DISPLAY_HPP
