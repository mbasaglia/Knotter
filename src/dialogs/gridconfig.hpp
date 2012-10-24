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
#ifndef GRIDCONFIG_HPP
#define GRIDCONFIG_HPP

#include "ui_gridconfig.h"

#include "snapping_grid.hpp"
#include "knotview.hpp"
#include <QDockWidget>

class GridConfig : public QDockWidget, private Ui::GridConfig
{
        Q_OBJECT

    protected:
        snapping_grid *target;

    public:
        explicit GridConfig(QWidget *parent = 0);

        void set_grid_view(snapping_grid *target_grid);

    private slots:
        void position_spin_changed();
        void retranslate();
        void on_size_spin_valueChanged(int arg1);
        void on_shape_combo_currentIndexChanged(int index);
        void on_enable_check_toggled(bool arg1);
        void on_reset_btn_clicked();

        void on_move_btn_clicked();


        void grid_edited();

        void on_enable_check_clicked(bool checked);

    public slots:
        void grid_moved(QPointF p);


    signals:
        void move_button_clicked();
};

#endif // GRIDCONFIG_HPP
