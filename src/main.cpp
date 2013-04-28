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

/*#include <QSvgGenerator>
#include "graph.hpp"*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Resource_Manager::register_edge_style(new Edge_Normal,QObject::tr("Regular"),"regular");
    Resource_Manager::register_edge_style(new Edge_Inverted,QObject::tr("Inverted"),"inverted");
    Resource_Manager::register_edge_style(new Edge_Hole,QObject::tr("Hole"),"hole");
    Resource_Manager::register_edge_style(new Edge_Wall,QObject::tr("Wall"),"wall");
    Resource_Manager::initialize();

    Main_Window mw;
    mw.show();

    /*Graph g;
    Node * last = 0;
    for ( int i = 0; i < 10; i++)
    {
        Node* nn = new Node(QPointF(i*16,(i%2)*64),&g);
        g.add_node(nn);
        if ( last != 0 )
            g.add_edge(new Edge(nn,last,new Edge_Normal,&g));
        last = nn;
    }
    QSvgGenerator generator;
    generator.setFileName("../../misc/foo.svg");
    QPainter painter;
    painter.begin(&generator);
    g.paint(&painter);
    painter.end();*/


    return a.exec();
}
