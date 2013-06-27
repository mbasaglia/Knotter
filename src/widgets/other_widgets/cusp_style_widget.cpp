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
    label_tooltip();


    combo_cusp_shape->clear();
    foreach(Cusp_Shape* cs, Resource_Manager::cusp_shapes())
        combo_cusp_shape->addItem(cs->icon(),cs->name(),
                                  QVariant::fromValue(cs) );

    combo_edge_type->clear();
    foreach(Edge_Type* et, Resource_Manager::edge_styles())
        combo_edge_type->addItem(et->icon(),et->name(),QVariant::fromValue(et));

    connect(spin_handle_length,SIGNAL(valueChanged(double)),
            SIGNAL(handle_length_changed(double)));
    connect(spin_crossing_gap,SIGNAL(valueChanged(double)),
            SIGNAL(crossing_distance_changed(double)));
    connect(spin_cusp_angle,SIGNAL(valueChanged(double)),
            SIGNAL(cusp_angle_changed(double)));
    connect(spin_cusp_distance,SIGNAL(valueChanged(double)),
            SIGNAL(cusp_distance_changed(double)));
    connect(slide_edge_slide,SIGNAL(valueChanged(int)),SLOT(emit_edge_slide(int)));

    connect(Resource_Manager::pointer,SIGNAL(cusp_shapes_changed()),
            SLOT(reload_cusp_shapes()));

    mapper.setMapping(check_crossing_gap,Knot_Style::CROSSING_DISTANCE);
    mapper.setMapping(check_cusp_angle,Knot_Style::CUSP_ANGLE);
    mapper.setMapping(check_cusp_distance,Knot_Style::CUSP_DISTANCE);
    mapper.setMapping(check_cusp_shape,Knot_Style::CUSP_SHAPE);
    mapper.setMapping(check_handle_length,Knot_Style::HANDLE_LENGTH);
    mapper.setMapping(check_edge_slide,Knot_Style::EDGE_SLIDE);
    connect(&mapper,SIGNAL(mapped(int)),SLOT(checkbox_toggled(int)));
    foreach(QCheckBox* cb, findChildren<QCheckBox*>() )
    {
        connect(cb,SIGNAL(clicked()),&mapper,SLOT(map()));
    }

}

void Cusp_Style_Widget::set_style(const Knot_Style &st)
{
    if ( st.enabled_style & Knot_Style::CROSSING_DISTANCE )
    {
        spin_crossing_gap->setValue(st.crossing_distance);
        check_crossing_gap->setChecked(true);
    }
    else
        check_crossing_gap->setChecked(false);


    if ( st.enabled_style & Knot_Style::EDGE_SLIDE )
    {
        slide_edge_slide->setValue(st.edge_slide*100);
        check_edge_slide->setChecked(true);
    }
    else
        check_edge_slide->setChecked(false);

    if ( st.enabled_style & Knot_Style::CUSP_SHAPE )
    {
        for(int i = 0; i < combo_cusp_shape->count(); i++ )
        {
            Cusp_Shape* cs = cusp_shape(i);
            if ( st.cusp_shape && cs->name() == st.cusp_shape->name() )
            {
                combo_cusp_shape->setCurrentIndex(i);
                break;
            }
        }
        check_cusp_shape->setChecked(true);
    }
    else
        check_cusp_shape->setChecked(false);

    if ( st.enabled_style & Knot_Style::CUSP_ANGLE )
    {
        spin_cusp_angle->setValue(st.cusp_angle);
        check_cusp_angle->setChecked(true);
    }
    else
        check_cusp_angle->setChecked(false);

    if ( st.enabled_style & Knot_Style::CUSP_DISTANCE )
    {
        spin_cusp_distance->setValue(st.cusp_distance);
        check_cusp_distance->setChecked(true);
    }
    else
        check_cusp_distance->setChecked(false);

    if ( st.enabled_style & Knot_Style::HANDLE_LENGTH )
    {
        spin_handle_length->setValue(st.handle_length);
        check_handle_length->setChecked(true);
    }
    else
        check_handle_length->setChecked(false);

    if ( st.enabled_style & Knot_Style::EDGE_TYPE )
    {
        set_edge_type(st.edge_type );
    }
}

Knot_Style Cusp_Style_Widget::get_style() const
{
    return Knot_Style(
                spin_cusp_angle->value(),
                spin_handle_length->value(),
                spin_crossing_gap->value(),
                spin_cusp_distance->value(),
                slide_edge_slide->value()/100.0,
                cusp_shape(),
                edge_type(),
                enabled_styles()
                );
}

