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

#ifndef CUSP_STYLE_WIDGET_HPP
#define CUSP_STYLE_WIDGET_HPP

#include "ui_cusp_style_widget.h"
#include "node_style.hpp"

class Cusp_Style_Widget : public QWidget, private Ui::Cusp_Style_Widget
{
    Q_OBJECT
    
public:
    explicit Cusp_Style_Widget(QWidget *parent = 0);

    void set_style(const Node_Style& st );
    Node_Style get_style() const;

    Node_Style::Enabled_Styles enabled_styles() const;
    Cusp_Shape* cusp_shape() const;

    void hide_checkboxes();
    
protected:
    void changeEvent(QEvent *e);

signals:
    void cusp_angle_changed(double);
    void handle_length_changed(double);
    void crossing_distance_changed(double);
    void cusp_distance_changed(double);
    void cusp_shape_changed(Cusp_Shape*);
    void enabled_styles_changed(Node_Style::Enabled_Styles);

private slots:
    void on_combo_cusp_shape_activated(int index);
    void checkbox_toggled();

private:
    /// Copy tooltip from buddy to label
    void label_tooltip();

    /// Cusp shape associaated with combo box index
    Cusp_Shape* cusp_shape(int index) const;
};

#endif // CUSP_STYLE_WIDGET_HPP
