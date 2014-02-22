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

#include "resource_script.hpp"

#include "script_line.hpp"
#include "misc_script_functions.hpp"
#include "script_window.hpp"
#include "script_polygon.hpp"
#include "script_color.hpp"
#include "script_qtablewidget.hpp"
#include "edge_scripted.hpp"
#include "json_stuff.hpp"
#include "resource_manager.hpp"


void Resource_Script::initialize()
{

    // Scripting
    m_script_engine = new QScriptEngine; // needs to be initialized only after qApp is created
    m_script_engine->setProcessEventsInterval(500);
    QScriptEngine* engine = m_script_engine; // shorer to write
    current_context = nullptr;
    m_script_engine_agent = new QScriptEngineAgent(engine);
    engine->setAgent(m_script_engine_agent);
    script_timeout = new QTimer;
    script_timeout->setSingleShot(true);
    connect(script_timeout,SIGNAL(timeout()),this,SLOT(abort_script()));

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
    emit plugins_changed();

}

Resource_Script::~Resource_Script()
{
    foreach ( Plugin* p, m_plugins)
        delete p;

    delete script_timeout;
    delete m_script_engine;
}


void Resource_Script::load_plugins(QString directory)
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

void Resource_Script::load_plugins()
{
    foreach (QString plugin_dir_name, resource_manager().program.data_directories("plugins") )
    {
        load_plugins(plugin_dir_name);
    }
}

void Resource_Script::reload_plugins()
{
    QMap<QString,bool> active;
    foreach(Plugin* p,m_plugins )
    {
        active[p->string_data("plugin_file")] = p->is_enabled();
        p->enable(false);
        delete p;
    }
    m_plugins.clear();
    load_plugins();

    foreach(Plugin* p,m_plugins )
    {
        if ( !p->data("plugin_deprecated",false)  &&
             active.contains(p->string_data("plugin_file")) )
            p->enable(active[p->string_data("plugin_file")]);
    }

    emit plugins_changed();
}

QList<Plugin *> Resource_Script::active_plugins(Plugin::Type type)
{
    QList<Plugin*> l;
    foreach(Plugin* p,m_plugins)
        if ( p->type() == type && p->is_enabled())
            l << p;
    return l;
}

QScriptContext* Resource_Script::script_context()
{
    if ( current_context == nullptr )
    {
        QScriptEngine* engine = m_script_engine;

        current_context = engine->pushContext();

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



        QScriptValue gui = m_script_engine->newObject();
        gui.setProperty("table_widget",engine->newFunction(script_create_tablewidget_wrapper));
        engine->globalObject().setProperty( "gui",gui);

    }
    return current_context;
}

void Resource_Script::param(QString name, QScriptValue value)
{
    script_context();
    current_context->activationObject().setProperty(name,value);
}


void Resource_Script::param(QString name, QObject* value,
                                    QScriptEngine::ValueOwnership owner)
{
    script_context();
    current_context->activationObject().setProperty(name,
              m_script_engine->newQObject(value,owner)
    );
}

QScriptValue Resource_Script::execute(Plugin *source,
                                          QScriptValue *activation_object)
{

    script_context();

    QScriptValue plugin = m_script_engine->newObject();
    foreach(QString k, source->metadata().keys())
    {
        plugin.setProperty(k,m_script_engine->toScriptValue(source->metadata()[k]));
    }

    QFile plugin_settings(source->settings_file_path());
    if ( plugin_settings.open(QFile::ReadOnly) )
    {
        QScriptValue settings = json_read_file(plugin_settings,m_script_engine);
        if ( settings.isError() )
        {
            emit error(plugin_settings.fileName(),
                                        settings.property("lineNumber").toInt32(),
                                        settings.property("message").toString(),
                                        QStringList());
        }
        else if ( !settings.isNull() )
            plugin.setProperty("settings",settings);
        plugin_settings.close();
    }


    param("plugin",plugin);

    QScriptValue *activation_object_local = activation_object;
    if ( activation_object == nullptr )
        activation_object_local = new QScriptValue;


    QScriptValue result = execute(source->script_program().sourceCode(),
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
            emit error(plugin_settings.fileName(),0,
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

QScriptValue Resource_Script::execute(const QString &program,
                                          const QString &fileName,
                                          int lineNumber,
                                          QScriptValue *activation_object)
{
    script_context();
    if ( resource_manager().settings.script_timeout() > 0 )
    {
        script_timeout->start(1000*resource_manager().settings.script_timeout());
    }

    emit running_script(true);

    QScriptValue result = m_script_engine->evaluate(program,fileName,lineNumber);

    emit running_script(false);

    if ( m_script_engine->hasUncaughtException() )
    {
        qWarning() << QObject::tr("%1:%2:Error: %3")
                      .arg(fileName)
                      .arg(m_script_engine->uncaughtExceptionLineNumber())
                      .arg(m_script_engine->uncaughtException().toString());;
        qWarning() << m_script_engine->uncaughtExceptionBacktrace();
        emit error(fileName,
                                  m_script_engine->uncaughtExceptionLineNumber(),
                                  m_script_engine->uncaughtException().toString(),
                                  m_script_engine->uncaughtExceptionBacktrace()
                                );
        m_script_engine->clearExceptions();
    }
    if ( activation_object != nullptr )
        *activation_object = current_context->activationObject();
    m_script_engine->popContext();
    current_context = nullptr;
    return result;
}


void Resource_Script::load_plugin(QString filename)
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
        foreach(Plugin* other, m_plugins)
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

    m_plugins << p;
}


void Resource_Script::abort_script()
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
