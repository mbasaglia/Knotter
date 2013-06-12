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

#include "cusp_style_dialog.hpp"

Cusp_Style_Dialog::Cusp_Style_Dialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    cusp_style_widget->hide_edge_type();
}

void Cusp_Style_Dialog::set_style(const Node_Style& style)
{
    cusp_style_widget->set_style(style);
}

Node_Style Cusp_Style_Dialog::node_style() const
{
    return cusp_style_widget->get_style();
}

void Cusp_Style_Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}
