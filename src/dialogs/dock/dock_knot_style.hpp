/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#ifndef DOCK_KNOT_STYLE_HPP
#define DOCK_KNOT_STYLE_HPP

#include "ui_dock_knot_style.h"
#include "node_style.hpp"
#include "edge_style.hpp"
#include "edge_type.hpp"


class Dock_Knot_Style : public QDockWidget, private Ui::Dock_Knot_Style
{
    Q_OBJECT


public:
    explicit Dock_Knot_Style(QWidget *parent = 0);

    void set_style(const Node_Style& nst, const Edge_Style& est );
    Node_Style node_style() const;
    Edge_Style edge_style() const;
    Cusp_Shape* cusp_shape() const;

    void hide_checkboxes();
    void set_style(const Edge_Style& st );
    Edge_Style get_style() const;

protected:
    void changeEvent(QEvent *e);


signals:
    void cusp_angle_changed(double);
    void handle_length_changed(double);
    void cusp_distance_changed(double);
    void cusp_shape_changed(Cusp_Shape*);
    void crossing_distance_changed(double);
    void edge_slide_changed(double);

private slots:
    void on_combo_cusp_shape_activated(int index);
    void reload_cusp_shapes();
    void emit_edge_slide(int percent);


private:
    /// Copy tooltip from buddy to label
    void label_tooltip();

    /// Cusp shape associated with combo box index
    Cusp_Shape* cusp_shape(int index) const;

    void load_cusp_shapes();
};

#endif // DOCK_KNOT_STYLE_HPP
