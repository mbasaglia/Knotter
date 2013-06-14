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
#include "resource_manager.hpp"
#include <QDir>
#include <QIcon>
#include <QCoreApplication>
#include "script_line.hpp"
#include "misc_script_functions.hpp"
#include "script_window.hpp"
#include "script_polygon.hpp"

#if HAS_QT_5
#include <QStandardPaths>
#else
#include <QDesktopServices>
#endif

Settings          Resource_Manager::settings;
Resource_Manager  Resource_Manager::singleton;
Resource_Manager* const Resource_Manager::pointer = &singleton;

QString Resource_Manager::program_name()
{
    return tr("Knotter");
}

QString Resource_Manager::program_version()
{
    return QString(VERSION);
}

QString Resource_Manager::trimmed_program_version()
{
    static const QRegExp version_re("^[0-9]+\\.[0-9]+\\.[0-9]+");
    QString version_str = program_version();
    version_re.indexIn(version_str);
    return version_str.left(version_re.matchedLength());
}

bool Resource_Manager::has_least_version(int maj, int min)
{
    return check_least_version ( program_version(), maj, min );
}

bool Resource_Manager::check_least_version(QString version, int maj, int min)
{
    QStringList v = version.split(".");
    //int i = v[0].toInt(), j = v[1].toInt();
    if ( v.size() < 2)
        return false;
    else
        return v[0].toInt() >= maj&& v[1].toInt() >= min;
}

QString Resource_Manager::data(QString name)
{
    QDir path ( DATA_DIR ); // install dir

    if ( !path.exists(name) )
        path.setPath( QCoreApplication::applicationDirPath()+"/data" ); // executable dir

    if ( !path.exists(name) )
        path.setPath( QDir::currentPath()+"/data" ); // current dir


#if HAS_QT_5
    if ( !path.exists(name) )
    {
        foreach ( QString d, QStandardPaths::standardLocations(QStandardPaths::DataLocation)  )
        {
            if ( path.exists(name) )
                break;
            path.setPath(d);
        }
    }
#else
    if ( !path.exists(name) )
        path.setPath( QDesktopServices::storageLocation(QDesktopServices::DataLocation) );
#endif


    if ( !path.exists(name) )
        return QString(); // not found

    return QDir::cleanPath(path.absoluteFilePath(name));
}

QStringList Resource_Manager::data_directories(QString name)
{
    QStringList found;

    QStringList search = data_directories_unckecked(name);

    foreach ( QString d, search )
    {
        if ( QFileInfo(d).exists() )
            found << d;
    }
    found.removeDuplicates();

    return found;
}

QStringList Resource_Manager::data_directories_unckecked(QString name)
{
    QStringList search;
    search << DATA_DIR;  // install dir
#if HAS_QT_5
    search << QStandardPaths::standardLocations(QStandardPaths::DataLocation);
#else
    search << QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif
    search << QDir(QCoreApplication::applicationDirPath()).filePath("data") ; // executable dir
    search << QDir::current().filePath("data"); // current dir

    QStringList filter;
    foreach ( QString d, search )
    {
        filter << QDir::cleanPath(QDir(d).absoluteFilePath(name));
    }
    filter.removeDuplicates();

    return filter;
}



void Resource_Manager::initialize(QString default_lang_code)
{
    qApp->setApplicationName(TARGET);
    qApp->setApplicationVersion(program_version());
    qApp->setOrganizationDomain(DOMAIN_NAME);
    qApp->setOrganizationName(TARGET);

    // Clean up
    connect(qApp,SIGNAL(aboutToQuit()),&singleton,SLOT(save_settings()));


    // Initialize Icon theme

    QIcon::setThemeSearchPaths(
#ifdef DEBUG
                QStringList()
#else
                QIcon::themeSearchPaths()
#endif
                 << data("img/icons") );

    QIcon::setThemeName("knotter-icons");

    // Translation

    if ( !default_lang_code.isEmpty() )
    {
        QString name = language_name(default_lang_code);
        if ( !name.isEmpty() )
            register_translation(name,default_lang_code,QString());
    }

    QDir translations = data("translations");
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

    // Scripting
    singleton.m_script_engine = new QScriptEngine; // needs to be initialized only after qApp is created
    singleton.m_script_engine->setProcessEventsInterval(500);
    QScriptEngine* engine = singleton.m_script_engine; // shorer to write
    singleton.current_context = nullptr;
    singleton.m_script_engine_agent = new QScriptEngineAgent(engine);
    engine->setAgent(singleton.m_script_engine_agent);
    singleton.script_timeout = new QTimer;
    singleton.script_timeout->setSingleShot(true);
    connect(singleton.script_timeout,SIGNAL(timeout()),pointer,SLOT(abort_script()));

    qRegisterMetaType<Script_Point>("Script_Point");
    qScriptRegisterMetaType(engine, point_to_script, point_from_script);
    qRegisterMetaType<Script_Line>("Script_Line");
    qScriptRegisterMetaType(engine, line_to_script, line_from_script);
    qRegisterMetaType<Script_Graph>("Script_Graph");
    qScriptRegisterMetaType(engine, graph_to_script, graph_from_script);
    qRegisterMetaType<Script_Polygon>("Script_Polygon");
    qScriptRegisterMetaType(engine, polygon_to_script, polygon_from_script);


    //plugins
    load_plugins();
    emit singleton.plugins_changed();


    // Load Settings: note after load_plugins
    settings.load_config();
}


