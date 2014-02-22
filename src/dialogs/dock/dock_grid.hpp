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

#ifndef DOCK_GRID_HPP
#define DOCK_GRID_HPP

#include "ui_dock_grid.h"
#include <QDockWidget>
#include "snapping_grid.hpp"
#include "c++.hpp"

class Dock_Grid : public QDockWidget, private Ui::Dock_Grid
{
    Q_OBJECT

private:
    Snapping_Grid* target;

public:
    explicit Dock_Grid(QWidget *parent = 0);

    void set_grid(Snapping_Grid* target_grid);

    /// if grid is target grid, clear target grid
    void unset_grid(Snapping_Grid* grid);

protected:
    void changeEvent(QEvent *e);

signals:
    /// Emitted when the user want to move the grid with the mouse
    void move_grid();

private slots:
    void position_spin_changed();
    void grid_moved(QPointF p);
    void on_button_reset_clicked();
    void on_combo_shape_currentIndexChanged(int index);
    void on_check_enable_toggled(bool arg1);
};

#endif // DOCK_GRID_HPP
