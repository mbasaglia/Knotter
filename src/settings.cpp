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
#include "settings.hpp"
#include "resource_manager.hpp"
#include <QSettings>

void Settings::load_config()
{
    QSettings settings("knotter","knotter");

    config_version = settings.value("version",VERSION).toString();

    /// \todo Language

    /// \todo performance

    settings.beginGroup("gui");


    int ntoolbars = settings.beginReadArray("toolbar");
    for ( int i = 0; i < ntoolbars; i++ )
    {
        settings.setArrayIndex(i);

        String_Toolbar tb;

        tb.name = settings.value("name").toString();
        if ( tb.name.isEmpty() )
        {
             qWarning() << tr("Warning:")
                        << tr("Not loading toolbar without name");
            continue;
        }
        tb.title = settings.value("title").toString();

        tb.actions = settings.value("items").toStringList();

        toolbars.push_back(tb);
    }
    settings.endArray();//toolbars


    save_ui = settings.value("save",save_ui).toBool();
    window_state = settings.value("geometry").toByteArray();
    window_geometry = settings.value("state").toByteArray();

    m_icon_size = settings.value("icon_size",m_icon_size).toInt();

    tool_button_style = settings.value("buttons",tool_button_style).
            value<Qt::ToolButtonStyle>();

    settings.endGroup();//gui

    /// \todo recent files

    /// \todo style, grid

}

void Settings::save_config()
{
    /// \todo
}


void Settings::initialize_window(QMainWindow *w)
{
    if ( !toolbars.empty() )
    {
        foreach(QToolBar* tb, w->findChildren<QToolBar*>() )
            delete tb;
        foreach(const String_Toolbar& tb, toolbars)
            tb.create(w);
    }

    if ( save_ui )
    {
        w->restoreGeometry(window_geometry);
        w->restoreState(window_state);
        w->setIconSize(QSize(m_icon_size,m_icon_size));
        w->setToolButtonStyle(tool_button_style);
    }
}

void Settings::save_window(QMainWindow *w)
{
    toolbars.clear();
    foreach(QToolBar* tb, w->findChildren<QToolBar*>())
        toolbars.append(tb);
    m_icon_size = w->iconSize().width();
    tool_button_style = w->toolButtonStyle();
}

QString Settings::version() const
{
    return config_version;
}

bool Settings::least_version(int maj, int min) const
{
    return Resource_Manager::check_least_version(config_version,maj,min);
}

bool Settings::current_version() const
{
    return Resource_Manager::program_version() == config_version;
}

void Settings::icon_size(int sz)
{
    m_icon_size = sz;
    emit icon_size_changed(sz);
}

void Settings::button_style(Qt::ToolButtonStyle bs)
{
    tool_button_style = bs;
    emit tool_button_style_changed(bs);
}

Settings::Settings()
    : m_icon_size(22), save_ui(true), tool_button_style(Qt::ToolButtonFollowStyle)
{
}
