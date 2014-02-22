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

#ifndef SCRIPT_QTABLEWIDGET_HPP
#define SCRIPT_QTABLEWIDGET_HPP

#include <QTableWidget>
#include <QScriptEngine>

class Script_QTableWidget : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject* table READ wrapped_table)

private:
    QTableWidget * table;

public:
    explicit Script_QTableWidget(QTableWidget * table, QObject *parent = 0);

    /**
     * \brief Set the value of a cell in a QTableWidget
     * \param row       Cell row
     * \param column    Cell column
     * \param value     String to write
     */
    Q_INVOKABLE void set_value(int row, int column, QString value);
    /**
     * \brief Get the value of a cell in a QTableWidget
     * \param row       Cell row
     * \param column    Cell column
     */
    Q_INVOKABLE QString get_value ( int row, int column );
    /**
     * \brief Add a row at the end of a QTableWidget with the given items
     * \param value     Strings to write
     */
    Q_INVOKABLE void append_row( QStringList value);
    /**
     * \brief Get the currently selected row in a QTableWidget
     * \return The index of the selected row or -1
     */
    Q_INVOKABLE int current_row();
    /**
     * \brief Get the currently selected column in a QTableWidget
     * \return The index of the selected column or -1
     */
    Q_INVOKABLE int current_column();

    QTableWidget * wrapped_table() const { return table; }

signals:
    /**
     * \brief Emitted when the user edits the text of a cell
     * \param row       Row of the changed cell
     * \param column    Column of the changed cell
     * \param value     New text in that cell
     */
    void value_changed(int row, int column, QString value);

private slots:
    void emit_value_changed(QTableWidgetItem* it);
    
};

QScriptValue script_create_tablewidget_wrapper (QScriptContext *context, QScriptEngine *engine);

#endif // SCRIPT_QTABLEWIDGET_HPP
