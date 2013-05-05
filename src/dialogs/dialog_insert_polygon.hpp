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

#ifndef DIALOG_INSERT_POLYGON_HPP
#define DIALOG_INSERT_POLYGON_HPP

#include "ui_dialog_insert_polygon.h"

class Dialog_Insert_Polygon : public QDialog, private Ui::Dialog_Insert_Polygon
{
    Q_OBJECT
    
public:
    explicit Dialog_Insert_Polygon(QWidget *parent = 0);

    bool middle_node() const;
    int sides() const;
    
protected:
    void changeEvent(QEvent *e);
};

#endif // DIALOG_INSERT_POLYGON_HPP
