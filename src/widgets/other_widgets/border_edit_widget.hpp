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

#ifndef BORDER_EDIT_WIDGET_HPP
#define BORDER_EDIT_WIDGET_HPP

#include "ui_border_edit_widget.h"

class Border_Edit_Widget : public QWidget, private Ui::Border_Edit_Widget
{
    Q_OBJECT
    
public:
    explicit Border_Edit_Widget(QWidget *parent = 0);

    QColor color() const;
    void set_color(QColor c);

    double border_width() const;
    void set_border_width(double w);

    void swap_values(Border_Edit_Widget* other);
    
protected:
    void changeEvent(QEvent *e);

signals:
    void borderChanged();
};

#endif // BORDER_EDIT_WIDGET_HPP
