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

#include "dock_knot_display.hpp"
#include "color_delegate.hpp"

#include "color_selector.hpp"

Dock_Knot_Display::Dock_Knot_Display(QWidget *parent) :
    QDockWidget(parent)
{
    setupUi(this);
    setWidget(central_widget);
    /*list_colors->setModel(&color_list_model);
    color_list_model.insert_new();
    list_colors->setIndexWidget(color_list_model.index(0,0),new Color_Selector);
    //list_colors->setItemDelegate(new Color_Delegate);*/

    connect(list_colors,SIGNAL(colorsChanged(QList<QColor>)),
            SIGNAL(colors_changed(QList<QColor>)));


    connect(spin_width,SIGNAL(valueChanged(double)), SIGNAL(width_changed(double)));

    connect(check_custom_colors,SIGNAL(toggled(bool)),SIGNAL(colors_enabled(bool)));

    combo_joint->setItemData(0,(int)Qt::BevelJoin);
    combo_joint->setItemData(1,(int)Qt::MiterJoin);
    combo_joint->setItemData(2,(int)Qt::RoundJoin);

}

void Dock_Knot_Display::set_colors(const QList<QColor> &c)
{
    list_colors->setColors(c);
}

void Dock_Knot_Display::set_join_style(Qt::PenJoinStyle s)
{
    for ( int i = 0; i < combo_joint->count(); i++ )
    {
        if ( combo_joint->itemData(i).toInt() == int(s) )
        {
            combo_joint->setCurrentIndex(i);
            break;
        }
    }
}

void Dock_Knot_Display::set_width(double w)
{
    spin_width->setValue(w);
}

void Dock_Knot_Display::toggle_custom_colors(bool b)
{
    check_custom_colors->setChecked(b);
}

void Dock_Knot_Display::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

void Dock_Knot_Display::on_list_colors_removed(int)
{
    if ( list_colors->count() == 0 )
        list_colors->append();
}

void Dock_Knot_Display::on_combo_joint_currentIndexChanged(int index)
{
    emit join_style_changed(Qt::PenJoinStyle(combo_joint->itemData(index).toInt()));
}
