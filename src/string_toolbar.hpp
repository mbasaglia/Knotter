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
#ifndef STRING_TOOLBAR_HPP
#define STRING_TOOLBAR_HPP
#include <QString>
#include <QToolBar>
/**
 *  This class holds a string definition of toolbars and actions
 *
 *  Separators are stored as empty strings
 */
struct String_Toolbar
{
    QString name;
    QString title;
    QStringList actions;

    String_Toolbar() {}
    String_Toolbar( QString name, QString title, QStringList actions):
        name(name), title(title), actions(actions)
    {}
    String_Toolbar(QToolBar* tb);
    /// Create toolbar and add to widndow
    QToolBar *create(QMainWindow* w) const;
};

#endif // STRING_TOOLBAR_HPP
