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

#ifndef DOCK_BORDERS_HPP
#define DOCK_BORDERS_HPP

#include "ui_dock_borders.h"

class Dock_Borders : public QDockWidget, private Ui::Dock_Borders
{
    Q_OBJECT
    
public:
    explicit Dock_Borders(QWidget *parent = 0);
    
    void set_borders(Border_List bl);

protected:
    void changeEvent(QEvent *e);

signals:
    void borders_changed(Border_List);
};

#endif // DOCK_BORDERS_HPP
