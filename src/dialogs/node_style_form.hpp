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
#include "node.hpp"

class node_style_form : public QWidget, private Ui::node_style_form
{
        Q_OBJECT

    protected:
        QMap<knot_curve_styler::style_id,int> combo_style;
        knot_curve_styler::style_id curstyle_id;

    public:
        explicit node_style_form( QWidget *parent = 0);

        /**
            \brief Initialize UI status (values and checkboxes)
        */
        void set_style_info ( styleinfo si );

        styleinfo get_style_info() const;

        /**
            \brief Initialize UI status for node-specific settings.
            \param custom style for the custom node (stuff that will be enabled)
            \param def    default style, values are loaded for disabled options
        */
        void from_single_node ( styleinfo custom, styleinfo def );

        /**
            \brief Initialize UI status from group of nodes
        */
        void from_multi_nodes ( node_list nodes, styleinfo def_style );

        /**
            \brief Make checkboxes invisible and uncheckable (global style mode)
        */
        void global_style_mode();


    signals:
        void style_changed ( styleinfo si );

    protected slots:
        void anything_changed();

    private slots:
        void on_style_combo_activated(int index);
        void retranslate();
};

#endif // NODE_STYLE_FORM_HPP
