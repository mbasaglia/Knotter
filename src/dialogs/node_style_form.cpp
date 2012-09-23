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

node_style_form::node_style_form(QWidget *parent, bool is_default) :
    QWidget(parent)
{
    setupUi(this);

    combo_style[knot_curve_styler::idof("pointed")]=0;
    combo_style[knot_curve_styler::idof("round")]=1;
    combo_style[knot_curve_styler::idof("ogee")]=2;
    combo_style[knot_curve_styler::idof("polygonal")]=3;
    curstyle_id = knot_curve_styler::idof("pointed");

    style_combo->setItemIcon(0,load::icon("cusp-point"));
    style_combo->setItemIcon(1,load::icon("cusp-round"));
    style_combo->setItemIcon(2,load::icon("cusp-ogee"));
    style_combo->setItemIcon(3,load::icon("cusp-poly"));

    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));

    if ( is_default )
    {
        for ( int i = 0; i < gridLayout->rowCount(); i++ )
        {
            QCheckBox *cb = dynamic_cast<QCheckBox*>(
                                gridLayout->itemAtPosition(i,0)->widget() );
            if ( cb )
            {
                cb->setVisible(false);
                cb->setChecked(true);
                cb->setEnabled(false);
            }
        }
    }
}

void node_style_form::set_style_info(styleinfo si)
{
    handle_length_spinner->setValue ( si.handle_length );
    crossing_gap_spinner->setValue ( si.crossing_distance );
    cusp_angle_spinner->setValue ( si.cusp_angle );
    cusp_distance_spinner->setValue ( si.cusp_distance );

    curstyle_id = si.curve_style;
    style_combo->setCurrentIndex(combo_style[curstyle_id]);

   if ( si.enabled_style & styleinfo::CURVE_STYLE )     style_check        ->setChecked(true);
   if ( si.enabled_style & styleinfo::HANDLE_LENGTH )   handle_length_check->setChecked(true);
   if ( si.enabled_style & styleinfo::CROSSING_DISTANCE)crossing_gap_check ->setChecked(true);
   if ( si.enabled_style & styleinfo::CUSP_ANGLE )      cusp_angle_check   ->setChecked(true);
   if ( si.enabled_style & styleinfo::CUSP_DISTANCE )   cusp_distance_check->setChecked(true);

}

styleinfo node_style_form::get_style_info() const
{
    styleinfo::Enabled enabled_style;
    if ( style_check        ->isChecked() ) enabled_style |= styleinfo::CURVE_STYLE;
    if ( handle_length_check->isChecked() ) enabled_style |= styleinfo::HANDLE_LENGTH;
    if ( crossing_gap_check ->isChecked() ) enabled_style |= styleinfo::CROSSING_DISTANCE;
    if ( cusp_angle_check   ->isChecked() ) enabled_style |= styleinfo::CUSP_ANGLE;
    if ( cusp_distance_check->isChecked() ) enabled_style |= styleinfo::CUSP_DISTANCE;

    return styleinfo(curstyle_id,cusp_angle_spinner->value(),
                    handle_length_spinner->value(),
                    crossing_gap_spinner->value(),
                    cusp_distance_spinner->value(),
                     enabled_style );
}

void node_style_form::set_default(styleinfo custom, styleinfo def)
{
    set_style_info(custom);
    styleinfo::Enabled en = custom.enabled_style;
    if(!(en&styleinfo::CURVE_STYLE) )     style_combo->setCurrentIndex(combo_style[def.curve_style]);
    if(!(en&styleinfo::HANDLE_LENGTH) )   handle_length_spinner->setValue ( def.handle_length );
    if(!(en&styleinfo::CROSSING_DISTANCE))crossing_gap_spinner->setValue ( def.crossing_distance );
    if(!(en&styleinfo::CUSP_ANGLE) )      cusp_angle_spinner->setValue ( def.cusp_angle );
    if(!(en&styleinfo::CUSP_DISTANCE) )   cusp_distance_spinner->setValue ( def.cusp_distance );
}




void node_style_form::on_style_combo_activated(int index)
{
    curstyle_id = combo_style.key( index );
    anything_changed();
}

void node_style_form::retranslate()
{
    retranslateUi(this);
    style_combo->setItemIcon(0,load::icon("cusp-point"));
    style_combo->setItemIcon(1,load::icon("cusp-round"));
    style_combo->setItemIcon(2,load::icon("cusp-ogee"));
    style_combo->setItemIcon(3,load::icon("cusp-poly"));
}

void node_style_form::anything_changed()
{
    emit style_changed(get_style_info());
}

