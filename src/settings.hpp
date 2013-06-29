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
#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <QObject>
#include <QMainWindow>
#include "string_toolbar.hpp"
#include <QMetaType>
#include "snapping_grid.hpp"

class Graph;

class Settings : public QObject
{
    Q_OBJECT
    Q_ENUMS(Qt::ToolButtonStyle)

public:
    //typedef Qt::ToolButtonStyle ToolButtonStyle;

public:
    enum Clipboard_Enum
    {
        KNOT = 0x01,
        SVG  = 0x02,
        PNG  = 0x04,
        XML  = 0x08,
        TIFF = 0x10
    };
    Q_DECLARE_FLAGS(Clipboard,Clipboard_Enum)

private:
    QString                     config_version; ///< Version of the loaded settings file

    bool                        save_nothing; ///< Whether to skip save_config (when clearing the saved conf)

    bool                        m_save_toolbars;
    QList<String_Toolbar>       toolbars;

    bool                        m_save_ui; ///< Whether the GUI state must be loaded between sessions
    int                         m_icon_size;
    QByteArray                  m_window_state;
    QByteArray                  m_window_geometry;
    Qt::ToolButtonStyle         tool_button_style;

    QStringList                 m_recent_files;
    int                         m_max_recent_files;

    bool                        m_graph_cache;
    bool                        m_fluid_refresh;
    bool                        m_antialiasing;
    int                         m_script_timeout;

    bool                        m_save_grid;
    bool                        m_grid_enabled;
    int                         m_grid_size;
    Snapping_Grid::Grid_Shape   m_grid_shape;

    bool                        m_check_unsaved_files; ///< When closing, ask to save changes

    bool                        m_save_knot_style;
    QString                     saved_knot_style_xml;

    Clipboard                   m_clipboard;


private:
    Settings();
    friend class Resource_Manager;

public:

    void load_config();
    void save_config();

    void initialize_window(QMainWindow*);
    void save_window(QMainWindow*);

    QString version() const;
    bool least_version(int maj,int min) const;
    bool current_version() const;

    void icon_size(int sz);
    int icon_size() const { return m_icon_size; }

    Qt::ToolButtonStyle button_style() const { return tool_button_style; }

    void button_style(Qt::ToolButtonStyle);

    bool graph_cache() const { return m_graph_cache; }
    bool fluid_refresh() const { return m_fluid_refresh; }
    bool antialiasing() const { return m_antialiasing; }
    int  script_timeout() const { return m_script_timeout; }

    void set_graph_cache(bool enable) { m_graph_cache = enable; }
    void set_fluid_refresh(bool enable) { m_fluid_refresh = enable; }
    void set_antialiasing(bool enable) { m_antialiasing = enable; }
    void set_script_timeout(int seconds) { m_script_timeout = seconds; }

    bool save_ui() const { return m_save_ui; }
    void set_save_ui(bool save) { m_save_ui = save; }

    QStringList recent_files() const { return m_recent_files; }

    /**
     *  \brief Push a file name to the top of the recent files list
     *
     *  If the file is already in the list, it is moved to the top.
     */
    void add_recent_file(QString file_name);

    int max_recent_files() const { return m_max_recent_files; }
    void set_max_recent_files(int max);
    void clear_recent_files();

    bool save_grid() const { return m_save_grid; }
    void set_save_grid(bool enable) { m_save_grid = enable; }
    int grid_size() const { return m_grid_size; }
    Snapping_Grid::Grid_Shape grid_shape() const { return m_grid_shape; }
    void set_grid_shape(Snapping_Grid::Grid_Shape gs) { m_grid_shape = gs; }
    bool grid_enabled() const { return m_grid_enabled; }


    bool save_toolbars() const { return m_save_toolbars; }
    void set_save_toolbars(bool save) { m_save_toolbars = save; }

    bool check_unsaved_files() const { return m_check_unsaved_files; }
    void set_check_unsaved_files(bool enable) { m_check_unsaved_files = enable; }

    bool save_knot_style() const { return m_save_knot_style; }
    void set_save_knot_style(bool enable ) { m_save_knot_style = enable; }
    void set_knot_style(const Graph& graph);
    void get_knot_style(Graph& graph) const;

    Clipboard clipboard() const { return m_clipboard; }
    bool clipboard_feature(Clipboard_Enum feature) const { return ( m_clipboard & feature ); }
    void set_clipboard_feature(Clipboard_Enum feature, bool enable);

public slots:
    void clear_config();

    void set_grid_size(int sz) { if ( sz > 0 ) m_grid_size = sz; }
    void set_grid_enabled(bool enable) { m_grid_enabled = enable; }


signals:
    void icon_size_changed(int);
    void tool_button_style_changed(Qt::ToolButtonStyle);
    void language_changed(/*todo*/);

};

Q_DECLARE_METATYPE(Qt::ToolButtonStyle)

Q_DECLARE_OPERATORS_FOR_FLAGS(Settings::Clipboard)

#endif // SETTINGS_HPP
