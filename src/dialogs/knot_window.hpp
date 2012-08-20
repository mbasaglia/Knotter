#ifndef KNOT_WINDOW_HPP
#define KNOT_WINDOW_HPP
/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012  Mattia Basaglia

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
#include "ui_knot_window.h"
#include <QUndoStack>
#include <QUndoView>
#include "closable_dock.hpp"
#include "clipboard.hpp"
#include "config_dialog.hpp"
#include "export_dialog.hpp"
#include "help_view.hpp"

/**
    Main knot window
*/
class Knot_Window : public QMainWindow, private Ui::Knot_Window
{
        Q_OBJECT

    protected:
        QString         filename;           ///< current/last file name used on open or save
        closable_dock*  undoDock;          ///< Little window showing undoable/redoable actions
        ClipboardItem   clipboard;          ///< Part of the graph copied to clipboard
        bool            save_ui;            ///< Whether to save geometry and toolbars on exit
        QStringList     recent_files;       ///< List of recently used files
        int             max_recent_files;   ///< Max number of entries in the Open recent menu
        config_dialog   config_dlg;         ///< Knotter preferences dialog
        Export_Dialog   export_dialog;      ///< Export SVG/Bitmap dialog
        Help_View       help_view;          ///< Window showing the user guide
        closable_dock*  default_node_style_dock;
        closable_dock*  global_style_dock;

    public:
        explicit Knot_Window(QWidget *parent = 0);
        ~Knot_Window();

        /**
            \brief Open the given file
            \param file     The name of the file to be open
            \param silent   Whether messagebox on error is disabled
            \return Whether the file was loaded without problem
        */
        bool open( QString file, bool silent = false );
        /// get the KnotView used by the window
        KnotView& knotview();

    protected:
        /// Save current knot to file
        void save ( QString file );

        /// make path the first item in the recent files menu
        void push_recent_file(QString path);
        /// synch the Open recent menu with the contents of recent_files
        void update_recent_menu();

        /// Update the grid icon in the toolbar/menu to the current grid shape
        void update_grid_icon();

    protected slots:

        /**
            \brief Change the window title to "Knotter - filename*"
            \param clean Whether the file is unedited
        */
        void update_title(bool clean);

        /// cause a segmentation fault, used to test error recovery
        void cause_crash();

        /// load configuration
        void load_config();
        /// save configuration
        void save_config();

        /// called when one of the Open recent entrieas has been clicked
        void click_recent();


    public slots:
        /// Change editing mode to edge list
        void mode_edge_list();
        /// Change editing mode to edge/node manipulation
        void mode_edge();

        /// Update the statusbar to show the mouse position
        void mouse_moved(QPointF);

        void copy();
        void cut();
        void paste();

        void clear();

        /// Save with current filename or show save dialog
        void save();
        /// Show save dialog
        void saveAs();
        /// Show open dialog
        void open();
        /// Show export dialog
        void export_image();

        void zoom_in();
        void zoom_out();

        void configure_grid();

        void enable_grid ( bool enabled );

        /// show preferences dialog
        void config();

        void clear_recent_files();

        void show_help();

    private slots:
        /// show about dialog
        void on_action_About_triggered();
};

#endif // KNOT_WINDOW_HPP
