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

    for ( knot_curve_styler::name_container::const_iterator
                        i = knot_curve_styler::get_ui_names().begin();
            i != knot_curve_styler::get_ui_names().end();
            ++i )
    {
        QIcon icon = load::icon("cusp-"+knot_curve_styler::name(i.value()));
        if ( icon.isNull() )
            icon = load::icon("cusp-other");
        combo_style[i.value()] = style_combo->count();
        style_combo->addItem(icon,i.key());
    }

    curstyle_id = knot_curve_styler::idof("pointed");

    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));

}

void node_style_form::set_style_info(styleinfo si)
{


    if ( si.enabled_style & styleinfo::CURVE_STYLE || !style_check->isVisible() )
    {
        style_check->setChecked(true);
        curstyle_id = si.curve_style;
        style_combo->setCurrentIndex(combo_style[curstyle_id]);
    }
    else
        style_check->setChecked(false);

    if ( si.enabled_style & styleinfo::HANDLE_LENGTH || !handle_length_check->isVisible() )
    {
        handle_length_check->setChecked(true);
        handle_length_spinner->setValue ( si.handle_length );
    }
    else
        handle_length_check->setChecked(false);

    if ( si.enabled_style & styleinfo::CROSSING_DISTANCE || !crossing_gap_check->isVisible() )
    {
        crossing_gap_check->setChecked(true);
        crossing_gap_spinner->setValue ( si.crossing_distance );
    }
    else
        crossing_gap_check->setChecked(false);

    if ( si.enabled_style & styleinfo::CUSP_ANGLE || !cusp_angle_check->isVisible() )
    {
        cusp_angle_check->setChecked(true);
        cusp_angle_spinner->setValue ( si.cusp_angle );
    }
    else
        cusp_angle_check->setChecked(false);

    if ( si.enabled_style & styleinfo::CUSP_DISTANCE || !cusp_distance_check->isVisible() )
    {
        cusp_distance_check->setChecked(true);
        cusp_distance_spinner->setValue ( si.cusp_distance );
    }
    else
        cusp_distance_check->setChecked(false);


}

styleinfo node_style_form::get_style_info() const
{
    styleinfo::Enabled enabled_style;
    if ( style_check->isChecked() || !style_check->isVisible() )
        enabled_style |= styleinfo::CURVE_STYLE;
    if ( handle_length_check->isChecked() || !handle_length_check->isVisible() )
        enabled_style |= styleinfo::HANDLE_LENGTH;
    if ( crossing_gap_check->isChecked() || !crossing_gap_check->isVisible() )
        enabled_style |= styleinfo::CROSSING_DISTANCE;
    if ( cusp_angle_check->isChecked() || !cusp_angle_check->isVisible() )
        enabled_style |= styleinfo::CUSP_ANGLE;
    if ( cusp_distance_check->isChecked() || !cusp_distance_check->isVisible() )
        enabled_style |= styleinfo::CUSP_DISTANCE;

    return styleinfo(curstyle_id,cusp_angle_spinner->value(),
                    handle_length_spinner->value(),
                    crossing_gap_spinner->value(),
                    cusp_distance_spinner->value(),
                     enabled_style );
}

void node_style_form::global_style_mode()
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


void node_style_form::from_single_node(styleinfo custom, styleinfo def)
{
    blockSignals(true);
    set_style_info(custom);
    styleinfo::Enabled en = custom.enabled_style;
    if(!(en&styleinfo::CURVE_STYLE) )     style_combo->setCurrentIndex(combo_style[def.curve_style]);
    if(!(en&styleinfo::HANDLE_LENGTH) )   handle_length_spinner->setValue ( def.handle_length );
    if(!(en&styleinfo::CROSSING_DISTANCE))crossing_gap_spinner->setValue ( def.crossing_distance );
    if(!(en&styleinfo::CUSP_ANGLE) )      cusp_angle_spinner->setValue ( def.cusp_angle );
    if(!(en&styleinfo::CUSP_DISTANCE) )   cusp_distance_spinner->setValue ( def.cusp_distance );
    blockSignals(false);
}

void node_style_form::from_multi_nodes(node_list nodes, styleinfo def_style)
{
    blockSignals(true);
    if ( nodes.empty() )
        setEnabled(false);
    else
    {
        setEnabled(true);
        for ( int i = 0; i < gridLayout->rowCount(); i++ )
        {
            QCheckBox *cb = dynamic_cast<QCheckBox*>(
                                gridLayout->itemAtPosition(i,0)->widget() );
            if ( cb )
            {
                cb->setChecked(false);
            }
        }
        /// \todo average or something
        from_single_node(nodes[0]->get_custom_style(),def_style);
    }
    blockSignals(false);
}




void node_style_form::on_style_combo_activated(int index)
{
    curstyle_id = combo_style.key( index );
    anything_changed();
}

void node_style_form::retranslate()
{
    retranslateUi(this);

    for ( knot_curve_styler::name_container::const_iterator
                        i = knot_curve_styler::get_ui_names().begin();
            i != knot_curve_styler::get_ui_names().end();
            ++i )
    {
        QIcon icon = load::icon("cusp-"+knot_curve_styler::name(i.value()));
        if ( icon.isNull() )
            icon = load::icon("cusp-other");
        style_combo->setItemIcon(combo_style[i.value()],icon);
    }
}

void node_style_form::anything_changed()
{
    emit style_changed(get_style_info());
}

