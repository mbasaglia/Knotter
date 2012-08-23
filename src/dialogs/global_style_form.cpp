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
#include "global_style_form.hpp"
#include "resource_loader.hpp"

global_style_form::global_style_form(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    // meh, Qt Designer...
    connect(color,SIGNAL(color_changed(QColor)),SIGNAL(knot_color_changed(QColor)));
    connect(knot_width_spinner,SIGNAL(valueChanged(double)),SIGNAL(knot_width_changed(double)));

    outline_pattern_combo->setItemIcon(0,load::icon("line_solid"));
    outline_pattern_combo->setItemIcon(1,load::icon("line_no"));
    outline_pattern_combo->setItemIcon(2,load::icon("line_dot"));
    outline_pattern_combo->setItemIcon(3,load::icon("line_dash"));
    outline_pattern_combo->setItemIcon(4,load::icon("line_dash_dot"));
}

double global_style_form::get_knot_width() const
{
    return knot_width_spinner->value();
}

void global_style_form::set_knot_width(double w)
{
    knot_width_spinner->setValue(w);
}


Qt::PenJoinStyle global_style_form::get_join_style() const
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

void global_style_form::set_join_style(Qt::PenJoinStyle pjs)
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


Qt::PenStyle global_style_form::pen_style_from_index(int index) const
{
    switch ( index )
    {
            case 0: return Qt::SolidLine;
    default:case 1: return Qt::NoPen;
            case 2: return Qt::DotLine;
            case 3: return Qt::DashLine;
            case 4: return Qt::DashDotLine;
    }
}

void global_style_form::change_pen()
{
    emit pen_changed(get_pen());
}

void global_style_form::change_join_style(int)
{
    emit join_style_changed(get_join_style());
}

void global_style_form::set_pen(QPen p)
{
    outline_color->set_color ( p.color() );
    outline_width_spinner->setValue(p.width());

    int index;
    switch ( p.style() )
    {
            case Qt::SolidLine: index = 0;  break;
    default:case Qt::NoPen:     index = 1;  break;
            case Qt::DotLine:   index = 2;  break;
            case Qt::DashLine:  index = 3;  break;
            case Qt::DashDotLine:index = 4; break;
    }
    outline_pattern_combo->setCurrentIndex(index);
}

QColor global_style_form::get_knot_color() const
{
    return color->color();
}

void global_style_form::set_knot_color(QColor c)
{
    color->set_color(c);
}


QPen global_style_form::get_pen() const
{
    return QPen (   outline_color->color(),
                    outline_width_spinner->value(),
                    pen_style_from_index(outline_pattern_combo->currentIndex())
                );
}
