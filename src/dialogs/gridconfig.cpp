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
#include "gridconfig.hpp"
#include "resource_loader.hpp"
#include "translator.hpp"

GridConfig::GridConfig(snapping_grid *target_grid, QWidget *parent) :
    QDialog(parent), target(target_grid)
{
    setupUi(this);

    if ( target )
    {
        size_spin->setValue(target->get_size());
        shape_combo->setCurrentIndex(target->get_shape());
        enable_check->setChecked(target->is_enabled());
        origin_x_spin->setValue(target->get_origin().x());
        origin_y_spin->setValue(target->get_origin().y());
    }

    setWindowIcon(load::icon("move_grid"));

    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));
}


void GridConfig::on_buttonBox_accepted()
{
    if ( target )
    {
        target->set_size(size_spin->value());
        target->set_shape(snapping_grid::grid_shape(shape_combo->currentIndex()));
        target->enable ( enable_check->isChecked() );
        target->set_origin(QPointF(origin_x_spin->value(),origin_y_spin->value()));
    }
}

void GridConfig::on_pushButton_clicked()
{
    origin_x_spin->setValue(0);
    origin_y_spin->setValue(0);
}

void GridConfig::retranslate()
{
    retranslateUi(this);
}
