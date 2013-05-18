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
#include <QApplication>
#include "main_window.hpp"
#include "resource_manager.hpp"
#include "edge_style.hpp"
#include "command_line.hpp"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Resource_Manager::register_cusp_shape(new Cusp_Pointed);
    Resource_Manager::register_cusp_shape(new Cusp_Rounded);
    Resource_Manager::register_cusp_shape(new Cusp_Polygonal);
    Resource_Manager::register_cusp_shape(new Cusp_Ogee);

    Resource_Manager::register_edge_style(new Edge_Normal);
    Resource_Manager::register_edge_style(new Edge_Inverted);
    Resource_Manager::register_edge_style(new Edge_Wall);
    Resource_Manager::register_edge_style(new Edge_Hole);
    Resource_Manager::initialize();

    Command_Line cmd(argc, argv);

    if ( !cmd.load_ui() )
        return 0;


    Main_Window mw;
    mw.show();

    foreach(QString s, cmd.files())
        mw.create_tab(s);

    return a.exec();
}
