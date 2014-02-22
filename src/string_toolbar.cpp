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
#include "string_toolbar.hpp"
#include <QMenu>
#include <QMainWindow>

String_Toolbar::String_Toolbar(QToolBar *tb)
    : name(tb->objectName()),title(tb->windowTitle())
{
    foreach(QAction* a, tb->actions())
        actions.push_back(a->objectName());
}

QToolBar *String_Toolbar::create(QMainWindow *w) const
{
    QToolBar *toolbar = new QToolBar(title,w);
    toolbar->setObjectName(name);

    w->addToolBar(Qt::TopToolBarArea,toolbar);

    foreach(const QString&item, actions)
    {
        if ( item.isEmpty() )
            toolbar->addSeparator();
        else
        {
            QAction* act = w->findChild<QAction*>(item);

            if ( !act )
            {
                QMenu *menu = w->findChild<QMenu*>(item);
                if ( menu )
                    act = menu->menuAction();
            }

            if ( act )
                toolbar->addAction ( act );
            else
                qWarning() << QObject::tr("Warning:")
                          << QObject::tr("Unknown action %1").arg(item);
        }
    }

    return toolbar;
}


