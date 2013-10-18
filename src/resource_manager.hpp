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
#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <QObject>
#include "settings.hpp"
#include <QTranslator>
#include "c++.hpp"
#include "edge_type.hpp"
#include <QScriptEngine>
#include "plugin.hpp"
#include <QScriptEngineAgent>
#include <QTimer>

/**
 * Manage resources and data
 */
class Resource_Manager : public QObject
{
    Q_OBJECT

private:
    Resource_Manager(){}
    Resource_Manager(const Resource_Manager&);
    Resource_Manager& operator= (const Resource_Manager&);
    ~Resource_Manager();

    static Resource_Manager singleton;


    QMap<QString,QString> lang_names; ///< map lang_name -> lang_code
    QMap<QString,QTranslator*> translators; ///< map lang_code -> translator
    QTranslator* current_translator;

    QList<Edge_Type*>  m_edge_types;
    QList<Cusp_Shape*> m_cusp_shapes;

    QScriptEngine *     m_script_engine;
    QList<Plugin*>      m_plugins;
    QScriptContext *    current_context;
    QScriptEngineAgent* m_script_engine_agent;
    QTimer*             script_timeout;

    //QString m_default_style_name;

public:
    static Settings settings;
    static Resource_Manager * const pointer ;

    /// Initialize the resource system
    static void initialize(QString default_lang_code="en");

    /// Get the translated program name
    static QString program_name();

    /// Get version as string
    static QString program_version();


    /// Get version as string (without _devel identifier)
    static QString trimmed_program_version();

    /// Check if has at least given version number
    static bool has_least_version(int maj, int min);

    /**
     * \brief Check if has at least given version number
     * \param version Acceptable input: "0.9.5_devel" and similar
     */
    static bool has_least_version(QString version);


    /// Check if given version string has at least given version number
    static bool check_least_version(QString version, int maj, int min);


    /**
     * \brief Get full path of given data file
     * \param name Name of the file (relative to datadir)
     * \return Full path or null string if not found
     */
    static QString data(QString name);


    /**
     * \brief Get all available directories to search data from
     * \param name Name of the data directory
     */
    static QStringList data_directories(QString name);


    /**
     * \brief Get all directories to search data from
     *
     * This function may include directories that don't exist but that will be
     * checked if they existed
     *
     * \param name Name of the data directory
     */
    static QStringList data_directories_unckecked(QString name);

    /**
     *  \brief Determine human readable language name from ISO 639-1 code
     *
     *  Depending on Qt version the returned string is either in English or
     *  in the language itself.
     *
     *  If lang_code is not rectognised, a null string is returned
    */
    static QString language_name ( QString lang_code );


    /**
     *  \brief Register a translation
     *
     *  \param name Human-readable language name
     *  \param code ISO 639-1 language code
     *  \param file Path to the translation file, if empty no file gets loaded
    */
    static void register_translation ( QString name, QString code, QString file );


    static QString current_lang_name();
    static QString current_lang_code();
    static QTranslator* translator();
    /**
     *  Get list of the names of all the available languages
    */
    static QStringList available_languages();

    /**
     *  \brief Register an edge style
     *
     *  Takes ownership of the style pointer
     *
     *  \param style Style to be registered
    */
    static void register_edge_type(Edge_Type* type);

    /**
     *  \brief get the default edge style
     *
     *  \returns the first inserted style or nullptr if there is no style
     */
    static Edge_Type* default_edge_type();

    static QList<Edge_Type*> edge_styles() { return singleton.m_edge_types; }

    /**
     *  \brief Cycle edge styles
     *
     *  Returns a different edge style so that calling next_edge_style n times
     *  ( where n is edge_styles().size() ) will return all installed styles.
     *
     *  Only returns NULL if there is no style available
     */
    static Edge_Type* next_edge_type(Edge_Type* style);
    /**
     *  \brief Cycle edge styles
     *
     *  Same as next_edge_style() but reversed
     *
     *  \sa next_edge_style
     */
    static Edge_Type* prev_edge_type(Edge_Type* type);

