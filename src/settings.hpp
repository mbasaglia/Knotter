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
#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <QObject>
#include <QMainWindow>
#include "string_toolbar.hpp"
#include <QMetaType>

class Settings : public QObject
{
    Q_OBJECT
    Q_ENUMS(Qt::ToolButtonStyle)

public:
    //typedef Qt::ToolButtonStyle ToolButtonStyle;

private:
    QString                 config_version; ///< Version of the setting file

    int                     m_icon_size;
    // language
    // performace
    // recent files
    // style
    // grid

    QList<String_Toolbar>   toolbars;
    bool                    save_ui; ///< Whether the GUI state must be loaded between sessions
    QByteArray              window_state;
    QByteArray              window_geometry;
    Qt::ToolButtonStyle     tool_button_style;

private:
    Settings();
    friend class Resource_Manager;

public:

    void load_config();
    void save_config();

    void initialize_window(QMainWindow*);
    void save_window(QMainWindow*);

    QString version() const;
    bool least_version(int maj,int min) const;
    bool current_version() const;

    void icon_size(int sz);
    int icon_size() const { return m_icon_size; }

    Qt::ToolButtonStyle button_style() const { return tool_button_style; }
    void button_style(Qt::ToolButtonStyle);
signals:
    void icon_size_changed(int);
    void tool_button_style_changed(Qt::ToolButtonStyle);
    void language_changed(/*todo*/);

};

Q_DECLARE_METATYPE(Qt::ToolButtonStyle)

#endif // SETTINGS_HPP
