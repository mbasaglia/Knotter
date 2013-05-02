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

#include "cusp_style_widget.hpp"
#include "resource_manager.hpp"


Cusp_Style_Widget::Cusp_Style_Widget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    foreach(Cusp_Shape* cs, Resource_Manager::cusp_shapes())
        combo_cusp_shape->addItem(cs->icon(),cs->name(),
                                  QVariant::fromValue(cs) );

    connect(spin_handle_length,SIGNAL(valueChanged(double)),
            SIGNAL(handle_length_changed(double)));
    connect(spin_crossing_gap,SIGNAL(valueChanged(double)),
            SIGNAL(crossing_distance_changed(double)));
    connect(spin_cusp_angle,SIGNAL(valueChanged(double)),
            SIGNAL(cusp_angle_changed(double)));
    connect(spin_cusp_distance,SIGNAL(valueChanged(double)),
            SIGNAL(cusp_distance_changed(double)));

}

void Cusp_Style_Widget::set_style(const Node_Style &st)
{
    if ( st.enabled_style & Node_Style::CROSSING_DISTANCE )
    {
        spin_crossing_gap->setValue(st.crossing_distance);
        check_crossing_gap->setChecked(true);
    }
    else
        check_crossing_gap->setChecked(false);

    if ( st.enabled_style & Node_Style::CUSP_SHAPE )
    {
        for(int i = 0; i < combo_cusp_shape->count(); i++ )
        {
            Cusp_Shape* cs = combo_cusp_shape->itemData(i).value<Cusp_Shape*>();
            if ( cs->name() == st.cusp_shape->name() )
            {
                combo_cusp_shape->setCurrentIndex(i);
                break;
            }
        }
        check_cusp_shape->setChecked(true);
    }
    else
        check_cusp_shape->setChecked(false);

    if ( st.enabled_style & Node_Style::CUSP_ANGLE )
    {
        spin_cusp_angle->setValue(st.cusp_angle);
        check_cusp_angle->setChecked(true);
    }
    else
        check_cusp_angle->setChecked(false);

    if ( st.enabled_style & Node_Style::CUSP_DISTANCE )
    {
        spin_cusp_distance->setValue(st.cusp_distance);
        check_cusp_distance->setChecked(true);
    }
    else
        check_cusp_distance->setChecked(false);

    if ( st.enabled_style & Node_Style::HANDLE_LENGTH )
    {
        spin_handle_length->setValue(st.handle_length);
        check_handle_length->setChecked(true);
    }
    else
        check_handle_length->setChecked(false);
}

void Cusp_Style_Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
        {
            retranslateUi(this);

            // reload cusp shapes
            int current_index = combo_cusp_shape->currentIndex();
            Cusp_Shape* current_shape = combo_cusp_shape->itemData(current_index)
                                            .value<Cusp_Shape*>();
            blockSignals(true);
            combo_cusp_shape->clear();

            for( int i = 0; i < Resource_Manager::cusp_shapes().size(); i++ )
            {
                Cusp_Shape* cs = Resource_Manager::cusp_shapes()[i];
                if ( cs == current_shape )
                    current_index = i;
                combo_cusp_shape->addItem(cs->icon(),cs->name(),
                                          QVariant::fromValue(cs) );
            }
            blockSignals(false);

            if ( current_index > combo_cusp_shape->count() )
                current_index = 0;

            combo_cusp_shape->setCurrentIndex(current_index);
            Cusp_Shape* new_shape = combo_cusp_shape->itemData(current_index)
                                            .value<Cusp_Shape*>();
            if ( new_shape != current_shape )
                emit cusp_shape_changed(new_shape);
        }
            break;
        default:
            break;
    }
}

void Cusp_Style_Widget::on_combo_cusp_shape_activated(int index)
{
    emit cusp_shape_changed(combo_cusp_shape->itemData(index).value<Cusp_Shape*>());
}
