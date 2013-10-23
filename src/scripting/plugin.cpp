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

#include "plugin.hpp"
#include <QFileInfo>
#include <QDir>
#include "resource_manager.hpp"
#include "plugin_cusp.hpp"
#include "plugin_crossing.hpp"
#include <QUiLoader>
#include <QFile>
#include <QDialog>
#include "main_window.hpp"
#include "json_stuff.hpp"


Plugin::Plugin()
    : m_type(Invalid), m_enabled(false)
{
}

Plugin::Plugin(const QVariantMap &metadata, Plugin::Type type)
    : m_metadata(metadata), m_type(type), m_enabled(true)
{

    if ( m_metadata.contains("ui") )
    {

       QStringList ui = m_metadata["ui"].toStringList();
       if ( ui.isEmpty() )
           ui << m_metadata["ui"].toString();


       foreach(QString file_name, ui)
       {
            QFile ui_file(QDir(string_data("plugin_dir"))
                          .absoluteFilePath(file_name) );
            if ( ui_file.open(QFile::ReadOnly|QFile::Text) )
            {
                QUiLoader loader;

                QWidget *widget = loader.load(&ui_file);
                if ( widget )
                {
                    if ( widget->windowIcon().isNull() )
                        widget->setWindowIcon(icon());
                    //((QObject*)widget)->setParent(this);
                    widget->hide();
                    m_widgets << widget;
                    connect(widget,SIGNAL(destroyed(QObject*)),SLOT(dialog_destroyed(QObject*)));
                }
            }
       }
    }


    m_enabled = data("auto_enable",true);
}

Plugin::~Plugin()
{
    foreach(QWidget* w, m_widgets)
        delete w;
}

bool Plugin::is_enabled() const
{
    return m_enabled && is_valid();
}


void Plugin::enable(bool e)
{
    if ( is_valid() )
    {
        m_enabled = e;
        on_enable(e);
        emit enabled(e);
    }
}

Plugin* Plugin::from_file (QFile &file, QString* error )
{
    error->clear();

    QVariantMap data = json_read_file(file);

    QFileInfo fi(file.fileName());
    data["plugin_file"] = fi.absoluteFilePath();
    data["plugin_dir"] = fi.absolutePath();
    data["plugin_shortname"] = fi.baseName();
    if ( data["name"].toString().isEmpty() )
    {
        data["name"] = data["plugin_shortname"];
    }

    if ( !data.contains("plugin_settings") )
    {
        data["plugin_settings"] = data["plugin_shortname"].toString()+"_config.json";
    }


    if ( !error->isEmpty() )
    {
        return new_error_plugin(data,*error);
    }


    Plugin::Type type = Plugin::Invalid;
    if ( data["type"] == "cusp" )
        type = Cusp;
    else if ( data["type"] == "crossing" )
        type = Crossing;
    else
        type = Script;

    if ( !data.contains("category") )
    {
        switch (type)
        {
            case Cusp: data["category"] = QObject::tr("Cusp"); break;
            case Crossing: data["category"] = QObject::tr("Crossing"); break;
            case Script: data["category"] = QObject::tr("Other"); break;
            default: data["category"] = QObject::tr("Invalid"); break;
        }

    }

    Plugin* p;
    switch (type)
    {
        case Cusp:
            p = new Plugin_Cusp(data);
            break;
        case Crossing:
            p = new Plugin_Crossing(data);
            break;
        case Script:
            p = new Plugin(data,type);
            break;
        default:
            *error = QObject::tr("Unknown plugin type");
            return new_error_plugin(data,*error);

    }

    if ( !data.contains("script") )
    {
        *error = QObject::tr("Missing script file");
        return new_error_plugin(data,*error);
    }

    if ( !p->reload_script_file() )
    {
        *error = QObject::tr("Error while opening script file %1").arg(p->script_file_path());
        //delete p;
        //return new_error_plugin(data,*error);
    }
    return p;
}


Plugin *Plugin::new_error_plugin(QVariantMap data, QString message)
{

    data["type"] = "invalid";
    data["error"] = message;
    data["auto_enable"] = false;
    return new Plugin(data,Invalid);
}

bool Plugin::is_valid() const
{
    if ( m_type == Invalid )
        return false;
    if ( m_metadata.contains("requires") &&
         !Resource_Manager::has_least_version(string_data("requires")) )
        return false;
    return true;
}

bool Plugin::reload_script_file()
{

    QFile script_file(script_file_path());

    if ( !script_file.open(QFile::Text|QFile::ReadOnly) )
    {
        return false;
    }
    m_script = QScriptProgram(script_file.readAll(),script_file_path());

    return true;
}

QString Plugin::settings_file_path() const
{;
    return settings_directory().absoluteFilePath(string_data("plugin_settings"));
}

QDir Plugin::settings_directory()
{
    return Resource_Manager::writable_data_directory("plugin_config");
}

QString Plugin::script_file_path() const
{
    return string_data("plugin_dir")+"/"+string_data("script");
}

QIcon Plugin::icon() const
{
    QString icon_name = string_data("icon");
    if ( icon_name.isEmpty() )
        return QIcon();

    if ( QIcon::hasThemeIcon(icon_name) )
        return QIcon::fromTheme(icon_name);

    QDir rel_path = string_data("plugin_dir");
    if ( rel_path.exists(icon_name) )
        return QIcon(rel_path.absoluteFilePath(icon_name));

    QFileInfo abs_file = icon_name;
    if ( abs_file.exists() )
    {
        return QIcon(icon_name);
    }

    QString data = Resource_Manager::data(icon_name);
    if ( !data.isEmpty() )
        return QIcon(data);

    return QIcon();

}

void Plugin::execute(Main_Window *window)
{
    foreach(QWidget* w, m_widgets)
        Resource_Manager::script_param(w->objectName(),w);

    if ( window )
    {
        Script_Window win(window);
        Resource_Manager::script_param("window",&win);
        Resource_Manager::script_param("document",win.document());
        Resource_Manager::run_script(this);
    }
    else
        Resource_Manager::run_script(this);

}

void Plugin::set_widget_parent(QWidget *parent)
{
    foreach(QWidget* w, m_widgets)
        w->setParent(parent,Qt::Dialog);
}


void Plugin::dialog_destroyed(QObject * widget)
{
    m_widgets.removeOne((QWidget*)widget);
}