    /**
     *  \brief Get edge style from its machine-readable name
     *
     *  Scans every register style to check a match to the given name,
     *  if none is found, the default style is returned.
     *
     *  Resurns NULL only if there are no registered styles
     */
    static Edge_Type* edge_type_from_machine_name(QString name);


    /**
     *  \brief Register a cusp style
     *
     *  Takes ownership of the style pointer
     *
     *  \param style Style to be registered
    */
    static void register_cusp_shape(Cusp_Shape* style);

    /**
     * \brief Removes a cusp style
     *
     * The object is not destroyed but loses ownership, the caller
     * becomes responsible of destructing the shape object.
     */
    static void remove_cusp_shape(Cusp_Shape* shape);

    static QList<Cusp_Shape*> cusp_shapes() { return singleton.m_cusp_shapes; }

    static Cusp_Shape* default_cusp_shape();
    /**
     *  \brief Get cusp shape from its machine-readable name
     *
     *  Scans every register style to check a match to the given name,
     *  if none is found, the default style is returned.
     *
     *  Resurns NULL only if there are no registered styles
     */
    static Cusp_Shape* cusp_shape_from_machine_name(QString name);

    /// Load plugin from json file
    static void load_plugin(QString filename);
    /// Load all plugins from given directory
    static void load_plugins(QString directory);
    /// Load plugin from data directories
    static void load_plugins();

    /// Load plugin from data directories, keep active plugins
    static void reload_plugins();

    /// Get all plugins
    static QList<Plugin*> plugins() { return singleton.m_plugins; }
    /// Get active plugins of given type
    static QList<Plugin*> active_plugins(Plugin::Type type);

    static QScriptEngineAgent& script_engine_agent() { return *singleton.m_script_engine_agent; }

    /**
     * \brief Get or create a new script context
     *
     *  Creates a context only if there is no current context.
     *
     *  This is called implicitly by script_param() and run_script().
     */
    static QScriptContext* script_context();

    /// Adds parameter to the script context
    static void script_param(QString name, QScriptValue value);
    /// Adds parameter to the script context
    static void script_param(QString name, QObject* value,
                             QScriptEngine::ValueOwnership owner = QScriptEngine::QtOwnership );
    /// Adds parameter to the script context
    template <class T>
    static void script_param_template(QString name, const T& value)
    { script_param(name,singleton.m_script_engine->toScriptValue(value)); }

    /// Run a script in the current context
    static void run_script(Plugin* source);

    /**
     *  \brief Execute a script
     *  \param program      Code to be executed
     *  \param fileName     Name of the file, used for error reporting
     *  \param lineNumber   First line of the file, used for error reporting
     *  \param[out] context_value If not \c nullptr used to store the context value
     *  \return The value resulting from the evaluation of the script
     */
    static QScriptValue run_script(const QString &program,
                                   const QString &fileName = QString(),
                                   int lineNumber = 1,
                                   QScriptValue* context_value=nullptr);


    static void emit_script_output(QString s) { emit singleton.script_output(s); }

    //static QString default_style_name() { return singleton.m_default_style_name; }

public slots:

    static void change_lang_code ( QString code );
    static void change_lang_name ( QString name );

    void save_settings();

    /**
     * \brief Terminates currently running scripts
     */
    void abort_script();

signals:

    void language_changed();

    /// Emitted when a cusp shape is registered or removed
    void cusp_shapes_changed();

    /// Emitted when an error has to be added to the plugin log
    void script_error(QString file,int line,QString msg, QStringList trace);

    void script_output(QString);

    /// Emitted when new plugins are loaded
    void plugins_changed();

    /// Emitted at the start and end of a script execution
    void running_script(bool);

};


#endif // RESOURCE_MANAGER_HPP
