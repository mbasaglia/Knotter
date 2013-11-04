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

#ifndef QT_GUI_SCRIPT_HPP
#define QT_GUI_SCRIPT_HPP

#include <QObject>
#include <QStringList>

/**
 * \brief Allow interactions between Script and standard UI classes
 *
 * This allows to access stuff that is available from C++ classes but is not provided
 * as invokable methods or properties
 */
class Qt_GUI_Script : public QObject
{
    Q_OBJECT
public:
    explicit Qt_GUI_Script(QObject *parent = 0);

    // QTableWidget
    /**
     * \brief Set the value of a cell in a QTableWidget
     * \param table     The QTableWidget object
     * \param row       Cell row
     * \param column    Cell column
     * \param value     String to write
     */
    Q_INVOKABLE void table_set(QObject* table_object, int row, int column, QString value);
    /**
     * \brief Get the value of a cell in a QTableWidget
     * \param table     The QTableWidget object
     * \param row       Cell row
     * \param column    Cell column
     */
    Q_INVOKABLE QString table_get(QObject* table_object, int row, int column );
    /**
     * \brief Add a row at the end of a QTableWidget with the given items
     * \param table     The QTableWidget object
     * \param value     Strings to write
     */
    Q_INVOKABLE void table_append_row(QObject* table_object, QStringList value);
    
signals:
    
public slots:
    
};

#endif // QT_GUI_SCRIPT_HPP
