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

#ifndef BORDER_WIDGET_HPP
#define BORDER_WIDGET_HPP

#include "abstract_widget_list.hpp"
#include "knot_border.hpp"

class Border_Widget : public Abstract_Widget_List
{
    Q_OBJECT


private:
    Border_List m_borders;
    QSignalMapper mapper;

public:
    explicit Border_Widget(QWidget *parent = 0);


    const Border_List& borders() const { return m_borders; }
    void setBorders(const Border_List& borders);

    void swap(int a, int b);

    void append();

signals:
    void bordersChanged(const Border_List&);

private slots:
    void handle_removed(int);
    void border_changed(int row);

private:
   void  append_widget(int col);
};

#endif // BORDER_WIDGET_HPP
