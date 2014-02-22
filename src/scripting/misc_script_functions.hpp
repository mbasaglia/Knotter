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

#ifndef MISC_SCRIPT_FUNCTIONS_HPP
#define MISC_SCRIPT_FUNCTIONS_HPP
#include <QScriptContext>
#include <QStringList>

/**
 * \brief Override qt script print function
 *
 * Writes any argument to stdout and emits Resource_Manager::sctipt_output
 */
QScriptValue script_print(QScriptContext * context, QScriptEngine * engine );

/**
 * \brief Run an external script file
 *
 * Takes the file name as parameter, will raise an exception if that is not found
 */
QScriptValue script_run_script(QScriptContext * context, QScriptEngine * engine );


/**
 * \brief Information on the program accessible from the scripts
 */
class Script_Knotter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString version READ version)
    Q_PROPERTY(QStringList edge_types READ edge_types)
    Q_PROPERTY(QStringList cusp_shapes READ cusp_shapes)

public:

    QString version() const;

    /// Check if Knotter has at leaset specified version
    Q_INVOKABLE bool has_version(int maj, int min);

    Q_INVOKABLE QString toString() const;

    QStringList edge_types();
    QStringList cusp_shapes();
};

/**
 *  \brief System stuff
 */
class Script_System : public QObject
{
    Q_OBJECT

public:
    /**
     * \brief Read data from a file
     * \param file_name Name of the file
     * \return The file data
     */
    Q_INVOKABLE QByteArray read_file( QString file_name );
    /**
     * \brief Write data to a  binary file
     * \param file_name Name of the file
     * \param data Data to write into the file
     * \return Whether the operation was successful
     */
    Q_INVOKABLE bool write_file( QString file_name, QByteArray data );
    /**
     * \brief Write data to a text file
     * \param file_name Name of the file
     * \param data Data to write into the file
     * \return Whether the operation was successful
     */
    Q_INVOKABLE bool write_file( QString file_name, QString data );

    Q_INVOKABLE QString toString() const;

    /**
     * \brief Get the system temporary directory path
     */
    Q_INVOKABLE QString temp_path();

    /**
     * \brief Check if a file with the given name exists
     * \param file_name Path to the file
     * \param readable  If the file has to be readable
     * \param writable  If the file has to be writable
     */
    Q_INVOKABLE bool file_exists( QString file_name, bool readable = false, bool writable = false );


    /**
     * \brief Get a unique temporary file name
     * \param base_name Base file name
     * \param extension File extension
     * \return \c base_name \i (n) \c . \c extension
     */
    Q_INVOKABLE QString unique_temp_file(QString base_name,QString extension);

    /**
     * \brief Run an external program
     * \param command  The name of the program
     * \param params   The command line arguments
     */
    Q_INVOKABLE bool exec(QString command, QStringList params);
};

#endif // MISC_SCRIPT_FUNCTIONS_HPP
