/**

\file

\author Mattia Basaglia

\section License

Copyright (C) 2014  Mattia Basaglia

This is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef RESOURCE_SCRIPT_HPP
#define RESOURCE_SCRIPT_HPP

#include <QObject>
#include <QScriptEngine>
#include "plugin.hpp"
#include <QScriptEngineAgent>
#include <QTimer>

class Resource_Script : public QObject
{
    Q_OBJECT

private:
    QScriptEngine *     m_script_engine;
    QList<Plugin*>      m_plugins;
    QScriptContext *    current_context;
    QScriptEngineAgent* m_script_engine_agent;
    QTimer*             script_timeout;


    Resource_Script(){}
    Resource_Script(const Resource_Script&);
    Resource_Script& operator= (const Resource_Script&);
    friend class Resource_Manager;

    /// \note must be called after QApplication has been initialized
    void initialize();

public:

    ~Resource_Script();

    /// Load plugin from json file
    void load_plugin(QString filename);
    /// Load all plugins from given directory
    void load_plugins(QString directory);
    /// Load plugin from data directories
    void load_plugins();

    /// Load plugin from data directories, keep active plugins
    void reload_plugins();

    /// Get all plugins
    QList<Plugin*> plugins() { return m_plugins; }
    /// Get active plugins of given type
    QList<Plugin*> active_plugins(Plugin::Type type);


    /*/// Get a reference to the internal script engine
    static QScriptEngine* script_engine() { return singleton.m_script_engine; }*/

    /// Get the script engine agent
    QScriptEngineAgent& script_engine_agent() { return *m_script_engine_agent; }

    /**
     * \brief Get or create a new script context
     *
     *  Creates a context only if there is no current context.
     *
     *  This is called implicitly by script_param() and run_script().
     */
    QScriptContext* script_context();

    /// Adds parameter to the script context
    void param(QString name, QScriptValue value);
    /// Adds parameter to the script context
    void param(QString name, QObject* value,
                             QScriptEngine::ValueOwnership owner = QScriptEngine::QtOwnership );
    /// Adds parameter to the script context
    template <class T>
    void param_template(QString name, const T& value)
    { param(name,m_script_engine->toScriptValue(value)); }

    /**
     *  \brief Run a script in the current context
     *  \param source      Code to be executed
     *  \param[out] activation_object If not \c nullptr used to store the context activation object
     *  \param[out] global_object     If not \c nullptr used to store the context global object
     *  \return The value resulting from the evaluation of the script
     */
    QScriptValue execute(Plugin* source, QScriptValue* activation_object=nullptr);

    /**
     *  \brief Execute a script
     *  \param program      Code to be executed
     *  \param fileName     Name of the file, used for error reporting
     *  \param lineNumber   First line of the file, used for error reporting
     *  \param[out] activation_object If not \c nullptr used to store the context activation object
     *  \return The value resulting from the evaluation of the script
     */
    QScriptValue execute(const QString &program,
                                   const QString &fileName = QString(),
                                   int lineNumber = 1,
                                   QScriptValue* activation_object=nullptr );


    void emit_output(QString s) { emit output(s); }

public slots:

    /**
     * \brief Terminates currently running scripts
     */
    void abort_script();

signals:
    /// Emitted when an error has to be added to the plugin log
    void error(QString file,int line,QString msg, QStringList trace);

    void output(QString);

    /// Emitted when new plugins are loaded
    void plugins_changed();

    /// Emitted at the start and end of a script execution
    void running_script(bool);
};

#endif // RESOURCE_SCRIPT_HPP
