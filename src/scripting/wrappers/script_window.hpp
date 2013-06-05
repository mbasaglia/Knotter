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

#ifndef SCRIPT_WINDOW_HPP
#define SCRIPT_WINDOW_HPP

#include <QObject>
#include "script_document.hpp"

class Main_Window;

/**
 *  Creates and displays simple dialogs
 */
class Script_Window_Dialog : public QObject
{
    Q_OBJECT

    QWidget* parent_widget; ///< Widget that wll be used as dialog parent

    QString default_title(QString title);
public:
    explicit Script_Window_Dialog(QWidget* parent_widget = nullptr)
        : parent_widget(parent_widget) {}

    Q_INVOKABLE void information(QString message, QString title = QString() );
    Q_INVOKABLE void warning(QString message, QString title = QString() );
    Q_INVOKABLE void critical(QString message, QString title = QString() );
    Q_INVOKABLE int question(QString message, QString title = QString(),
                             QString button1 = QString(), QString button2 = QString(),
                             QString button3 = QString());

    Q_INVOKABLE QString get_open_file(QString title = QString(), QString filters = QString() );

    Q_INVOKABLE  double get_number(QString message, QString title=QString(),
                                   double default_value = 0,
                                   double min = -2147483647,
                                   double max =  2147483647 );

    Q_INVOKABLE  double get_integer(QString message, QString title=QString(),
                                   int default_value = 0,
                                   int min = -2147483647,
                                   int max =  2147483647 );

    Q_INVOKABLE QString get_text(QString message, QString title=QString(),
                                 QString default_value=QString() );

    Q_INVOKABLE QString get_item(QString message, QString title=QString(),
                                 QStringList items=QStringList() );


    Q_INVOKABLE QString toString() const;


    /**
     * \brief Loads a widget from a ui file
     */
    Q_INVOKABLE QWidget* load_widget(QString ui_file_name);
};

/// Wrapper to Main_Window
class Script_Window : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString current_file READ current_file)
    Q_PROPERTY(int current_tab READ current_tab WRITE set_current_tab)
    Q_PROPERTY(int open_tabs READ open_tabs)
    Q_PROPERTY(QObject* document READ document)
    Q_PROPERTY(QObject* dialog READ dialog)

    Main_Window* window;
    QMap<Knot_View*,Script_Document*> docs;
    Script_Window_Dialog m_dialog;
public:
    explicit Script_Window(Main_Window* window, QObject *parent = 0);

    Q_INVOKABLE bool open(QString name=QString());
    QString current_file() const;
    int current_tab() const;
    void set_current_tab(int t);
    int open_tabs() const;

    Script_Document* document();

    Script_Window_Dialog* dialog();

    Q_INVOKABLE QString toString() const;

    /**
     * \brief Take a screenshot of a widget
     * \param output_image_file name of the file to save the screenshot
     * \param widget name of the child widget. If empty the whole window
     */
    Q_INVOKABLE void screenshot(QString output_image_file, QString widget="");

    void clean_up();

private slots:
    void close_tab(Knot_View* v);
};

#endif // SCRIPT_WINDOW_HPP
