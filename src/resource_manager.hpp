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
#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <QObject>
#include <QDebug>
#include "settings.hpp"

/**
 * Manage resources and data
 */
class Resource_Manager : public QObject
{
    Q_OBJECT

    Resource_Manager(){}
    Resource_Manager(const Resource_Manager&);
    Resource_Manager& operator= (const Resource_Manager&);
    ~Resource_Manager();

    static Resource_Manager singleton;

public:
    static Settings settings;

    /// Initialize the resource system
    static void initialize();

    /// Get the translated program name
    static QString program_name();

    /// Get version as string
    static QString program_version();

    /// Check if has at least given version number
    static bool has_least_version(int maj, int min);


    /// Check if given version string has at least given version number
    static bool check_least_version(QString version, int maj, int min);


    /** Get full path of given data file
     * \param name Name of the file (relative to datadir)
     * \return Full path or null string if not found
     */
    static QString data(QString name);
    
};

#endif // RESOURCE_MANAGER_HPP
