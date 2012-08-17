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
#include "style_dialog.hpp"

/**
    \todo PenStyle
*/
StyleDialog::StyleDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    Node* a = preview->add_node(QPointF(-64,0));
    Node* b = preview->add_node(QPointF(0,-64));
    Node* c = preview->add_node(QPointF(64,0));
    Node* d = preview->add_node(QPointF(128,0));
    Node* e = preview->add_node(QPointF(128+64,0));
    Node* f = preview->add_node(QPointF(128+64,-64));
    preview->add_edge(a,b);
    preview->add_edge(c,b);
    preview->add_edge(c,d);
    preview->add_edge(e,d);
    preview->add_edge(e,f);
    preview->setSceneRect(preview->scene()->itemsBoundingRect());

    preview->setInteractive(false);

    combo_style[knot_curve_styler::idof("pointed")]=0;
    combo_style[knot_curve_styler::idof("ogee")]=1;
    combo_style[knot_curve_styler::idof("advanced_poly")]=2;
    combo_style[knot_curve_styler::idof("simple_poly")]=3;
}

knot_curve_styler::style_id StyleDialog::get_style_id() const
{
    return curstyle;
}

void StyleDialog::set_style_id(knot_curve_styler::style_id sid)
{
    curstyle = sid;
    style_combo->setCurrentIndex(combo_style[curstyle]);
    preview->set_curve_style(curstyle);
}


void StyleDialog::clicked(QAbstractButton *button)
{
    QDialogButtonBox::StandardButton sb = buttonBox->standardButton ( button );
    if ( sb == QDialogButtonBox::Apply || sb == QDialogButtonBox::Ok )
        emit changed();
}

void StyleDialog::on_knot_width_spinner_valueChanged(double arg1)
{
    preview->set_width(arg1);
}

void StyleDialog::on_outline_width_spinner_valueChanged(double arg1)
{
    QPen p = preview->get_pen();
    p.setWidth ( arg1 );
    preview->set_pen ( p );
}

void StyleDialog::on_color_color_changed(const QColor & c)
{
    QBrush b = preview->get_brush();
    b.setColor ( c );
    preview->set_brush ( b );
}

void StyleDialog::on_outline_color_color_changed(const QColor & c)
{
    QPen p = preview->get_pen();
    p.setColor ( c );
    preview->set_pen ( p );

}

void StyleDialog::on_style_combo_activated(int index)
{
    curstyle = combo_style.key( index );
    preview->set_curve_style(curstyle);
}

void StyleDialog::on_cusp_angle_spinner_valueChanged(double arg1)
{
    preview->set_cusp_angle(arg1);
}

void StyleDialog::on_handle_length_spinner_valueChanged(double arg1)
{
    preview->set_handle_length(arg1);
}

void StyleDialog::on_crossing_gap_spinner_valueChanged(double arg1)
{
    preview->set_crossing_distance(arg1);
}

void StyleDialog::on_point_combo_activated(int index)
{
    if ( index == 0 )
        preview->set_join_style(Qt::BevelJoin);
    else if ( index == 1 )
        preview->set_join_style(Qt::MiterJoin);
    else if ( index == 2 )
        preview->set_join_style(Qt::RoundJoin);
}


Qt::PenJoinStyle StyleDialog::get_join_style() const
{
    switch ( point_combo->currentIndex() )
    {
        default: case 0:
            return Qt::BevelJoin;
        case 1:
            return Qt::MiterJoin;
        case 2:
            return Qt::RoundJoin;
    }
}

void StyleDialog::set_join_style(Qt::PenJoinStyle pjs)
{
    switch ( pjs )
    {
        default: case Qt::BevelJoin:
            point_combo->setCurrentIndex(0);
            break;
        case Qt::MiterJoin:
            point_combo->setCurrentIndex(1);
            break;
        case Qt::RoundJoin:
            point_combo->setCurrentIndex(2);
            break;
    }
}
