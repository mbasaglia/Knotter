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
#include <QFile>

class Plugin_Metadata
{

public:
    enum Type
    {
        Invalid,    ///< An invalid plugin
        Test        ///< A plugin that has no use
    };

    QString     name;
    QString     description;
    QString     filename;
    QString     author; ///< \todo
    QString     license;///< \todo
    QString     version;///< \todo
    Type        type;

    explicit Plugin_Metadata();


    /**
     * \brief Create plugin metadata from JSON file
     *
     * \param[in]  file  Json file
     * \param[out] error Error string
     */
    static Plugin_Metadata from_file (QFile &file, QString* error );
};

class Plugin
{

    Plugin_Metadata m_metadata;
    bool            m_enabled;

public:
    explicit Plugin(const Plugin_Metadata& metadata);

    const Plugin_Metadata& metadata() const { return m_metadata; }

    /// Whether the plugin has been enabled
    bool enabled() const { return m_enabled; }
    /// Enable or disable the plugin
    void enable(bool e);

    /**
     * \brief Create a plugin from JSON file
     *
     * \param[in]  file  Json file
     * \param[out] error Error string
     *
     * \return A Dynamic object of the proper specialized class, NULL if invalid.
     */
    static Plugin* from_file (QFile &file, QString* error );
};




#endif // PLUGIN_HPP
