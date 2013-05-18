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
#include <QJsonObject>

Plugin::Plugin(const QJsonDocument &doc)
    : m_type(Invalid), m_enabled(false)
{
    if ( !doc.isNull() )
    {
        QVariantMap obj = doc.object().toVariantMap();
        m_name = obj["name"].toString();
        m_description = obj["description"].toString();
        if ( !m_name.isNull() )
        {
            /// \todo check type
            //QString type_name = obj["type"].toString();
        }
    }

}
