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
#include "main_window.hpp"
#include "resource_manager.hpp"

Main_Window::Main_Window(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    retranslate();

    init_menus();
}

void Main_Window::retranslate()
{
    retranslateUi(this);
    setWindowTitle(Resource_Manager::program_name());
}

void Main_Window::init_menus()
{
    // Menu Edit
    /// \todo undo/redo

    // Menu View
    /// \todo docks
    QActionGroup* display_mode = new QActionGroup(this);
    display_mode->addAction(action_Normal);
    display_mode->addAction(action_Highlight_Links);

    // Menu Nodes
    QActionGroup* transform_mode = new QActionGroup(this);
    transform_mode->addAction(action_Scale);
    transform_mode->addAction(action_Rotate);

    QActionGroup* edit_mode = new QActionGroup(this);
    edit_mode->addAction(action_Edit_Graph);
    edit_mode->addAction(action_Edge_Loop);



}