Knot_Style::Enabled_Styles Cusp_Style_Widget::enabled_styles() const
{
    Knot_Style::Enabled_Styles es = Knot_Style::NOTHING;
    if ( check_crossing_gap->isChecked() )
        es |= Knot_Style::CROSSING_DISTANCE;
    if ( check_edge_slide->isChecked() )
        es |= Knot_Style::EDGE_SLIDE;
    if ( check_cusp_angle->isChecked() )
        es |= Knot_Style::CUSP_ANGLE;
    if ( check_cusp_distance->isChecked() )
        es |= Knot_Style::CUSP_DISTANCE;
    if ( check_cusp_shape->isChecked() )
        es |= Knot_Style::CUSP_SHAPE;
    if ( check_handle_length->isChecked() )
        es |= Knot_Style::HANDLE_LENGTH;
    return es;
}

Cusp_Shape *Cusp_Style_Widget::cusp_shape() const
{
    return cusp_shape(combo_cusp_shape->currentIndex());
}

void Cusp_Style_Widget::set_edge_type(Edge_Type *type)
{

    for(int i = 0; i < combo_edge_type->count(); i++ )
    {
        Edge_Type* cs = edge_type(i);
        if ( cs == type )
        {
            combo_edge_type->setCurrentIndex(i);
            break;
        }
    }
}

Edge_Type *Cusp_Style_Widget::edge_type() const
{
    return edge_type(combo_edge_type->currentIndex());
}

void Cusp_Style_Widget::hide_checkboxes()
{
    foreach(QCheckBox* cb, findChildren<QCheckBox*>())
    {
        cb->setChecked(true);
        cb->hide();
    }
}

void Cusp_Style_Widget::hide_edge_type()
{
    layout()->removeWidget(label_edge_type);
    combo_edge_type->hide();
}

Cusp_Shape *Cusp_Style_Widget::cusp_shape(int index) const
{
    return combo_cusp_shape->itemData(index).value<Cusp_Shape*>();
}

Edge_Type *Cusp_Style_Widget::edge_type(int index) const
{
    return combo_edge_type->itemData(index).value<Edge_Type*>();
}


void Cusp_Style_Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
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

void Cusp_Style_Widget::on_combo_cusp_shape_activated(int index)
{
    emit cusp_shape_changed(cusp_shape(index));
}


void Cusp_Style_Widget::on_combo_edge_type_activated(int index)
{

    emit edge_type_changed(edge_type(index));
}

void Cusp_Style_Widget::checkbox_toggled(int style)
{
    emit enabled_styles_changed(enabled_styles());

    if ( qobject_cast<QCheckBox*>(mapper.mapping(style))->isChecked() )
    {
        switch(Knot_Style::Enabled_Styles_Enum(style))
        {
            case Knot_Style::CROSSING_DISTANCE:
                emit crossing_distance_changed(spin_crossing_gap->value());
                break;
            case Knot_Style::EDGE_SLIDE:
                emit crossing_distance_changed(slide_edge_slide->value());
                break;
            case Knot_Style::CUSP_ANGLE:
                emit cusp_angle_changed(spin_cusp_angle->value());
                break;
            case Knot_Style::CUSP_DISTANCE:
                emit cusp_distance_changed(spin_cusp_distance->value());
                break;
            case Knot_Style::CUSP_SHAPE:
                emit cusp_shape_changed(cusp_shape(combo_cusp_shape->currentIndex()));
                break;
            case Knot_Style::HANDLE_LENGTH:
                emit handle_length_changed(spin_handle_length->value());
                break;
            case Knot_Style::EDGE_TYPE:
                emit edge_type_changed(edge_type());
                break;
            case Knot_Style::NOTHING:
            case Knot_Style::EVERYTHING:
            case Knot_Style::NODE_EVERYTHING:
            case Knot_Style::EDGE_EVERYTHING:
                break;
        }

    }
}

void Cusp_Style_Widget::label_tooltip()
{
    foreach(QLabel* l, findChildren<QLabel*>())
    {
        QWidget *bud = l->buddy();
        if ( bud )
            l->setToolTip(bud->toolTip());
    }
}

void Cusp_Style_Widget::reload_cusp_shapes()
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

void Cusp_Style_Widget::emit_edge_slide(int percent)
{
    emit edge_slide_changed(percent/100.0);
}

