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
#include "node_style_form.hpp"
#include "resource_loader.hpp"
#include "translator.hpp"

node_style_form::node_style_form(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    combo_style[knot_curve_styler::idof("pointed")]=0;
    combo_style[knot_curve_styler::idof("ogee")]=1;
    combo_style[knot_curve_styler::idof("polygonal")]=2;
    curstyle_id = knot_curve_styler::idof("pointed");

    style_combo->setItemIcon(0,load::icon("pointcurve"));
    style_combo->setItemIcon(1,load::icon("ogee"));
    style_combo->setItemIcon(2,load::icon("poly"));

    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));
}

void node_style_form::set_style_info(styleinfo si)
{
    handle_length_spinner->setValue ( si.handle_length );
    crossing_gap_spinner->setValue ( si.crossing_distance );
    cusp_angle_spinner->setValue ( si.cusp_angle );
    cusp_distance_spinner->setValue ( si.cusp_distance );

    curstyle_id = si.curve_style;
    style_combo->setCurrentIndex(combo_style[curstyle_id]);
}

styleinfo node_style_form::get_style_info() const
{
    return styleinfo(curstyle_id,cusp_angle_spinner->value(),
                    handle_length_spinner->value(),
                    crossing_gap_spinner->value(),
                    cusp_distance_spinner->value() );
}



void node_style_form::on_style_combo_activated(int index)
{
    curstyle_id = combo_style.key( index );
    anything_changed();
}

void node_style_form::retranslate()
{
    retranslateUi(this);
}

void node_style_form::anything_changed()
{
    emit style_changed(get_style_info());
}

