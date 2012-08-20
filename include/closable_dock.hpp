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
#ifndef CLOSABLE_DOCK_HPP
#define CLOSABLE_DOCK_HPP

#include <QDockWidget>
/**
    \brief DockWidget with some extra signals

    Reimplements QDockWidget to make synch with checkable menu items possible
*/
class closable_dock : public QDockWidget
{
    Q_OBJECT
public:
    closable_dock ( QWidget* parent ) : QDockWidget (parent) {}
signals:
    void closed();
    void openChanged(bool);
protected:
    void closeEvent(QCloseEvent *event)
    {
        emit openChanged(false);
        emit closed();
        QDockWidget::closeEvent(event);
    }
};


#endif // CLOSABLE_DOCK_HPP
