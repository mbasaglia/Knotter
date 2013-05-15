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

#include "command_line.hpp"
#include "graph.hpp"
#include "image_exporter.hpp"
#include "xml_loader.hpp"
#include <QFile>
#include <iostream>
#include "resource_manager.hpp"

Command_Line::Command_Line(int argc, char *argv[])
    : ui(true), antialias(false), include_graph(false)
{
    for ( int i = 1; i < argc; i++ )
    {
        QString arg = QString::fromLocal8Bit(argv[i]);

        if ( arg == "-h" || arg == "--help" )
        {
            ui = false;
            help();
            break;
        }
        else if ( arg == "-v" || arg == "--version" )
        {
            ui = false;
            version();
            break;
        }
        else if ( arg == "-l" || arg == "--license" )
        {
            ui = false;
            license();
            break;
        }
        else if ( arg == "-b" || arg == "--no-gui")
        {
            ui = false;
        }
        else if ( arg == "-a" || arg == "--antialias")
        {
            antialias = true;
        }
        else if ( arg == "-na" || arg == "--no-antialias")
        {
            antialias = false;
        }
        else if ( arg == "-g" || arg == "--graph" )
        {
            include_graph = true;
        }
        else if ( arg == "-ng" || arg == "--no-graph" )
        {
            include_graph = false;
        }
        else if ( arg == "-o" || arg == "--output")
        {
            if ( i >= argc-1 )
                qWarning() << QObject::tr("Warning:") <<
                              QObject::tr("Missing file for argument %1").arg(arg);
            else if ( m_files.isEmpty() )
                qWarning() << QObject::tr("Warning:") <<
                               QObject::tr("No input file specified for option %1").arg(arg);
            else
            {
                i++;
                QFile infile ( m_files.back() );
                QFile outfile ( QString::fromLocal8Bit(argv[i]) );
                Graph g;

                if ( import_xml(infile,g) )
                {
                    if ( outfile.fileName().endsWith(".svg") )
                        export_svg(outfile,g,include_graph);
                    else if ( outfile.fileName().endsWith(".png") )
                        export_raster(outfile,g,Qt::transparent,antialias,
                                      g.boundingRect().size().toSize(),100,
                                      include_graph);
                    else
                        export_raster(outfile,g,Qt::white,antialias,
                                      g.boundingRect().size().toSize(),100,
                                      include_graph);
                }

            }
        }
        else
        {
            m_files.push_back(arg);
        }
    }
}

void Command_Line::license() const
{
    std::cout << "GNU General Public License version 3 or any later version." << std::endl;
}

void Command_Line::help() const
{
    version();
    std::cout << "Usage:\n"
              << "knotter [args file [-o output] ...] [qt-options ...]\n"
              << "knotter -(h|v|l)\n"

              << "\n"
              << "\tInformation:\n"
              << "-h, --help\n"
              << "\tPrint help and exit\n"
              << "-v, --version\n"
              << "\tDisplay version information and exit\n"
              << "-l, --license\n"
              << "\tDisplay license and exit\n"

              << "\n"
              << "Output:\n"
              << "-o file, --output file\n"
              << "\tExport last loaded file. Format is deducend from the extension.\n"
              << "-a, --antialias\n"
              << "\tEnable antialiasing for the following exports."
              << "-na, --no-antialias\n"
              << "\tDisable antialiasing for the following exports."
              << "-g, --graph\n"
              << "\tEnable graph output for the following exports."
              << "-ng, --no-graph\n"
              << "\tDisable graph output for the following exports."

              << "\n"
              << "Misc:\n"
              << "-b, --no-gui\n"
              << "\tDon't start the gui after parsing the command line.\n"
              << std::endl;



}

void Command_Line::version() const
{
    std::cout << Resource_Manager::program_name().toStdString() << " " <<
                 Resource_Manager::program_version().toStdString() << std::endl;
}
