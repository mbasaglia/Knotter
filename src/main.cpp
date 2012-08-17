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
#include <QtGui/QApplication>
#include "knot_window.hpp"
#include "error_recovery.hpp"

#include <QTranslator>
int main(int argc, char *argv[])
{
    ErrorRecovery::initialize();

    QApplication a(argc, argv);

    /*QTranslator myappTranslator;
    myappTranslator.load("knotter_it");
    a.installTranslator(&myappTranslator);*/

    Knot_Window w;
    w.show();

    
    return a.exec();
}
