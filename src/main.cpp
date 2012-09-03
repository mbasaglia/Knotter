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
#include "resource_loader.hpp"


std::ostream& operator<< ( std::ostream&os, QString str )
{
    return os << str.toStdString();
}

#ifndef NO_BOOST
    #include <boost/program_options.hpp>
    void parse_cmd_args(int argc, char* argv[], QString& open_file)
    {
        using namespace boost::program_options;
        options_description opts("Command line arguments");
        opts.add_options()
            ("help,h", Knot_Window::tr("Show this message and exit").toStdString().c_str() )
            ("version,v", Knot_Window::tr("Show version info and exit").toStdString().c_str() )
            ("input-file", value<std::string>(), Knot_Window::tr("Knot file to be loaded").toStdString().c_str() )
        ;

        positional_options_description infile;
        infile.add("input-file",-1);

        parsed_options parsed = command_line_parser ( argc, argv ).options ( opts )
               .allow_unregistered().positional ( infile ). run();
        variables_map vm;
        store ( parsed, vm );
        notify ( vm );

        if (vm.count("help"))
        {
            std::cout << Knot_Window::tr("Usage: ") << "knotter [infile...]\n"
                      << opts << "\n";
            exit(0);
        }
        if ( vm.count("version") )
        {
            std::cout << BUILD_INFO << '\n';
            exit(0);
        }

        if ( vm.count("input-file") )
        {
            std::string sof = vm["input-file"].as<std::string>();
            open_file = QString::fromStdString(sof);
        }

    }

#else

    void parse_cmd_args(int, char*[], QString& open_file)
    {
        QStringList args = QCoreApplication::arguments();

        for(int i = 1; i < args.size(); i++ )
        {
            QString arg = args[i];

            if ( arg == "-v" || arg == "--version" )
            {
                std::cout << BUILD_INFO << '\n';
                exit(0);
            }
            else if ( arg == "-h" || arg == "--help" )
            {
                std::cout << Knot_Window::tr("Usage: ") <<
                            "knotter [infile...]\n"
                          << "-v --version\n\t" << Knot_Window::tr(
                                "Show version info and exit" ) << std::endl
                          << "-h --help\n\t" << Knot_Window::tr(
                                "Show this message and exit" ) << std::endl
                          <<  Knot_Window::tr("This build of Knotter has "
                                "been compiled without command line support.");
                exit(0);
            }
            else
            {
                if ( QFile::exists(arg) )
                    open_file = arg;
            }
        }
    }
#endif // NO_BOOST

int main(int argc, char *argv[])
{
    ErrorRecovery::initialize();

    QApplication a(argc, argv);

    load::initialize_icon_theme();

    Translator::initialize("en");
    Translator::load_system_default();

    QString open_file;
    parse_cmd_args(argc,argv,open_file);

    Knot_Window w;

    if ( !open_file.isEmpty() )
        w.open(open_file);


    w.show();
    
    return a.exec();
}
