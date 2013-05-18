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

#ifndef PLUGIN_HPP
#define PLUGIN_HPP
#include <QString>
#include <QVariant>
#include <QJsonDocument>
class Plugin
{
public:
    enum Type
    {
        Invalid     ///< An invalid plugin
    };

private:
    QString     m_name;
    QString     m_description;
    Type        m_type;
    bool        m_enabled;

public:
    Plugin(const QJsonDocument &doc);
    const QString& name() const { return m_name; }
    const QString& description() const { return m_description; }
    Type type() const { return m_type; }

    bool enabled() const { return m_enabled; }
    void enable(bool e) { m_enabled = e; }
};

#endif // PLUGIN_HPP
