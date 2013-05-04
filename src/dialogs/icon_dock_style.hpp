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

#ifndef ICON_DOCK_STYLE_HPP
#define ICON_DOCK_STYLE_HPP

#include <QProxyStyle>
#include <QWidget>
#include "c++.hpp"
#include <QStyleOption>
#include <QPainter>

#include <QDockWidget>
#include <QDebug>

class Icon_Dock_Style: public QProxyStyle
{
    Q_OBJECT

    //QDockWidget* target;
public:
    Icon_Dock_Style(QDockWidget* target)
        : QProxyStyle(target->style()){}



    void drawControl(ControlElement element, const QStyleOption* option,
        QPainter* painter, const QWidget* widget = 0) const override
    {
        if( element == QStyle::CE_DockWidgetTitle
            //element == QStyle::CE_TabBarTabLabel
            )
        {
            int width = pixelMetric(QStyle::PM_SmallIconSize);

            int margin = baseStyle()->pixelMetric(QStyle::PM_DockWidgetTitleMargin);

            QPoint icon_point(margin + option->rect.left(),
                              margin + option->rect.center().y()-width/2);

            painter->drawPixmap(icon_point, widget->windowIcon().pixmap(width, width));

            const_cast<QStyleOption*>(option)->rect = option->rect.adjusted(width+margin, 0, 0, 0);
        }

        baseStyle()->drawControl(element, option, painter, widget);
    }
};

#endif // ICON_DOCK_STYLE_HPP
