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
#ifndef NODE_STYLE_FORM_HPP
#define NODE_STYLE_FORM_HPP

#include "ui_node_style_form.h"
#include "knot_curve_style.hpp"

class node_style_form : public QWidget, private Ui::node_style_form
{
        Q_OBJECT

    protected:
        QMap<knot_curve_styler::style_id,int> combo_style;
        knot_curve_styler::style_id curstyle_id;

    public:
        explicit node_style_form(QWidget *parent = 0);

        void set_style_info ( styleinfo si );
        styleinfo get_style_info() const;


    signals:
        void style_changed ( styleinfo si );

    protected slots:
        void anything_changed();

    private slots:
        void on_style_combo_activated(int index);
};

#endif // NODE_STYLE_FORM_HPP
