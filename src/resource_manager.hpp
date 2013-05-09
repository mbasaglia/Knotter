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
#include <QDebug>
#include "settings.hpp"
#include <QTranslator>
#include "c++.hpp"
#include "edge_style.hpp"

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

    QVector<Edge_Style*> m_edge_styles;
    QVector<Cusp_Shape*> m_cusp_shapes;

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


    /** Get full path of given data file
     * \param name Name of the file (relative to datadir)
     * \return Full path or null string if not found
     */
    static QString data(QString name);

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

    static QVector<Edge_Style*> edge_styles() { return singleton.m_edge_styles; }

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

    static QVector<Cusp_Shape*> cusp_shapes() { return singleton.m_cusp_shapes; }

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

public slots:

    static void change_lang_code ( QString code );
    static void change_lang_name ( QString name );

    void save_settings();

signals:

    void language_changed();
    
};

#endif // RESOURCE_MANAGER_HPP
