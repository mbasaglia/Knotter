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

#ifndef DIALOG_EDGE_PROPERTIES_HPP
#define DIALOG_EDGE_PROPERTIES_HPP

#include "ui_dialog_edge_properties.h"

class Dialog_Edge_Properties : public QDialog, private Ui::Dialog_Edge_Properties
{
    Q_OBJECT
    
public:
    explicit Dialog_Edge_Properties(QWidget *parent = 0);

    void set_style(const Edge_Style &style);
    Edge_Style edge_style() const;

protected:
    void changeEvent(QEvent *e);
};

#endif // DIALOG_EDGE_PROPERTIES_HPP
