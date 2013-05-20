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

#include "border_widget.hpp"
#include "border_edit_widget.hpp"


Border_Widget::Border_Widget(QWidget *parent)
    : Abstract_Widget_List(parent)
{
    connect(this,SIGNAL(removed(int)),SLOT(handle_removed(int)));
    connect(&mapper,SIGNAL(mapped(int)),SLOT(border_changed(int)));
}

void Border_Widget::setBorders(const Border_List &cols)
{
    clear();
    m_borders = cols;
    for( int i = 0; i < m_borders.size(); i++ )
        append_widget(i);
    emit bordersChanged(m_borders);
}

void Border_Widget::swap(int a, int b)
{
    Border_Edit_Widget* sa = widget_cast<Border_Edit_Widget>(a);
    Border_Edit_Widget* sb = widget_cast<Border_Edit_Widget>(b);
    if ( sa && sb )
    {
        sa->swap_values(sb);
        emit bordersChanged(m_borders);
    }
}

void Border_Widget::append()
{
    m_borders.push_back(Knot_Border(Qt::black,1));
    append_widget(m_borders.size()-1);
    emit bordersChanged(m_borders);
}

void Border_Widget::handle_removed(int i)
{
    m_borders.removeAt(i);
    emit bordersChanged(m_borders);
}

void Border_Widget::border_changed(int row)
{
    Border_Edit_Widget *cs = widget_cast<Border_Edit_Widget>(row);
    if ( cs )
    {
        m_borders[row].color = cs->color();
        m_borders[row].width = cs->border_width();
        emit bordersChanged(m_borders);
    }
}

void Border_Widget::append_widget(int col)
{
    Border_Edit_Widget* bew = new Border_Edit_Widget;

    mapper.setMapping(bew,col);
    connect(bew,SIGNAL(borderChanged()),&mapper,SLOT(map()));
    appendWidget(bew);
    //setRowHeight(count()-1,22);
}
