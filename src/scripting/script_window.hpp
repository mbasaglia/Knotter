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

#ifndef SCRIPT_WINDOW_HPP
#define SCRIPT_WINDOW_HPP

#include <QObject>
#include "script_document.hpp"

class Main_Window;

/// Wrapper to Main_Window
class Script_Window : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString current_file READ current_file)
    Q_PROPERTY(int current_tab READ current_tab WRITE set_current_tab)
    Q_PROPERTY(int open_tabs READ open_tabs)
    Q_PROPERTY(QObject* document READ document)

    Main_Window* window;
    QMap<Knot_View*,Script_Document*> docs;

public:
    explicit Script_Window(Main_Window* window, QObject *parent = 0);

    Q_INVOKABLE bool open(QString name=QString());
    QString current_file() const;
    int current_tab() const;
    void set_current_tab(int t);
    int open_tabs() const;

    Script_Document* document();

    Q_INVOKABLE QString toString() const;

private slots:
    void close_tab(Knot_View* v);
};

#endif // SCRIPT_WINDOW_HPP
