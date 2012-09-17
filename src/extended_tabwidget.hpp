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
#ifndef EXTENTED_TABWIDGET_HPP
#define EXTENTED_TABWIDGET_HPP

#include <QTabWidget>
#include <QTabBar>

class extended_QTabBar : public QTabBar
{
    Q_OBJECT

    private:
        QPoint drag_start;
        int    drag_index;

    public:
        extended_QTabBar( QWidget * parent = 0 ) ;

    protected:
        void mousePressEvent(QMouseEvent *qme);
        void mouseMoveEvent(QMouseEvent *qme);
        void mouseReleaseEvent(QMouseEvent *qme);

    signals:
        /// emitted when the user wants to detach tab
        void drag_away(int tab);

};
class extended_TabWidget : public QTabWidget
{
    Q_OBJECT

    public:
        extended_TabWidget(QWidget* parent=NULL);

    signals:
        /// emitted when the user wants to detach tab
        void drag_away(int tab);
};

#endif // EXTENTED_TABWIDGET_HPP
