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

#include "dialog_edge_properties.hpp"

Dialog_Edge_Properties::Dialog_Edge_Properties(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void Dialog_Edge_Properties::set_style(const Edge_Style &style)
{
    crossing_style_widget->set_style(style);
}

Edge_Style Dialog_Edge_Properties::edge_style() const
{
    return crossing_style_widget->get_style();
}

void Dialog_Edge_Properties::changeEvent(QEvent *e)
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
