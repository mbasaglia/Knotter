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

#include "json_stuff.hpp"
#include <QTextStream>
#include <QScriptValue>
#include <QScriptEngine>
#include <QScriptValueIterator>


QString json_escape(QString s)
{
    s.replace('\\',"\\\\");
    s.replace('"',"\\\"");
    s.replace('\'',"\\\'");
    s.replace('\n',"\\n");
    return '"'+s+'"';
}

QVariant json_read_file(QIODevice& file)
{
    if ( ! file.isOpen() )
        return QVariant();

    QScriptEngine engine;
    QScriptValue obj = json_read_file(file,&engine);
    return obj.toVariant();
}

QScriptValue json_read_file(QIODevice& file, QScriptEngine* engine)
{
    QByteArray json_data = file.readAll();
    return engine->evaluate("(" + json_data + ")");
}



/**
 * \brief Helper for json_convert
 */
void json_convert_list(QTextStream& json, QVariantList list , int indent)
{
    json << "[ ";
    for ( int i = 0; i < list.size(); i++ )
    {
        json_convert(json,list[i],indent+1);
        if ( i < list.size()-1 )
            json << ", ";
    }
    json << "]";

}

/**
 * \brief Helper for json_convert
 */
void json_convert_map(QTextStream& json, QVariantMap map , int indent)
{
    json << "{\n";
    for ( int i = 0; i < map.size(); i++ )
    {
        json << QString(indent+1,'\t') << json_escape(map.keys()[i]) << " : ";
        json_convert(json,map.values()[i], indent+1);
        if ( i < map.size()-1 )
            json << ",";
        json << "\n";
    }
    json << QString(indent,'\t') << "}";
}

void json_convert(QTextStream& json, QVariant v, int indent)
{
    switch (v.type())
    {
        case QVariant::List:
            json_convert_list(json,v.toList(),indent);
            break;
        case QVariant::Map:
            json_convert_map(json,v.toMap(),indent);
            break;
        case QVariant::Bool:
        case QVariant::Char:
        case QVariant::Double:
        case QVariant::Int:
        case QVariant::LongLong:
        case QVariant::UInt:
        case QVariant::ULongLong:
            json << v.toDouble();
            break;
        case QVariant::String:
            json << json_escape(v.toString());
            break;
        default:
            if ( v.canConvert(QVariant::List) )
            {
                json_convert_map(json,v.toMap(),indent);
            }
            else if ( v.canConvert(QVariant::String) )
            {
                json << json_escape(v.toString());
            }
            else
            {
                json << "null /*unsupported value*/";
            }

    }
}

void json_write_file( QIODevice&file, QVariant values )
{
    /// \todo maybe use JSON.stringify
    if ( file.isOpen() && values.isValid() )
    {
        QTextStream json(&file);
        json_convert(json, values, 0);
    }
}
