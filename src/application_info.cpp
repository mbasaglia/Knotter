/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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
#include "application_info.hpp"

#include <QCoreApplication>
#include <QDir>

#if HAS_QT_5
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif

QString Application_Info::name()
{
    return tr("Knotter");
}

QString Application_Info::version()
{
    return QString(VERSION);
}

QString Application_Info::trimmed_version()
{
    static const QRegExp version_re("^[0-9]+\\.[0-9]+\\.[0-9]+");
    QString version_str = version();
    version_re.indexIn(version_str);
    return version_str.left(version_re.matchedLength());
}

bool Application_Info::has_least_version(int maj, int min)
{
    return check_least_version ( version(), maj, min );
}

bool Application_Info::has_least_version(QString checkversion)
{
    QStringList v_in = checkversion.split(".");
    QStringList v_k = version().split(".");
    if ( v_in.size() < 2 )
        return false; // weird format
    for ( int i = 0; i < v_in.size(); i++ )
    {
        if ( i >= v_k.size() )
            return false; // input version has one extra sub-number
        int vk = v_k[i].remove(QRegExp("[^0-9]")).toInt();
        int vin = v_in[i].remove(QRegExp("[^0-9]")).toInt();
        if ( vk < vin )
            return false; // input version is greater
        else if ( vk > vin )
            return true; // local version is greater
    }
    return true; // versions are the same
}

bool Application_Info::check_least_version(QString version, int maj, int min)
{
    QStringList v = version.split(".");
    //int i = v[0].toInt(), j = v[1].toInt();
    if ( v.size() < 2)
        return false;
    else
        return v[0].toInt() >= maj&& v[1].toInt() >= min;
}

QString Application_Info::data(QString name)
{
    QDir path ( DATA_DIR ); // install dir

    if ( !path.exists(name) )
        path.setPath( QCoreApplication::applicationDirPath()+"/data" ); // executable dir

    if ( !path.exists(name) )
        path.setPath( QDir::currentPath()+"/data" ); // current dir


#if HAS_QT_5
    if ( !path.exists(name) )
    {
        foreach ( QString d, QStandardPaths::standardLocations(QStandardPaths::DataLocation)  )
        {
            if ( path.exists(name) )
                break;
            path.setPath(d);
        }
    }
#else
    if ( !path.exists(name) )
        path.setPath( QDesktopServices::storageLocation(QDesktopServices::DataLocation) );
#endif


    if ( !path.exists(name) )
        return QString(); // not found

    return QDir::cleanPath(path.absoluteFilePath(name));
}

QStringList Application_Info::data_directories(QString name)
{
    QStringList found;

    QStringList search = data_directories_unckecked(name);

    foreach ( QString d, search )
    {
        if ( QFileInfo(d).exists() )
            found << d;
    }
    found.removeDuplicates();

    return found;
}

QStringList Application_Info::data_directories_unckecked(QString name)
{
    QStringList search;
    search << DATA_DIR;  // install dir
#if HAS_QT_5
    search << QStandardPaths::standardLocations(QStandardPaths::DataLocation);
#else
    search << QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif
    search << QDir(QCoreApplication::applicationDirPath()).filePath("data") ; // executable dir
    search << QDir::current().filePath("data"); // current dir

    QStringList filter;
    foreach ( QString d, search )
    {
        filter << QDir::cleanPath(QDir(d).absoluteFilePath(name));
    }
    filter.removeDuplicates();

    return filter;
}

QString Application_Info::writable_data_directory(QString name)
{
    QStringList search;

#if HAS_QT_5
    search << QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
    search << QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif

    if ( !search.empty() )
    {
        return QDir::cleanPath(QDir(search[0]).absoluteFilePath(name));
    }

    return QString();
}
