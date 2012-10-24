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

GridConfig::GridConfig(QWidget *parent) :
    QDockWidget(parent), target(NULL)
{
    setupUi(this);

    setWindowIcon(load::icon("configure-grid"));

    setWidget(Ui_GridConfig::widget);

    move_btn->setIcon(load::icon("move_grid"));


    hide();
    setFloating(true);

    set_grid_view ( NULL );

    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));
}

void GridConfig::set_grid_view(snapping_grid *target_grid)
{
    if ( target )
    {
        target->disconnect(this);
    }

    target = target_grid;

    if ( target )
    {
        size_spin->setValue(target->get_size());
        shape_combo->setCurrentIndex(target->get_shape());
        enable_check->setChecked(target->is_enabled());

        origin_x_spin->blockSignals(true);
        origin_y_spin->blockSignals(true);
        origin_x_spin->setValue(target->get_origin().x());
        origin_y_spin->setValue(target->get_origin().y());
        origin_x_spin->blockSignals(false);
        origin_y_spin->blockSignals(false);

        connect(target,SIGNAL(edited()),SLOT(grid_edited()));
        setEnabled(true);
    }
    else
    {
        enable_check->setChecked(false);
        setEnabled(false);
    }
}

void GridConfig::position_spin_changed()
{
    if ( target )
    {
        target->set_origin(QPointF(origin_x_spin->value(),origin_y_spin->value()));
    }
}

void GridConfig::retranslate()
{
    retranslateUi(this);
    shape_combo->setItemIcon(0,load::icon("square_grid"));
    shape_combo->setItemIcon(1,load::icon("triangular_grid"));
    shape_combo->setItemIcon(2,load::icon("triangular_grid2"));
}

void GridConfig::on_size_spin_valueChanged(int arg1)
{
    if ( target )
    {
        target->set_size(arg1);
    }
}

void GridConfig::on_shape_combo_currentIndexChanged(int index)
{
    if ( target )
    {
        target->set_shape(snapping_grid::grid_shape(index));
    }
}

void GridConfig::on_enable_check_toggled(bool arg1)
{
    if ( target )
    {
        target->enable ( arg1 );
    }
}

void GridConfig::on_reset_btn_clicked()
{
    origin_x_spin->setValue(0);
    origin_y_spin->setValue(0);
}


void GridConfig::on_move_btn_clicked()
{
    emit move_button_clicked();
}

void GridConfig::grid_moved(QPointF p)
{
    origin_x_spin->setValue(p.x());
    origin_y_spin->setValue(p.y());
    Q_UNUSED(p);
}

void GridConfig::grid_edited()
{
    if ( target )
    {
        enable_check->setChecked(target->is_enabled());
    }
}

void GridConfig::on_enable_check_clicked(bool checked)
{

    if ( target )
    {
        target->enable(checked);
    }
}
