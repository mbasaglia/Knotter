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
#include "resource_manager.hpp"
#include <QDir>
#include <QIcon>
#include <QCoreApplication>
#include <QApplication>
#include <QStyle>
#include <QNetworkRequest>



void Resource_Manager::initialize(QString default_lang_code)
{
    qApp->setApplicationName(TARGET);
    qApp->setApplicationVersion(program.version());
    qApp->setOrganizationDomain(DOMAIN_NAME);
    qApp->setOrganizationName(TARGET);

    // Clean up
    connect(qApp,SIGNAL(aboutToQuit()),pointer(),SLOT(save_settings()));


    // Initialize Icon theme

    QIcon::setThemeSearchPaths(
#ifdef DEBUG
                QStringList()
#else
                QIcon::themeSearchPaths()
#endif
                 << program.data("img/icons") );

    QIcon::setThemeName("knotter-icons");

    // Translation

    if ( !default_lang_code.isEmpty() )
    {
        QString name = language_name(default_lang_code);
        if ( !name.isEmpty() )
            register_translation(name,default_lang_code,QString());
    }

    QDir translations = program.data("translations");
    QStringList translation_files = translations.entryList(QStringList()<<"*.qm");

    QRegExp re("[^_]+_([^.]+)\\.qm");
    foreach ( QString file, translation_files )
    {
        if ( re.exactMatch(file) )
        {
            QString code = re.cap(1);
            QString name = language_name(code);
            if ( !name.isEmpty() )
                register_translation(name,code,translations.absoluteFilePath(file));
        }
        else
            qWarning() << tr("Warning:")
                       << tr("Unrecognised translation file name pattern: %1")
                          .arg(file);
    }

    change_lang_code(QLocale::system().name());

    script.initialize();

    //network
    m_network_access_manager = new QNetworkAccessManager;


    // Load Settings: note after load_plugins
    settings.load_config();
}

Resource_Manager::~Resource_Manager()
{
    foreach ( QTranslator* tr, translators.values() )
        delete tr;

    foreach ( Edge_Type* es, m_edge_types )
        delete es;

    #if !HAS_QT_5
        delete m_network_access_manager;
    #endif
}

Resource_Manager &Resource_Manager::instance()
{
    static Resource_Manager singleton;
    return singleton;
}

Resource_Manager * Resource_Manager::pointer()
{
    return &instance();
}



QString Resource_Manager::language_name(QString lang_code)
{
    QLocale lang_loc = QLocale(lang_code);
    QString name;

    #if HAS_QT_4_8
        name = lang_loc.nativeLanguageName(); // native name
    #else
        name = QLocale::languageToString(lang_loc.language()); // English name
    #endif

    if ( !name.isEmpty() )
        name[0] = name[0].toUpper();

    return name;
}

void Resource_Manager::register_translation(QString name, QString code, QString file)
{
    lang_names[name]=code;
    if ( !file.isEmpty() )
    {
        QTranslator* ntrans = new QTranslator;
        if ( ! ntrans->load(file) )
            qWarning() << tr("Warning:") <<
            /*: %1 is the file name,
             *  %2 is the human-readable language code
             *  %3 is the ISO language code
             */
            tr("Error on loading translation file %1 for language %2 (%3)")
            .arg(file).arg(name).arg(code);
        translators[code]=ntrans;
    }
    else
        translators[code] = nullptr;
}

QString Resource_Manager::current_lang_name()
{
    return lang_names.key(current_lang_code());
}

QString Resource_Manager::current_lang_code()
{
    return translators.key(current_translator);
}

QTranslator *Resource_Manager::translator()
{
    return current_translator;
}

QStringList Resource_Manager::available_languages()
{
    return lang_names.keys();
}

void Resource_Manager::change_lang_code(QString code)
{

    if ( !translators.contains(code) )
    {
        QString base_code = code.left(code.lastIndexOf('_')); // en_US -> en
        bool found = false;
        foreach ( QString installed_code, translators.keys() )
        {
            if ( installed_code.left(installed_code.lastIndexOf('_')) == base_code )
            {
                code = installed_code;
                found = true;
                break;
            }
        }
        if ( not found )
        {
            qWarning() << tr("Warning:") <<
                          tr("There is no translation for language %1 (%2)")
                          .arg(language_name(code))
                          .arg(code);
            return;
        }
    }

    QCoreApplication* app = QCoreApplication::instance();
    app->removeTranslator(current_translator);
    current_translator = translators[code];
    app->installTranslator(current_translator);
    emit language_changed();
}

void Resource_Manager::change_lang_name(QString name)
{
    change_lang_code(lang_names[name]);
}

void Resource_Manager::save_settings()
{
    settings.save_config();
}


void Resource_Manager::register_edge_type(Edge_Type *type)
{
    m_edge_types.push_back(type);
    emit edge_types_changed();
}

void Resource_Manager::remove_edge_type(Edge_Type *type)
{
    m_edge_types.removeOne(type);
    emit edge_types_changed();
}

Edge_Type *Resource_Manager::default_edge_type()
{
    if ( m_edge_types.empty() )
        return nullptr;
    return m_edge_types.front();
}

Edge_Type *Resource_Manager::next_edge_type(Edge_Type *type)
{
    int sz = m_edge_types.size();
    for ( int i = 0; i < sz; i++ )
        if ( m_edge_types[i] == type )
            return m_edge_types[(i+1)%sz];
    return default_edge_type();
}

Edge_Type *Resource_Manager::prev_edge_type(Edge_Type *type)
{
    int sz = m_edge_types.size();
    for ( int i = sz-1; i >= 0; i-- )
    {
        if ( m_edge_types[i] == type )
        {
            if ( i == 0 )
                return m_edge_types.back();

            return m_edge_types[i-1];
        }
    }
    return default_edge_type();
}

Edge_Type *Resource_Manager::edge_type_from_machine_name(QString name)
{
    foreach(Edge_Type* st, m_edge_types )
        if ( st->machine_name() == name )
            return st;
    return default_edge_type();
}

void Resource_Manager::register_cusp_shape(Cusp_Shape *style)
{
    m_cusp_shapes.push_back(style);
    emit cusp_shapes_changed();
}

void Resource_Manager::remove_cusp_shape(Cusp_Shape *shape)
{
    m_cusp_shapes.removeOne(shape);
    emit cusp_shapes_changed();
}

Cusp_Shape *Resource_Manager::default_cusp_shape()
{
    if ( m_cusp_shapes.empty() )
        return nullptr;
    return m_cusp_shapes.front();
}

Cusp_Shape *Resource_Manager::cusp_shape_from_machine_name(QString name)
{
    foreach(Cusp_Shape* st, m_cusp_shapes )
        if ( st->machine_name() == name )
            return st;
    return default_cusp_shape();
}


QNetworkReply *Resource_Manager::network_get(QString url)
{
    return m_network_access_manager->get(QNetworkRequest(QUrl(url)));
}

