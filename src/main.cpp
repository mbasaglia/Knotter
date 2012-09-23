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
#include <QImageWriter>
#include <algorithm>

std::ostream& operator<< ( std::ostream&os, QString str )
{
    return os << str.toStdString();
}

#ifndef NO_BOOST
    #include <boost/program_options.hpp>
    void parse_cmd_args(int argc, char* argv[], KnotGraph& graph)
    {

        using namespace boost::program_options;

        options_description info_opt("Information");
        info_opt.add_options()
            ("help,h", Knot_Window::tr("Show this message and exit").toStdString().c_str() )
            ("version,v", Knot_Window::tr("Show version info and exit").toStdString().c_str() )
            ("list-formats", Knot_Window::tr("Display a list of available export formats and exit").toStdString().c_str() )
            ;

        options_description file_opt("Load and export");
        file_opt.add_options()
            ("output,o",value<std::string>(),Knot_Window::tr("Export last loaded knot file").toStdString().c_str() )
            ("format,f",value<std::string>(),Knot_Window::tr("Export format").toStdString().c_str() )
            ("input-file", value<std::string>(), Knot_Window::tr("Knot file to be loaded").toStdString().c_str() )
            ("antialias", Knot_Window::tr("Enable supersampling for raster export").toStdString().c_str() )
            ("no-antialias", Knot_Window::tr("Disable supersampling for raster export").toStdString().c_str() )
            ("minimal", Knot_Window::tr("Export only path to SVG").toStdString().c_str() )
            ("no-minimal", Knot_Window::tr("Export full shape to SVG").toStdString().c_str() )
            ("background,b", value<std::string>(), Knot_Window::tr("Name for the background color for raster export").toStdString().c_str() )
            ;


        options_description misc_opt("Other");
        misc_opt.add_options()
            ("no-gui", Knot_Window::tr("Don't start the gui after parsing the command line").toStdString().c_str() )
            ;

        options_description all_opt("Command line arguments");
        all_opt.add(info_opt).add(file_opt).add(misc_opt);

        positional_options_description infile;
        infile.add("input-file",-1);

        parsed_options parsed = command_line_parser ( argc, argv ).options ( all_opt )
               .allow_unregistered().positional ( infile ). run();

        bool no_gui = false;
        bool antialias = true;
        bool minimal = false;
        QColor background;
        QString format = "auto";
        QStringList formats;
        formats << "svg";
        foreach(QByteArray ba, QImageWriter::supportedImageFormats())
        {
            QString f = ba.toLower();
            if ( !formats.contains(f) )
                formats << f;
        }

        foreach(option opt, parsed.options)
        {
            const std::string& key = opt.string_key;
            if ( key == "no-gui" )
                no_gui = true;
            else if ( key == "help" )
            {
                std::cout << Knot_Window::tr("Usage: ") << "knotter [option...] [infile]\n"
                          << all_opt << "\n";
                exit(0);
            }
            else if ( key == "version" )
            {
                std::cout << BUILD_INFO << '\n';
                exit(0);
            }
            else if ( key == "list-formats" )
            {
                std::cout << "auto";
                foreach(QString f, formats)
                    std::cout << f << std::endl;
                exit(0);
            }
            else if ( key == "antialias" )
                antialias = true;
            else if ( key == "no-antialias" )
                antialias = false;
            else if ( key == "minimal" )
                minimal = true;
            else if ( key == "no-minimal" )
                minimal = false;
            else
            {
                QString value;
                if ( !opt.value.empty() )
                    value = QString::fromStdString(opt.value.front());

                if ( key == "format" )
                {
                    if ( value != "auto" && !formats.contains(value) )
                        std::cerr << Knot_Window::tr("Unsupported format: %1").arg(value) << std::endl;
                    else
                        format = value;
                }
                else if ( key == "input-file" )
                {
                    QFile open_file(value);
                    value = value.toLower();
                    if ( !open_file.open(QIODevice::ReadOnly | QIODevice::Text) ||
                         !graph.load_xml(&open_file) )
                         std::cerr << Knot_Window::tr("Error while reading \"%1\".").arg(value) << std::endl;
                    else
                        graph.build();
                }
                else if ( key == "output" )
                {
                    QString tformat = format;
                    QIODevice::OpenMode flags = QIODevice::WriteOnly;
                    if ( format == "auto" )
                    {
                        tformat = value.mid(value.lastIndexOf('.')+1).toLower();
                        if ( !formats.contains(tformat) )
                        {
                            std::cerr << Knot_Window::tr("Failed to detect format for %1").arg(value) << std::endl;
                            std::cerr << Knot_Window::tr("Skipping export to %1").arg(value) << std::endl;
                            continue;
                        }
                    }
                    if ( tformat == "svg" )
                        flags |= QIODevice::Text;

                    QFile open_file(value);
                    if ( !open_file.open( flags ) )
                    {
                        std::cerr << Knot_Window::tr("Could not write to \"%1\".").arg(value) << std::endl;
                        continue;
                    }

                    if ( tformat == "svg" )
                        graph.export_svg(open_file,minimal);
                    else
                    {
                        /// \todo size argument
                        graph.export_raster(open_file,false,background,
                            antialias,graph.boundingRect().size().toSize(),100);
                    }
                } // output
                else if ( key == "background" )
                {
                    if ( value.startsWith("rgb") )
                    {
                        std::stringstream ss ( value.toStdString() );
                        ss.ignore(3);
                        bool alpha = 0;
                        int r=255, g=255, b=255, a=255;
                        char c;
                        if ( ss.get() == 'a' )
                            alpha = true;
                        else
                            ss.unget();

                        if (
                                (ss >> c) && c == '(' &&
                                (ss>>r) && (ss >> c) && c == ',' &&
                                (ss>>g) && (ss >> c) && c == ',' &&
                                (ss>>b) &&
                                ( !alpha || ( (ss >> c) && c == ',' && (ss>>a) ) ) &&
                                (ss>>c) && c == ')'
                            )
                            background = QColor(r,g,b,a);
                        else
                            std::cerr << Knot_Window::tr("Bad color format: %1").arg(value);
                    }
                    else
                        background = QColor(value);
                }
            }
        }

        if ( no_gui )
            exit(0);

    }

