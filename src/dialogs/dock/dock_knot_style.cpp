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

#include "dock_knot_style.hpp"
#include "resource_manager.hpp"

Dock_Knot_Style::Dock_Knot_Style(QWidget *parent) :
    QDockWidget(parent)
{
    setupUi(this);
    label_tooltip();


    combo_cusp_shape->clear();
    foreach(Cusp_Shape* cs, Resource_Manager::cusp_shapes())
        combo_cusp_shape->addItem(cs->icon(),cs->name(),
                                  QVariant::fromValue(cs) );

    connect(spin_handle_length,SIGNAL(valueChanged(double)),
            SIGNAL(handle_length_changed(double)));
    connect(spin_cusp_angle,SIGNAL(valueChanged(double)),
            SIGNAL(cusp_angle_changed(double)));
    connect(spin_cusp_distance,SIGNAL(valueChanged(double)),
            SIGNAL(cusp_distance_changed(double)));
    connect(spin_crossing_gap,SIGNAL(valueChanged(double)),
            SIGNAL(crossing_distance_changed(double)));
    connect(slide_edge_slide,SIGNAL(valueChanged(int)),SLOT(emit_edge_slide(int)));

    connect(Resource_Manager::pointer,SIGNAL(cusp_shapes_changed()),
            SLOT(reload_cusp_shapes()));

}

void Dock_Knot_Style::set_style(const Node_Style &nst, const Edge_Style &est)
{

    for(int i = 0; i < combo_cusp_shape->count(); i++ )
    {
        Cusp_Shape* cs = cusp_shape(i);
        if ( nst.cusp_shape && cs->name() == nst.cusp_shape->name() )
        {
            combo_cusp_shape->setCurrentIndex(i);
            break;
        }
    }

    spin_cusp_angle->setValue(nst.cusp_angle);

    spin_cusp_distance->setValue(nst.cusp_distance);
    //spin_handle_length->setValue(nst.handle_length);

    spin_crossing_gap->setValue(est.crossing_distance);
    slide_edge_slide->setValue(est.edge_slide*100);
    spin_handle_length->setValue(est.handle_length);
}

Node_Style Dock_Knot_Style::node_style() const
{
    return Node_Style(
                spin_cusp_angle->value(),
                spin_handle_length->value(),
                spin_cusp_distance->value(),
                cusp_shape(),
                Node_Style::EVERYTHING
                );
}


Edge_Style Dock_Knot_Style::edge_style() const
{
    return Edge_Style(
                spin_handle_length->value(),
                spin_crossing_gap->value(),
                slide_edge_slide->value()/100.0,
                Resource_Manager::default_edge_type(),
                Edge_Style::EVERYTHING
                );
}


Cusp_Shape *Dock_Knot_Style::cusp_shape() const
{
    return cusp_shape(combo_cusp_shape->currentIndex());
}


Cusp_Shape *Dock_Knot_Style::cusp_shape(int index) const
{
    return combo_cusp_shape->itemData(index).value<Cusp_Shape*>();
}


void Dock_Knot_Style::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
        {
            retranslateUi(this);
            label_tooltip();
            reload_cusp_shapes();
        }
            break;
        default:
            break;
    }
}

void Dock_Knot_Style::on_combo_cusp_shape_activated(int index)
{
    emit cusp_shape_changed(cusp_shape(index));
}


void Dock_Knot_Style::label_tooltip()
{
    foreach(QLabel* l, findChildren<QLabel*>())
    {
        QWidget *bud = l->buddy();
        if ( bud )
            l->setToolTip(bud->toolTip());
    }
}

void Dock_Knot_Style::reload_cusp_shapes()
{

    // reload cusp shapes
    int current_index = combo_cusp_shape->currentIndex();
    Cusp_Shape* current_shape = cusp_shape();

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

    if ( current_index >= combo_cusp_shape->count() )
        current_index = 0;

    combo_cusp_shape->setCurrentIndex(current_index);

    blockSignals(false);

    Cusp_Shape* new_shape = cusp_shape(current_index);

    if ( new_shape != current_shape )
        emit cusp_shape_changed(new_shape);
}


void Dock_Knot_Style::emit_edge_slide(int percent)
{
    emit edge_slide_changed(percent/100.0);
}

