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

QIcon load::icon ( QString name )
{
    if ( QIcon::hasThemeIcon(name) ) // check theme first
        return QIcon::fromTheme(name);

    QString fallbackname = "img/"+name+".svg";
    if ( QFile::exists(DATA_DIR+fallbackname) ) // check install datadir
        return QIcon(DATA_DIR+fallbackname);
    else if ( QFile::exists(fallbackname) ) // check current dir
        return QIcon(fallbackname);


    return QIcon(); // no icon found
}


QString load::resource_name ( QString base_dir, QString name )
{
    if ( QFile::exists(base_dir+name) )
        return base_dir+name;
    else
        return name;
}
