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

#include "dock_borders.hpp"

Dock_Borders::Dock_Borders(QWidget *parent) :
    QDockWidget(parent)
{
    setupUi(this);
    connect(border_widget,SIGNAL(bordersChanged(Border_List)),
            SIGNAL(borders_changed(Border_List)));
    connect(check_enable,SIGNAL(toggled(bool)),SIGNAL(borders_enabled(bool)));
}

void Dock_Borders::set_borders(Border_List bl)
{
    border_widget->setBorders(bl);
}

void Dock_Borders::enable_borders(bool b)
{
    check_enable->setChecked(b);
}

void Dock_Borders::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}
