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

#include "border_edit_widget.hpp"

Border_Edit_Widget::Border_Edit_Widget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    connect(color_selector,SIGNAL(colorChanged(QColor)),SIGNAL(borderChanged()));
    connect(spin_width,SIGNAL(valueChanged(double)),SIGNAL(borderChanged()));
}

void Border_Edit_Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

QColor Border_Edit_Widget::color() const
{
    return color_selector->color();
}


void Border_Edit_Widget::set_color(QColor c)
{
    color_selector->setColor(c);
}

double Border_Edit_Widget::border_width() const
{
    return spin_width->value();
}

void Border_Edit_Widget::set_border_width(double w)
{
    spin_width->setValue(w);
}

void Border_Edit_Widget::swap_values(Border_Edit_Widget *other)
{
    blockSignals(true);
    other->blockSignals(true);

    QColor c = color();
    double w = border_width();

    set_color(other->color());
    set_border_width(other->border_width());

    other->set_color(c);
    other->set_border_width(w);

    blockSignals(false);
    other->blockSignals(false);
}
