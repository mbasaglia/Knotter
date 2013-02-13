#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <QDir>
#include <QScriptEngine>

/**
    \brief external script handler
*/
class Plugin : public QObject
{
        Q_OBJECT

    public:
        /**
            \brief Meta information

            The fields of this class are defined in
            DATA/plugins/(plugin)/info.ini [meta]

            They describe meta information about the plugin
        */
        struct meta
        {
            QString author;         ///< Author name
            QString version;        ///< Plugin version (can be any string)
            QString license;        ///< License identifier
            QString description;    ///< Brief description of the plugin
            QString title;          ///< Name of the plugin

        };

        /**
            \brief Plugin type

            Different types will give different semantic to the [plugin] group
        */
        enum type
        {
            CUSP
        };

        /// Base plugin directory, where to search for the plugins
        static const QDir& get_base_directory() { return base_directory; }
        /// Set base plugin directory, where to search for the plugins
        static bool set_base_directory(QString dir);
    private:

        /// Base plugin directory, where to search for the plugins
        static QDir base_directory;

        meta meta_info;         ///< Meta info
        QString m_id;           ///< Plugin ID, name of the subdirectory of base_directory
        QDir m_directory;       ///< Actual directory containing the plugin files
        type m_type;            ///< plugin type
        QScriptProgram m_script;///< The script content

        bool m_enabled;         ///< Whether the plugin has been enabled by the user
        bool loaded;            ///< Whether the plugin has been properly loaded

    public:
        Plugin() : m_enabled(false), loaded(false) {}

        explicit Plugin ( QString file_id, bool enabled = false )
            : m_enabled(enabled), loaded ( false )
        {
            load(file_id);
        }

        /**
            \brief Load base_directory/file_id/
            \return true <=> loaded succesfully
            \post loaded = return value, loaded = true => all data is initialized
        */
        bool load ( QString file_id );

        const meta& get_meta() const { return meta_info; }
        const QString& id() const { return m_id; }
        const QDir& directory() const { return m_directory; }
        const QScriptProgram& script() { return m_script; }
        const type& get_type() { return m_type; }


        bool enabled() const { return m_enabled; }

    public slots:
        void enable( bool enabled );

    signals:
        void enable_changed(bool enabled);

        
};

#endif // PLUGIN_HPP