#else

    void parse_cmd_args(int, char*[], KnotGraph& graph)
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
                            "knotter [infile]\n"
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
                QFile open_file(arg);
                if ( !open_file.isOpen() || !graph.load_xml(&open_file) )
                     std::cerr << Knot_Window::tr("Error while reading \"%1\".").arg(arg) << std::endl;
            }
        }
    }
#endif // NO_BOOST

int main(int argc, char *argv[])
{
// initialize
    ErrorRecovery::initialize();

    knot_curve_styler::register_style(new knot_curve_pointed, "pointed" );
    knot_curve_styler::register_style(new knot_curve_ogee, "ogee" );
    knot_curve_styler::register_style(new knot_curve_polygonal, "polygonal" );
    knot_curve_styler::register_style(new knot_curve_round, "round" );

    QApplication a(argc, argv);

    Translator::initialize("en");
    Translator::load_system_default();

    Transform_Handle::scale_rest.load(load::resource_name(DATA_DIR,"img/handle_scale_rest.svg"));
    Transform_Handle::scale_active.load(load::resource_name(DATA_DIR,"img/handle_scale_active.svg"));
    Transform_Handle::rotate_rest.load(load::resource_name(DATA_DIR,"img/handle_rotate_rest.svg"));
    Transform_Handle::rotate_active.load(load::resource_name(DATA_DIR,"img/handle_rotate_active.svg"));

    KnotGraph graph;
    parse_cmd_args(argc,argv,graph);

    load::initialize_icon_theme();

// run
    Knot_Window w(&graph);

    w.show();
    
    int status = a.exec();

// finalize
    knot_curve_styler::clear();

    return status;
}
