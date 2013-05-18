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

#if HAS_QT_5
#include <QJsonObject>
#include <QJsonDocument>
#else
#include <QScriptValue>
#include <QScriptEngine>
#endif

Plugin::Plugin()
    : m_type(Invalid), m_enabled(false)
{

}

Plugin Plugin::from_file(QFile &file, QString *error)
{

    Plugin p;

    p.m_filename = file.fileName();

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
            p.m_name = obj["name"].toString();
            p.m_description = obj["description"].toString();
            type_string = obj["type"].toString();
        }
    #else
        QScriptEngine engine;
        QScriptValue obj = engine.evaluate("(" + json_data + ")");
        p.m_name = obj.property("name").toString();
        p.m_description = obj.property("description").toString();
        type_string = obj.property("type").toString();
    #endif

    if ( type_string == "test" )
        p.m_type = Test;

    /// \todo check type_string
    if ( p.m_name.isEmpty() )
        *error = QObject::tr("Plugin name not specified");
    else if ( p.m_type == Plugin::Invalid )
        *error = QObject::tr("Unknown plugin type ");

    return p;
}
