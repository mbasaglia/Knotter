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
#include "edge_style.hpp"
#include <QScriptEngine>
#include "plugin.hpp"

/**
 * Manage resources and data
 */
class Resource_Manager : public QObject
{
    Q_OBJECT

    Resource_Manager(){}
    Resource_Manager(const Resource_Manager&);
    Resource_Manager& operator= (const Resource_Manager&);
    ~Resource_Manager();

    static Resource_Manager singleton;


    QMap<QString,QString> lang_names; ///< map lang_name -> lang_code
    QMap<QString,QTranslator*> translators; ///< map lang_code -> translator
    QTranslator* current_translator;

    QList<Edge_Style*> m_edge_styles;
    QList<Cusp_Shape*> m_cusp_shapes;

    QScriptEngine *m_script_engine;
    QList<Plugin*>  m_plugins;
    QScriptContext *current_context;

public:
    static Settings settings;
    static Resource_Manager * const pointer ;

    /// Initialize the resource system
    static void initialize(QString default_lang_code="en");

    /// Get the translated program name
    static QString program_name();

    /// Get version as string
    static QString program_version();

    /// Check if has at least given version number
    static bool has_least_version(int maj, int min);


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
    static void register_edge_style(Edge_Style* style);

    /**
     *  \brief get the default edge style
     *
     *  \returns the first inserted style or nullptr if there is no style
     */
    static Edge_Style* default_edge_style();

    static QList<Edge_Style*> edge_styles() { return singleton.m_edge_styles; }

    /**
     *  \brief Cycle edge styles
     *
     *  Returns a different edge style so that calling next_edge_style n times
     *  ( where n is edge_styles().size() ) will return all installed styles.
     *
     *  Only returns NULL if there is no style available
     */
    static Edge_Style* next_edge_style(Edge_Style* style);
    /**
     *  \brief Cycle edge styles
     *
     *  Same as next_edge_style() but reversed
     *
     *  \sa next_edge_style
     */
    static Edge_Style* prev_edge_style(Edge_Style* style);

    /**
     *  \brief Get edge style from its machine-readable name
     *
     *  Scans every register style to check a match to the given name,
     *  if none is found, the default style is returned.
     *
     *  Resurns NULL only if there are no registered styles
     */
    static Edge_Style* edge_style_from_machine_name(QString name);


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

    static QList<Plugin*> plugins() { return singleton.m_plugins; }


    //static QScriptEngine& script_engine() { return *singleton.m_script_engine; }
    /// Adds parameter to the script context
    static void script_param(QString name, QScriptValue value);
    /// Adds parameter to the script context
    static void script_param(QString name, QObject* value);
    /// Adds parameter to the script context
    template <class T>
    static void script_param_template(QString name, const T& value)
    { script_param(name,singleton.m_script_engine->toScriptValue(value)); }

    /// Run a script in the current context
    static void run_script(Plugin* source);

    static QScriptValue run_script(const QString &program, const QString &fileName = QString(), int lineNumber = 1);


    static void emit_script_output(QString s) { emit singleton.script_output(s); }


public slots:

    static void change_lang_code ( QString code );
    static void change_lang_name ( QString name );

    void save_settings();

signals:

    void language_changed();

    /// Emitted when a cusp shape is registered or removed
    void cusp_shapes_changed();

    /// Emitted when an error has to be added to the plugin log
    void script_error(QString file,int line,QString msg, QStringList trace);

    void script_output(QString);

private:

    /**
     * \brief Creates a new script context
     *
     *  Creates a context only if there is no current context.
     *
     *  This is called implicitly by script_param() and run_script().
     */
    static void script_context();

};

#endif // RESOURCE_MANAGER_HPP
