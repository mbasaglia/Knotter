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
#include <QSignalMapper>
#include "edge_type.hpp"

class Cusp_Style_Widget : public QWidget, private Ui::Cusp_Style_Widget
{
    Q_OBJECT

    QSignalMapper mapper;
    
public:
    explicit Cusp_Style_Widget(QWidget *parent = 0);

    void set_style(const Knot_Style& st );
    Knot_Style get_style() const;

    Knot_Style::Enabled_Styles enabled_styles() const;
    Cusp_Shape* cusp_shape() const;

    void hide_checkboxes();
    void hide_edge_type();
    
protected:
    void changeEvent(QEvent *e);

signals:
    void cusp_angle_changed(double);
    void handle_length_changed(double);
    void crossing_distance_changed(double);
    void cusp_distance_changed(double);
    void cusp_shape_changed(Cusp_Shape*);
    void edge_type_changed(Edge_Type*);
    void enabled_styles_changed(Knot_Style::Enabled_Styles);
    void edge_slide_changed(double);

private slots:
    void on_combo_cusp_shape_activated(int index);
    void checkbox_toggled(int style);
    void reload_cusp_shapes();
    void emit_edge_slide(int percent);

    void on_combo_edge_type_activated(int index);

private:
    /// Copy tooltip from buddy to label
    void label_tooltip();

    /// Cusp shape associated with combo box index
    Cusp_Shape* cusp_shape(int index) const;

    /// Edge type associated with combo box index
    Edge_Type* edge_type(int index) const;
    Edge_Type* edge_type() const;

    void set_edge_type(Edge_Type *type);

    void load_cusp_shapes();
};

#endif // CUSP_STYLE_WIDGET_HPP
