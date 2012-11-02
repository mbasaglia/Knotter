#include "plugin.hpp"
#include <QSettings>
#include "knot_curve_script.hpp"
#include <QDebug>

QDir Plugin::base_directory;

bool Plugin::set_base_directory(QString dir)
{
    QDir d = dir;
    if ( d.exists() )
    {
        base_directory = dir;
        return true;
    }
    else if ( QDir().mkdir(dir) )
    {
        base_directory = dir;
        return true;
    }
    else
    {
        qWarning() << tr("Cannot set plugin directory to %1").arg(dir);
        return false;
    }
}

bool Plugin::load( QString file_id )
{
    loaded = false;

    m_directory = base_directory.filePath(file_id);
    m_id = file_id;

    if ( !m_directory.exists() )
    {
        qWarning() << tr("Plugin %1 does not exists").arg(m_directory.path());
        return false;
    }

    QSettings info(m_directory.filePath("info.ini"),QSettings::IniFormat);
    info.beginGroup("meta");
    meta_info.author = info.value("author",tr("Unknown")).toString();
    meta_info.license = info.value("license",tr("Public Domain")).toString();
    meta_info.version = info.value("version",tr("0.0")).toString();
    meta_info.description = info.value("description","").toString();
    meta_info.title = info.value("title",m_id).toString();
    info.endGroup();

    info.beginGroup("plugin");
    QString type_string = info.value("type").toString().toLower();

    if ( type_string == "cusp" )
        m_type = CUSP;
    else
    {
        qWarning() << tr("Unknown plugin type: '%1'").arg(type_string);
        return false;
    }

    if ( !info.contains("script") )
        return false;

    QFile f( m_directory.filePath(info.value("script").toString()) );
    if ( !f.open(QIODevice::ReadOnly) )
    {
        qWarning() << tr("Cannot load script file %1").arg(f.fileName());
        return false;
    }
    m_script = QScriptProgram(f.readAll(),f.fileName());

    return  loaded = true;
}

void Plugin::enable(bool en)
{
    if ( Q_LIKELY(loaded) )
    {
        m_enabled = en;

        if ( m_type == CUSP )
        {

            if ( m_enabled )
            {
                if ( knot_curve_styler::style(m_id) != NULL )
                    qWarning() << tr("Curve shape %1 already registered").arg(m_id);
                else
                    knot_curve_styler::register_style(new knot_curve_script(m_script),m_id, meta_info.title );
            }
            else
                knot_curve_styler::unregister(m_id);
        }


        emit enable_changed(en);
    }
}
