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

#if HAS_QT_5
#include <QJsonObject>
#include <QJsonDocument>
#else
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptValueIterator>
#endif




Plugin::Plugin(const QVariantMap &metadata, Plugin::Type type)
    : m_metadata(metadata), type(type), m_enabled(false)
{
}
void Plugin::enable(bool e)
{
    m_enabled = e;
    on_enable(e);
}

Plugin* Plugin::from_file (QFile &file, QString* error )
{



    error->clear();

    QVariantMap data;


    QByteArray json_data = file.readAll();

    QString type_string;

    #if HAS_QT_5
        QJsonParseError err;
        QJsonDocument json ( QJsonDocument::fromJson(json_data,&err) );
        if ( err.error != QJsonParseError::NoError )
        {
            *error = err.errorString();
            return p;
        }
        else if ( !json.isNull() )
        {
            data = json.object().toVariantMap();
        }
    #else
        QScriptEngine engine;
        QScriptValue obj = engine.evaluate("(" + json_data + ")");
        QScriptValueIterator it(obj);
        while (it.hasNext()) {
            it.next();
            if (it.value().isNumber())
                data.insert(it.name(),QVariant(it.value().toNumber()));
            else if (it.value().isBool())
                    data.insert(it.name(),QVariant(it.value().toBool()));
            else
                data.insert(it.name(),QVariant(it.value().toString()));
            data.insert(it.name(),it.value().toString());
        }
    #endif

    if ( !error->isEmpty() )
        return nullptr;

    QFileInfo fi(file.fileName());
    data["plugin_file"] = fi.absoluteFilePath();
    data["plugin_dir"] = fi.absolutePath();
    if ( data["name"].toString().isEmpty() )
    {

        data["name"] = fi.baseName();
    }

    Plugin::Type type = Plugin::Invalid;
    if ( data["type"] == "test" )
        type = Test;
    else if ( data["type"] == "cusp" )
        type = Cusp;
    /// \todo more types here



    switch (type)
    {
        case Test: return new Plugin(data,type);
        case Cusp: return new Plugin_Cusp(data);
        default:
            *error = QObject::tr("Unknown plugin type ");
            return nullptr;

    }

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
