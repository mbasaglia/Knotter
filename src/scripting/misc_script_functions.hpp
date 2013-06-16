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

#ifndef MISC_SCRIPT_FUNCTIONS_HPP
#define MISC_SCRIPT_FUNCTIONS_HPP
#include <QScriptContext>
#include <QStringList>

/**
 * @brief Override qt script print function
 *
 * Writes any argument to stdout and emits Resource_Manager::sctipt_output
 */
QScriptValue script_print(QScriptContext * context, QScriptEngine * engine );

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

#endif // MISC_SCRIPT_FUNCTIONS_HPP
