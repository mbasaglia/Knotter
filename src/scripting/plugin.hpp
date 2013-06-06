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
#include <QObject>


class Plugin : public QObject
{
    Q_OBJECT

public:
    enum Type
    {
        Invalid,    ///< An invalid plugin
        Cusp,       ///< Node cusp
        Script      ///< A script triggered by the user
    };

private:
    QVariantMap     m_metadata;
    Type            m_type;
    bool            m_enabled;
    QScriptProgram  m_script;
    QList<QWidget*> m_widgets;

public:
    Plugin();
    explicit Plugin(const QVariantMap& metadata, Type type);
    ~Plugin();

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
    bool is_enabled() const;
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


    /**
     * \brief Set parent for loaded Ui widgets
     * \param parent Parent for the loaded widgets
     */
    void set_widget_parent(QWidget *parent);

    Type type() const { return m_type; }

    /**
     * @brief Create a new error plugin
     * @param data      Metadata
     * @param message   Error message
     * @return A new plugin of type Invalid
     */
    static Plugin* new_error_plugin(QVariantMap data,QString message);

    bool is_valid() const { return m_type != Invalid; }

protected:
    /**
     * \brief Function called by enable()
     *
     * Child classes may override this to perform custom actions on enable/disable
     */
    virtual void on_enable(bool){}

    void set_data(QString name, QVariant value) { m_metadata[name] = value; }

public slots:

    /**
     *  \brief Run the script
     */
    void execute();

signals:
    void enabled(bool);

private slots:
    void dialog_destroyed(QObject*widget);

};

Q_DECLARE_METATYPE(Plugin*)



#endif // PLUGIN_HPP
