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
#include "script_color.hpp"
#include "edge_scripted.hpp"
#include <QApplication>
#include <QStyle>
#include "json_stuff.hpp"
#include "script_qtablewidget.hpp"
#include <QNetworkRequest>

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

bool Resource_Manager::has_least_version(QString version)
{
    QStringList v_in = version.split(".");
    QStringList v_k = program_version().split(".");
    if ( v_in.size() < 2 )
        return false; // weird format
    for ( int i = 0; i < v_in.size(); i++ )
    {
        if ( i >= v_k.size() )
            return false; // input version has one extra sub-number
        int vk = v_k[i].remove(QRegExp("[^0-9]")).toInt();
        int vin = v_in[i].remove(QRegExp("[^0-9]")).toInt();
        if ( vk < vin )
            return false; // input version is greater
        else if ( vk > vin )
            return true; // local version is greater
    }
    return true; // versions are the same
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

QString Resource_Manager::writable_data_directory(QString name)
{
    QStringList search;

#if HAS_QT_5
    search << QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
    search << QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#endif

    if ( !search.empty() )
    {
        return QDir::cleanPath(QDir(search[0]).absoluteFilePath(name));
    }

    return QString();
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
    qRegisterMetaType<Script_Color>("Script_Color");
    qScriptRegisterMetaType(engine, color_to_script, color_from_script);

    qScriptRegisterMetaType(engine,edge_handle_to_script,edge_handle_from_script);


    //plugins
    load_plugins();
    emit singleton.plugins_changed();

    //network
    singleton.m_network_access_manager = new QNetworkAccessManager;


    // Load Settings: note after load_plugins
    settings.load_config();
}


Resource_Manager::~Resource_Manager()
{
    foreach ( QTranslator* tr, translators.values() )
        delete tr;

    foreach ( Edge_Type* es, m_edge_types )
        delete es;

    foreach ( Plugin* p, m_plugins)
        delete p;

    delete script_timeout;

    #if !HAS_QT_5
        delete m_network_access_manager;
    #endif
    delete m_script_engine;
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


void Resource_Manager::register_edge_type(Edge_Type *type)
{
    singleton.m_edge_types.push_back(type);
    emit singleton.edge_types_changed();
}

void Resource_Manager::remove_edge_type(Edge_Type *type)
{
    singleton.m_edge_types.removeOne(type);
    emit singleton.edge_types_changed();
}

Edge_Type *Resource_Manager::default_edge_type()
{
    if ( singleton.m_edge_types.empty() )
        return nullptr;
    return singleton.m_edge_types.front();
}

Edge_Type *Resource_Manager::next_edge_type(Edge_Type *type)
{
    int sz = singleton.m_edge_types.size();
    for ( int i = 0; i < sz; i++ )
        if ( singleton.m_edge_types[i] == type )
            return singleton.m_edge_types[(i+1)%sz];
    return default_edge_type();
}

Edge_Type *Resource_Manager::prev_edge_type(Edge_Type *type)
{
    int sz = singleton.m_edge_types.size();
    for ( int i = sz-1; i >= 0; i-- )
    {
        if ( singleton.m_edge_types[i] == type )
        {
            if ( i == 0 )
                return singleton.m_edge_types.back();

            return singleton.m_edge_types[i-1];
        }
    }
    return default_edge_type();
}

Edge_Type *Resource_Manager::edge_type_from_machine_name(QString name)
{
    foreach(Edge_Type* st, singleton.m_edge_types )
        if ( st->machine_name() == name )
            return st;
    return default_edge_type();
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
        if ( !p->data("plugin_deprecated",false)  &&
             active.contains(p->string_data("plugin_file")) )
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
        engine->globalObject().setProperty( "system",
            engine->newQObject(new Script_System,QScriptEngine::ScriptOwnership));
        engine->globalObject().setProperty("run_script", engine->newFunction(script_run_script));

        engine->globalObject().setProperty("Graph", engine->newFunction(build_graph));


        engine->globalObject().setProperty("Polygon", engine->newFunction(build_polygon));


        engine->globalObject().setProperty("Color", engine->newFunction(build_color));
        engine->globalObject().setProperty("rgb", engine->newFunction(script_rgb));
        engine->globalObject().setProperty("rgba", engine->newFunction(script_rgb));
        engine->globalObject().setProperty("hsv", engine->newFunction(script_hsv));
        engine->globalObject().setProperty("hsl", engine->newFunction(script_hsl));
        engine->globalObject().setProperty("cmyk", engine->newFunction(script_cmyk));



        QScriptValue gui = singleton.m_script_engine->newObject();
        gui.setProperty("table_widget",engine->newFunction(script_create_tablewidget_wrapper));
        engine->globalObject().setProperty( "gui",gui);

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

QScriptValue Resource_Manager::run_script(Plugin *source,
                                          QScriptValue *activation_object)
{

    script_context();

    QScriptValue plugin = singleton.m_script_engine->newObject();
    foreach(QString k, source->metadata().keys())
    {
        plugin.setProperty(k,singleton.m_script_engine->toScriptValue(source->metadata()[k]));
    }

    QFile plugin_settings(source->settings_file_path());
    if ( plugin_settings.open(QFile::ReadOnly) )
    {
        QScriptValue settings = json_read_file(plugin_settings,singleton.m_script_engine);
        if ( settings.isError() )
        {
            emit singleton.script_error(plugin_settings.fileName(),
                                        settings.property("lineNumber").toInt32(),
                                        settings.property("message").toString(),
                                        QStringList());
        }
        else if ( !settings.isNull() )
            plugin.setProperty("settings",settings);
        plugin_settings.close();
    }


    script_param("plugin",plugin);

    QScriptValue *activation_object_local = activation_object;
    if ( activation_object == nullptr )
        activation_object_local = new QScriptValue;


    QScriptValue result = run_script(source->script_program().sourceCode(),
                      source->script_program().fileName(),
                      source->script_program().firstLineNumber(),
                      activation_object_local);


    QVariant new_settings = activation_object_local->property("plugin")
                                .property("settings").toVariant();
    if ( new_settings.isValid() && !new_settings.isNull() )
    {
        QDir plugin_settings_dir = Plugin::settings_directory();
        if ( !plugin_settings_dir.exists() )
            plugin_settings_dir.mkpath(".");
        plugin_settings.unsetError();
        if ( ! plugin_settings.open(QFile::WriteOnly) )
        {
            emit singleton.script_error(plugin_settings.fileName(),0,
                                        tr("Cannot open file"),QStringList() );
        }
        else
        {
            json_write_file(plugin_settings,new_settings);
        }
    }

    if ( activation_object == nullptr  )
        delete activation_object_local;

    return result;

}

QScriptValue Resource_Manager::run_script(const QString &program,
                                          const QString &fileName,
                                          int lineNumber,
                                          QScriptValue *activation_object)
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
    if ( activation_object != nullptr )
        *activation_object = singleton.current_context->activationObject();
    singleton.m_script_engine->popContext();
    singleton.current_context = nullptr;
    return result;
}

QNetworkReply *Resource_Manager::network_get(QString url)
{
    return singleton.m_network_access_manager->get(QNetworkRequest(QUrl(url)));
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

    if ( p->is_enabled() )
    {
        foreach(Plugin* other, singleton.m_plugins)
        {
            if ( other->is_enabled() &&
                 other->string_data("plugin_shortname") == p->string_data("plugin_shortname") )
            {
                if ( other->data<double>("version",0) < p->data<double>("version",0) )
                {
                    other->enable(false);
                    other->set_data("plugin_deprecated",true);
                }
                else
                {
                    p->enable(false);
                    other->set_data("plugin_deprecated",false);
                }
            }
        }
    }

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
