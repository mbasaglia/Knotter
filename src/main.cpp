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
#include <iostream>
#include <QSvgGenerator>
#include <QFile>
#include "translator.hpp"


std::ostream& operator<< ( std::ostream&os, QString str )
{
    return os << str.toStdString();
}

int main(int argc, char *argv[])
{
    ErrorRecovery::initialize();

    QApplication a(argc, argv);

    Translator::initialize("en");

    Knot_Window w;


    QStringList args = QCoreApplication::arguments();
    bool nogui = false;
    for(int i = 1; i < args.size(); i++ )
    {
        QString arg = args[i];
        QString param = i < args.size()-1 ? args[i+1] : "";

        if ( arg == "-v" || arg == "--version" )
        {
            std::cout << BUILD_INFO << '\n';
            return 0;
        }
        else if ( arg == "--svg" && !param.isEmpty() )
        {
            i++; // discard param

            QFile quf(param);
            if ( ! quf.open(QIODevice::WriteOnly | QIODevice::Text) )
            {
                std::cerr << Knot_Window::tr("Could not write to \"%1\".").arg(param) << '\n';
                continue;
            }

            QSvgGenerator gen;
            gen.setOutputDevice(&quf);

            w.knotview().paint_knot ( &gen, QRectF(), false );

            quf.close();
        }
        else if ( arg == "--nogui" )
            nogui = true;
        else if ( arg == "-h" || arg == "--help" )
        {
            std::cout << Knot_Window::tr("Usage: ") <<
                        "knotter [--nogui] [infile [--svg outfile]...]\n"
                      << "--nogui\n\t" << Knot_Window::tr(
                            "Tell knotter to terminate when finished parsing the"
                            " command-line arguments" ) << std::endl
                      << "--svg outfile\n\t" << Knot_Window::tr(
                            "Convert currently loaded file into SVG" ) << std::endl
                      << "-v --version\n\t" << Knot_Window::tr(
                            "Show version info and exit" ) << std::endl
                      << "-h --help\n\t" << Knot_Window::tr(
                            "Show this message and exit" ) << std::endl;
            return 0;
        }
        else
        {
            if ( ! w.open(arg,true) )
                std::cerr << Knot_Window::tr("Error while reading \"%1\".").arg(arg) << '\n';
        }
    }
    if ( nogui )
        return 0;

    w.show();
    
    return a.exec();
}
