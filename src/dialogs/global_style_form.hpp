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
#ifndef GLOBAL_STYLE_FORM_HPP
#define GLOBAL_STYLE_FORM_HPP

#include "ui_global_style_form.h"

class global_style_form : public QWidget, private Ui::global_style_form
{
        Q_OBJECT
        
    public:
        explicit global_style_form(QWidget *parent = 0);

        double get_knot_width() const;
        void set_knot_width (double w);

        QPen get_pen() const;
        void set_pen ( QPen p );

        QColor get_knot_color() const;
        void set_knot_color( QColor c );

        Qt::PenJoinStyle get_join_style() const;
        void set_join_style(Qt::PenJoinStyle pjs);

    signals:
        void pen_changed(QPen);
        void knot_width_changed(double);
        void knot_color_changed(QColor);
        void join_style_changed(Qt::PenJoinStyle);

    protected:
        Qt::PenStyle pen_style_from_index(int index) const;

    protected slots:
        void change_pen();
        void change_join_style(int);
};

#endif // GLOBAL_STYLE_FORM_HPP
