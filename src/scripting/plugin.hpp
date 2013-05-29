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
#include <QVariantMap>
#include "c++.hpp"
#include <QIcon>
#include <QScriptProgram>


class Plugin
{
public:
    enum Type
    {
        Invalid,    ///< An invalid plugin
        Test,       ///< A plugin that has no use
        Cusp        ///< Node cusp
    };

    QVariantMap     m_metadata;
    Type            type;
    bool            m_enabled;
    QScriptProgram  m_script;


public:
    explicit Plugin(const QVariantMap& metadata, Type type);
    virtual ~Plugin() {}

    /**
     * \brief Metadata map
     */
    const QVariantMap& metadata() const { return m_metadata; }
    /**
     * \brief Get single data item as string
     *
     * \return Empty string if no data with \c name is found or is not convertible to a string
     */
    QString string_data(QString name) const { return m_metadata[name].toString(); }

    template<class T>
        T data(QString name, T def = T()) const
        {
            if ( m_metadata.contains(name) && m_metadata[name].canConvert<T>() )
                return m_metadata[name].value<T>();
            return def;
        }

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

    QIcon icon() const;

    const QScriptProgram& script_program() const { return m_script; }

protected:
    /**
     * \brief Function called by enable()
     *
     * Child classes may override this to perform custom actions on enable/disable
     */
    virtual void on_enable(bool){}

    void set_data(QString name, QVariant value) { m_metadata[name] = value; }
};




#endif // PLUGIN_HPP
