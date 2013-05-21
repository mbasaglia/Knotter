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

    data["plugin_file"] = file.fileName();

    Plugin::Type type = Plugin::Invalid;
    if ( data["type"] == "test" )
        type = Test;
    /// \todo more types here

    if ( data["name"].toString().isEmpty() )
    {
        QFileInfo fi(file.fileName());

        data["name"] = fi.baseName();
    }

    if ( type == Plugin::Invalid )
        *error = QObject::tr("Unknown plugin type ");

    if ( !error->isEmpty() )
        return nullptr;

    /// \todo switch type to create proper object
    return new Plugin(data,type);
}
