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

#include "crossing_style_widget.hpp"
#include "resource_manager.hpp"

Crossing_Style_Widget::Crossing_Style_Widget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    label_tooltip();


    combo_edge_type->clear();
    foreach(Edge_Type* et, Resource_Manager::edge_styles())
        combo_edge_type->addItem(et->icon(),et->name(),QVariant::fromValue(et));

    connect(spin_handle_length,SIGNAL(valueChanged(double)),
            SIGNAL(handle_length_changed(double)));
    connect(spin_crossing_gap,SIGNAL(valueChanged(double)),
            SIGNAL(crossing_distance_changed(double)));
    connect(slide_edge_slide,SIGNAL(valueChanged(int)),SLOT(emit_edge_slide(int)));


    mapper.setMapping(check_crossing_gap,Edge_Style::CROSSING_DISTANCE);
    mapper.setMapping(check_handle_length,Edge_Style::HANDLE_LENGTH);
    mapper.setMapping(check_edge_slide,Edge_Style::EDGE_SLIDE);
    connect(&mapper,SIGNAL(mapped(int)),SLOT(checkbox_toggled(int)));
    foreach(QCheckBox* cb, findChildren<QCheckBox*>() )
    {
        connect(cb,SIGNAL(clicked()),&mapper,SLOT(map()));
    }

}

void Crossing_Style_Widget::set_style(const Edge_Style &st)
{
    if ( st.enabled_style & Edge_Style::CROSSING_DISTANCE )
    {
        spin_crossing_gap->setValue(st.crossing_distance);
        check_crossing_gap->setChecked(true);
    }
    else
        check_crossing_gap->setChecked(false);


    if ( st.enabled_style & Edge_Style::EDGE_SLIDE )
    {
        slide_edge_slide->setValue(st.edge_slide*100);
        check_edge_slide->setChecked(true);
    }
    else
        check_edge_slide->setChecked(false);

    if ( st.enabled_style & Edge_Style::HANDLE_LENGTH )
    {
        spin_handle_length->setValue(st.handle_length);
        check_handle_length->setChecked(true);
    }
    else
        check_handle_length->setChecked(false);

    if ( st.enabled_style & Edge_Style::EDGE_TYPE )
    {
        set_edge_type(st.edge_type );
    }
}

Edge_Style Crossing_Style_Widget::get_style() const
{
    return Edge_Style(
                spin_handle_length->value(),
                spin_crossing_gap->value(),
                slide_edge_slide->value()/100.0,
                edge_type(),
                enabled_styles()
                );
}

Edge_Style::Enabled_Styles Crossing_Style_Widget::enabled_styles() const
{
    Edge_Style::Enabled_Styles es = Edge_Style::NOTHING;
    if ( check_crossing_gap->isChecked() )
        es |= Edge_Style::CROSSING_DISTANCE;
    if ( check_edge_slide->isChecked() )
        es |= Edge_Style::EDGE_SLIDE;
    if ( check_handle_length->isChecked() )
        es |= Edge_Style::HANDLE_LENGTH;
    es |= Edge_Style::EDGE_TYPE;
    return es;
}

void Crossing_Style_Widget::set_edge_type(Edge_Type *type)
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

Edge_Type *Crossing_Style_Widget::edge_type() const
{
    return edge_type(combo_edge_type->currentIndex());
}

void Crossing_Style_Widget::hide_checkboxes()
{
    foreach(QCheckBox* cb, findChildren<QCheckBox*>())
    {
        cb->setChecked(true);
        cb->hide();
    }
}

Edge_Type *Crossing_Style_Widget::edge_type(int index) const
{
    return combo_edge_type->itemData(index).value<Edge_Type*>();
}

void Crossing_Style_Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
        {
            retranslateUi(this);
            label_tooltip();
        }
            break;
        default:
            break;
    }
}

void Crossing_Style_Widget::on_combo_edge_type_activated(int index)
{

    emit edge_type_changed(edge_type(index));
}

void Crossing_Style_Widget::checkbox_toggled(int style)
{
    emit enabled_styles_changed(enabled_styles());

    if ( qobject_cast<QCheckBox*>(mapper.mapping(style))->isChecked() )
    {
        switch(Edge_Style::Enabled_Styles_Enum(style))
        {
            case Edge_Style::CROSSING_DISTANCE:
                emit crossing_distance_changed(spin_crossing_gap->value());
                break;
            case Edge_Style::EDGE_SLIDE:
                emit crossing_distance_changed(slide_edge_slide->value());
                break;
            case Edge_Style::HANDLE_LENGTH:
                emit handle_length_changed(spin_handle_length->value());
                break;
            case Edge_Style::EDGE_TYPE:
                emit edge_type_changed(edge_type());
                break;
            case Edge_Style::NOTHING:
            case Edge_Style::EVERYTHING:
                break;
        }

    }
}

void Crossing_Style_Widget::label_tooltip()
{
    foreach(QLabel* l, findChildren<QLabel*>())
    {
        QWidget *bud = l->buddy();
        if ( bud )
            l->setToolTip(bud->toolTip());
    }
}

void Crossing_Style_Widget::emit_edge_slide(int percent)
{
    emit edge_slide_changed(percent/100.0);
}

