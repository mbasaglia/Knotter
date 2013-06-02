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

#include "script_window.hpp"
#include "main_window.hpp"

Script_Window::Script_Window(Main_Window *window, QObject *parent) :
    QObject(parent), window(window)
{
    connect(window,SIGNAL(tab_closing(Knot_View*)),SLOT(close_tab(Knot_View*)));
}

bool Script_Window::open(QString name)
{
    return window->create_tab(name);
}

QString Script_Window::current_file() const
{
    return window->view->windowFilePath();
}

int Script_Window::current_tab() const
{
    return window->tabWidget->currentIndex();
}

void Script_Window::set_current_tab(int t)
{
    if ( t >= 0 && t < open_tabs() )
        window->switch_to_tab(t);
}

int Script_Window::open_tabs() const
{
    return window->tabWidget->count();
}

Script_Document* Script_Window::document()
{
    if ( docs.contains(window->view) )
    {
        docs[window->view]->update();
        return docs[window->view];
    }
    else
        return docs[window->view] = new Script_Document(window->view,this);
}

QString Script_Window::toString() const
{
    return "[Knotter window]";
}

void Script_Window::close_tab(Knot_View *view)
{
    if ( docs.contains(view) )
    {
        delete docs[view];
        docs.remove(view);
    }
}