Resource_Manager::~Resource_Manager()
{
    foreach ( QTranslator* tr, translators.values() )
        delete tr;

    foreach ( Edge_Style* es, m_edge_styles )
        delete es;

    foreach ( Plugin* p, m_plugins)
        delete p;

    delete script_timeout;
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
    singleton.lang_names[name]=code;
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
        singleton.translators[code]=ntrans;
    }
    else
        singleton.translators[code] = nullptr;
}

QString Resource_Manager::current_lang_name()
{
    return singleton.lang_names.key(current_lang_code());
}

QString Resource_Manager::current_lang_code()
{
    return singleton.translators.key(singleton.current_translator);
}

QTranslator *Resource_Manager::translator()
{
    return singleton.current_translator;
}

QStringList Resource_Manager::available_languages()
{
    return singleton.lang_names.keys();
}

void Resource_Manager::change_lang_code(QString code)
{

    if ( !singleton.translators.contains(code) )
    {
        QString base_code = code.left(code.lastIndexOf('_')); // en_US -> en
        bool found = false;
        foreach ( QString installed_code, singleton.translators.keys() )
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
    app->removeTranslator(singleton.current_translator);
    singleton.current_translator = singleton.translators[code];
    app->installTranslator(singleton.current_translator);
    emit singleton.language_changed();
}

void Resource_Manager::change_lang_name(QString name)
{
    change_lang_code(singleton.lang_names[name]);
}

void Resource_Manager::save_settings()
{
    settings.save_config();
}


void Resource_Manager::register_edge_style(Edge_Style *style)
{
    singleton.m_edge_styles.push_back(style);
}

Edge_Style *Resource_Manager::default_edge_style()
{
    if ( singleton.m_edge_styles.empty() )
        return nullptr;
    return singleton.m_edge_styles.front();
}

Edge_Style *Resource_Manager::next_edge_style(Edge_Style *style)
{
    int sz = singleton.m_edge_styles.size();
    for ( int i = 0; i < sz; i++ )
        if ( singleton.m_edge_styles[i] == style )
            return singleton.m_edge_styles[(i+1)%sz];
    return default_edge_style();
}

Edge_Style *Resource_Manager::prev_edge_style(Edge_Style *style)
{
    int sz = singleton.m_edge_styles.size();
    for ( int i = sz-1; i >= 0; i-- )
    {
        if ( singleton.m_edge_styles[i] == style )
        {
            if ( i == 0 )
                return singleton.m_edge_styles.back();

            return singleton.m_edge_styles[i-1];
        }
    }
    return default_edge_style();
}

Edge_Style *Resource_Manager::edge_style_from_machine_name(QString name)
{
    foreach(Edge_Style* st, singleton.m_edge_styles )
        if ( st->machine_name() == name )
            return st;
    return default_edge_style();
}

void Resource_Manager::register_cusp_shape(Cusp_Shape *style)
{
    singleton.m_cusp_shapes.push_back(style);
    emit singleton.cusp_shapes_changed();
}

void Resource_Manager::remove_cusp_shape(Cusp_Shape *shape)
{
    singleton.m_cusp_shapes.removeOne(shape);
    emit singleton.cusp_shapes_changed();
}

Cusp_Shape *Resource_Manager::default_cusp_shape()
{
    if ( singleton.m_cusp_shapes.empty() )
        return nullptr;
    return singleton.m_cusp_shapes.front();
}

Cusp_Shape *Resource_Manager::cusp_shape_from_machine_name(QString name)
{
    foreach(Cusp_Shape* st, singleton.m_cusp_shapes )
        if ( st->machine_name() == name )
            return st;
    return default_cusp_shape();
}

void Resource_Manager::load_plugins(QString directory)
{
    QDir plugin_dir = QDir(directory);
    QRegExp file_name ( "plugin_(.+)\\.json" );
    if ( plugin_dir.exists() )
    {
        foreach ( QFileInfo finfo, plugin_dir.entryInfoList(QDir::Dirs|
                                            QDir::Files|QDir::NoDotAndDotDot) )
        {
            if ( finfo.isDir() )
                load_plugins(finfo.absoluteFilePath());
            else if ( finfo.isFile() && finfo.isReadable() &&
                      file_name.indexIn(finfo.fileName()) != -1 )
                load_plugin(finfo.absoluteFilePath());
        }
    }
}

void Resource_Manager::load_plugins()
{
    foreach (QString plugin_dir_name, data_directories("plugins") )
    {
        load_plugins(plugin_dir_name);
    }
}

void Resource_Manager::reload_plugins()
{
    QMap<QString,bool> active;
    foreach(Plugin* p,singleton.m_plugins )
    {
        active[p->string_data("plugin_file")] = p->is_enabled();
        p->enable(false);
        delete p;
    }
    singleton.m_plugins.clear();
    load_plugins();

    foreach(Plugin* p,singleton.m_plugins )
    {
        if ( active.contains(p->string_data("plugin_file")) )
            p->enable(active[p->string_data("plugin_file")]);
    }

    emit singleton.plugins_changed();
}

QList<Plugin *> Resource_Manager::active_plugins(Plugin::Type type)
{
    QList<Plugin*> l;
    foreach(Plugin* p,singleton.m_plugins)
        if ( p->type() == type && p->is_enabled())
            l << p;
    return l;
}

QScriptContext* Resource_Manager::script_context()
{
    if ( singleton.current_context == nullptr )
    {
        QScriptEngine* engine = singleton.m_script_engine;

        singleton.current_context = engine->pushContext();

        engine->globalObject().setProperty("Point", engine->newFunction(build_point));
        ///sengine->globalObject().setProperty("diff", engine->newFunction(subtract_points));
        engine->globalObject().setProperty("opposite", engine->newFunction(opposite_point));
        engine->globalObject().setProperty("distance", engine->newFunction(distance));

        engine->globalObject().setProperty("Line", engine->newFunction(build_line));

        engine->globalObject().setProperty( "print", engine->newFunction( script_print ) );

        engine->globalObject().setProperty( "knotter",
            engine->newQObject(new Script_Knotter,QScriptEngine::ScriptOwnership));

        engine->globalObject().setProperty("Graph", engine->newFunction(build_graph));


        engine->globalObject().setProperty("Polygon", engine->newFunction(build_polygon));
    }
    return singleton.current_context;
}

void Resource_Manager::script_param(QString name, QScriptValue value)
{
    script_context();
    singleton.current_context->activationObject().setProperty(name,value);
}


void Resource_Manager::script_param(QString name, QObject* value,
                                    QScriptEngine::ValueOwnership owner)
{
    script_context();
    singleton.current_context->activationObject().setProperty(name,
              singleton.m_script_engine->newQObject(value,owner)
    );
}

void Resource_Manager::run_script(Plugin *source)
{

    script_context();

    QScriptValue plugin = singleton.m_script_engine->newObject();
    foreach(QString k, source->metadata().keys())
    {
        plugin.setProperty(k,singleton.m_script_engine->newVariant(source->metadata()[k]));
    }
    script_param("plugin",plugin);

    run_script(source->script_program().sourceCode(),
               source->script_program().fileName(),
               source->script_program().firstLineNumber());
}

QScriptValue Resource_Manager::run_script(const QString &program,
                                          const QString &fileName,
                                          int lineNumber,
                                          QScriptValue *context_value)
{
    script_context();
    if ( settings.script_timeout() > 0 )
    {
        singleton.script_timeout->start(1000*settings.script_timeout());
    }

    emit singleton.running_script(true);

    QScriptValue result = singleton.m_script_engine->evaluate(program,fileName,lineNumber);

    emit singleton.running_script(false);

    if ( singleton.m_script_engine->hasUncaughtException() )
    {
        qWarning() << QObject::tr("%1:%2:Error: %3")
                      .arg(fileName)
                      .arg(singleton.m_script_engine->uncaughtExceptionLineNumber())
                      .arg(singleton.m_script_engine->uncaughtException().toString());;
        qWarning() << singleton.m_script_engine->uncaughtExceptionBacktrace();
        emit singleton.script_error(fileName,
                                  singleton.m_script_engine->uncaughtExceptionLineNumber(),
                                  singleton.m_script_engine->uncaughtException().toString(),
                                  singleton.m_script_engine->uncaughtExceptionBacktrace()
                                );
        singleton.m_script_engine->clearExceptions();
    }
    if ( context_value != nullptr )
        *context_value = singleton.current_context->activationObject();
    singleton.m_script_engine->popContext();
    singleton.current_context = nullptr;
    return result;
}


void Resource_Manager::load_plugin(QString filename)
{
    QFile file(filename);
    if ( !file.open(QFile::Text|QFile::ReadOnly) )
        return;


    QString error;
    Plugin *p = Plugin::from_file(file,&error);

    if ( !p && error.isEmpty() )
        error = tr("Unknown error");

    if ( !error.isEmpty() )
        qWarning() << tr("%1: Error: %2").arg(filename).arg(error);

    singleton.m_plugins << p;
}


void Resource_Manager::abort_script()
{
    if ( m_script_engine->isEvaluating() )
    {
        QScriptValue v;
        if ( current_context )
        {
            v = current_context->throwError(tr("Script aborted"));
        }
        m_script_engine->abortEvaluation(v);
    }
}
