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
#ifndef APPLICATION_INFO_HPP
#define APPLICATION_INFO_HPP

#include <QObject>

/**
 * @brief Static class which contains information about the application
 */
class Application_Info : public QObject
{
    Q_OBJECT
private:
    Application_Info(){}
    Application_Info(const Application_Info&);
    Application_Info& operator=(const Application_Info &);
    friend class Resource_Manager;

public:

    /// Get the translated program name
    QString name();

    /// Get version as string
    QString version();


    /// Get version as string (without _devel identifier)
    QString trimmed_version();

    /// Check if has at least given version number
    bool has_least_version(int maj, int min);

    /**
     * \brief Check if has at least given version number
     * \param version Acceptable input: "0.9.5_devel" and similar
     */
    bool has_least_version(QString version);


    /// Check if given version string has at least given version number
    bool check_least_version(QString version, int maj, int min);


    /**
     * \brief Get full path of given data file
     * \param name Name of the file (relative to datadir)
     * \return Full path or null string if not found
     */
    QString data(QString name);


    /**
     * \brief Get all available directories to search data from
     * \param name Name of the data directory
     */
    QStringList data_directories(QString name);


    /**
     * \brief Get all directories to search data from
     *
     * This function may include directories that don't exist but that will be
     * checked if they existed
     *
     * \param name Name of the data directory
     */
    QStringList data_directories_unckecked(QString name);


    /**
     * \brief A directory to write user preferences into
     * \param name Name of the data subdirectory
     */
    QString writable_data_directory(QString name);
};

#endif // APPLICATION_INFO_HPP
