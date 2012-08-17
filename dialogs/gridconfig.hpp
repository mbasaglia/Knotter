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

#include "graphics/snapping_grid.hpp"

class GridConfig : public QDialog, private Ui::GridConfig
{
        Q_OBJECT

    protected:
        snapping_grid *target;

    public:
        explicit GridConfig(snapping_grid *target_grid, QWidget *parent = 0);
    private slots:
        void on_buttonBox_accepted();
        void on_pushButton_clicked();
};

#endif // GRIDCONFIG_HPP
