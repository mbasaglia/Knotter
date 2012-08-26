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

namespace load {


QIcon icon ( QString name )
{
    if ( QIcon::hasThemeIcon(name) ) // check theme first
        return QIcon::fromTheme(name);

    return QIcon( resource_name( DATA_DIR, "img/"+name+".svg") );
}


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


QUrl resource_url ( QString base_dir, QString name )
{
    if ( base_dir.startsWith(":/") ) // resource
        return "qrc"+base_dir+'/'+name;
    else
        return "file://"+resource_name(base_dir,name);
}


} // namespace load
