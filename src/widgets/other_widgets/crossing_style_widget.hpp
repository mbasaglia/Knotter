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

#ifndef CROSSING_STYLE_WIDGET_HPP
#define CROSSING_STYLE_WIDGET_HPP

#include "ui_crossing_style_widget.h"
#include "edge_style.hpp"
#include <QSignalMapper>
#include "edge_type.hpp"

class Crossing_Style_Widget : public QWidget, private Ui::Crossing_Style_Widget
{
    Q_OBJECT

    QSignalMapper mapper;
    
public:
    explicit Crossing_Style_Widget(QWidget *parent = 0);

    void set_style(const Edge_Style& st );
    Edge_Style get_style() const;

    Edge_Style::Enabled_Styles enabled_styles() const;

protected:
    void changeEvent(QEvent *e);

signals:
    void handle_length_changed(double);
    void crossing_distance_changed(double);
    void edge_type_changed(Edge_Type*);
    void enabled_styles_changed(Edge_Style::Enabled_Styles);
    void edge_slide_changed(double);

private slots:
    void checkbox_toggled(int style);
    void emit_edge_slide(int percent);
    void reload_edge_types();
    void on_combo_edge_type_activated(int index);

private:
    /// Copy tooltip from buddy to label
    void label_tooltip();

    /// Edge type associated with combo box index
    Edge_Type* edge_type(int index) const;
    Edge_Type* edge_type() const;

    void set_edge_type(Edge_Type *type);
};

#endif // CROSSING_STYLE_WIDGET_HPP
