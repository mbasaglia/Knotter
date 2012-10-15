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
#include "resource_loader.hpp"
#include <QFile>
#include <QCoreApplication>
#include <iostream>
#include <QDir>

/**
    \brief Resource loading

    The functions enclosed in this namespace locate resources based on build configuration
*/
namespace load {

/**
    \brief Conditionally load Tango icons

    If Tango is disabled, this function does nothing.

    If Tango is used as fall back theme, the system theme is queried with the
    "document-new" icon, if that is not present, Tango is loaded.

    If Tango is used as default theme, it's loaded directly.
*/
void initialize_icon_theme()
{
  #ifdef TANGO_FALLBACK
    if ( !QIcon::hasThemeIcon("document-new") )
    {
        QIcon::setThemeSearchPaths( QIcon::themeSearchPaths()
                                    << resource_url(DATA_DIR,"themes").toString() );
        QIcon::setThemeName("tango-icons");
    }
  #elif defined(TANGO_DEFAULT)
        QIcon::setThemeSearchPaths( QStringList()
                                    << resource_name(DATA_DIR,"themes") );
        QIcon::setThemeName("tango-icons");
  #endif
}

/**
    \brief return an icon from given name

    If the current theme has an icon corresponding to the given name, that it selected
    otherwise it loads DATA_DIR/img/name.svg
*/
QIcon icon ( QString name )
{
    if ( QIcon::hasThemeIcon(name) ) // check theme first
        return QIcon::fromTheme(name);

    return QIcon( resource_name( DATA_DIR, "img/"+name+".svg") );
}

/**
    \brief get full path to resource given it's name

    If base_dir is a QRC resource path, it's used as file path.

    If base_dir is a directory, "name" is searched there.
    If it's not found there, "name" is searched in the current directory.

    If the file cannot be located, an empty string is returned and a message is
    written to stderr.

    \param base_dir  exprected installation directory
    \param name      resource file name relative to base_dir
    \return A normalized path pointing to "name"
*/
QString resource_name ( QString base_dir, QString name )
{
    if ( base_dir.startsWith(":/") ) // resource
        return base_dir+'/'+name;

    QDir path ( base_dir ); // install dir

    if ( !path.exists(name) )
        path.setPath( QCoreApplication::applicationDirPath() ); // executable dir

    if ( !path.exists(name) )
        path.setPath( QDir::currentPath() ); // current dir


    if ( !path.exists(name) )
    {
        std::cerr << QObject::tr("Could not locate resource %1").arg(name).toStdString() << std::endl;
        return QString(); // not found
    }

    return QDir::cleanPath(path.absoluteFilePath(name));
}

/**
    \brief Get a QUrl object for a resource
*/
QUrl resource_url ( QString base_dir, QString name )
{
    if ( base_dir.startsWith(":/") ) // resource
        return "qrc"+base_dir+'/'+name;
    else
        return "file://"+resource_name(base_dir,name);
}


} // namespace load
