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
#include "c++.hpp"

#if HAS_QT_5
#include <QJsonObject>
#include <QJsonDocument>
#else
#include <QScriptValue>
#include <QScriptEngine>
#endif

Plugin_Metadata::Plugin_Metadata()
    : type(Invalid)
{
}

Plugin_Metadata Plugin_Metadata::from_file(QFile &file, QString *error)
{

    error->clear();

    Plugin_Metadata p;

    p.filename = file.fileName();

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
            QVariantMap obj = json.object().toVariantMap();
            p.name = obj["name"].toString();
            p.description = obj["description"].toString();
            p.version = obj["version"].toString();
            p.author = obj["author"].toString();
            p.license = obj["license"].toString();
            type_string = obj["type"].toString();
        }
    #else
        QScriptEngine engine;
        QScriptValue obj = engine.evaluate("(" + json_data + ")");
        p.name = obj.property("name").toString();
        p.description = obj.property("description").toString();
        p.version = obj.property("version").toString();
        p.author = obj.property("author").toString();
        p.license = obj.property("license").toString();
        type_string = obj.property("type").toString();
    #endif

    if ( type_string == "test" )
        p.type = Test;

    /// \todo check type_string
    if ( p.name.isEmpty() )
        *error = QObject::tr("Plugin name not specified");
    else if ( p.type == Plugin_Metadata::Invalid )
        *error = QObject::tr("Unknown plugin type ");

    return p;
}



Plugin::Plugin(const Plugin_Metadata &metadata)
    : m_metadata(metadata), m_enabled(false)
{
}

void Plugin::enable(bool e)
{
    m_enabled = e;
}
Plugin* Plugin::from_file (QFile &file, QString* error )
{
    Plugin_Metadata md = Plugin_Metadata::from_file(file,error);
    if ( !error->isEmpty() )
        return nullptr;

    /// \todo switch type to create proper object
    return new Plugin(md);
}
