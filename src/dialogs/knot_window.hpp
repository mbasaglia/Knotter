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
#include "config_dialog.hpp"
#include "export_dialog.hpp"
#include "help_view.hpp"
#include "context_menu.hpp"
#include "node_style_form.hpp"
#include "global_style_form.hpp"
#include "background_config.hpp"
#include <QPrinter>
#include "gridconfig.hpp"

/**
    Main knot window
*/
class Knot_Window : public QMainWindow, private Ui::Knot_Window
{
        Q_OBJECT

    protected:
        bool            save_ui;            ///< Whether to save geometry and docks on exit
        QStringList     recent_files;       ///< List of recently used files
        int             max_recent_files;   ///< Max number of entries in the Open recent menu
        config_dialog   config_dlg;         ///< Knotter preferences dialog
        Export_Dialog   export_dialog;      ///< Export SVG/Bitmap dialog
        Help_View       help_view;          ///< Window showing the user guide
        node_cxmn       node_context_menu;  ///< Menu shown on right-clicking on a node
        edge_cxmn       edge_context_menu;  ///< Menu shown on right-clicking on an edge
        QUndoView *     undoView;           ///< Action history
        node_style_form*default_node_style_form;
        node_style_form*selected_node_style_form;
        global_style_form* global_style_frm;
        bool            save_toolbars;      ///< Whether custom toolbar buttons shall be saved on exit
        bool            save_style;         ///< Whether style shall be saved on exit
        bool            save_anything;      ///< Whether the save config shall be performed on exit
        Background_Config back_config;
        QDoubleSpinBox* zoomer;             ///< Zomm spinner on statusbar
        KnotView*       canvas;             ///< Current tab widget
        QPrinter        printer;
        GridConfig      grid_config_dialog;

    public:
        explicit Knot_Window(KnotGraph* graph = 0, QWidget *parent = 0);
        ~Knot_Window();

        /**
            \brief Open the given file
            \param file     The name of the file to be open
            \return Whether the file was loaded without problem
        */
        bool open( QString file);
        /// get the KnotView used by the window
        KnotView* knotview();

    protected:
        /// Save current knot to file
        void save ( QString file );

        /// make path the first item in the recent files menu
        void push_recent_file(QString path);

        /// synch the Open recent menu with the contents of recent_files
        void update_recent_menu();


        /// update ui elements based on the actual values
        void update_ui();

        void init_menus();
        void init_docks();
        void init_toolbars();
        void init_dialogs();

        void connect_view();
        void disconnect_view();
        void new_tab(QString file = QString());

        void dropEvent(QDropEvent *event);
        void dragEnterEvent(QDragEnterEvent *event);

        KnotView* canvas_at(int index);

    public slots:

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

        void print ( QPrinter* );


    private slots:

        /// Update the grid icon in the toolbar/menu to the current grid shape
        void update_grid_icon();

        /**
            \brief Change the window title to "Knotter - filename*"
            \param clean Whether the file is unedited
        */
        void update_title();

        /// load configuration
        void load_config();
        /// save configuration
        void save_config();

        /// called when one of the Open recent entrieas has been clicked
        void click_recent();

        /// show node-wise context menu
        void show_node_prefs(Node*);

        void insert_toolbar(QToolBar*);

        void drag_tab_away(int tab);


        /// show about dialog
        void on_action_About_triggered();
        void on_actionShow_Graph_triggered(bool checked);
        void on_actionInsert_Polygon_triggered();
        void retranslate();
        void on_action_Background_triggered();

        void set_clean_icon(bool);
        void set_undo_text(QString);
        void set_redo_text(QString);

        /// enable/disable widgets that act on selection within current canvas
        void enable_selection_action();

        void set_selected_style(styleinfo);


        void on_action_Undo_triggered();
        void on_action_Redo_triggered();
        void on_action_Erase_triggered();
        void on_action_Merge_triggered();
        void on_action_Link_triggered();
        void on_actionSelect_All_triggered();
        void on_actionInsert_Edges_triggered();
        void on_actionRefresh_Path_triggered();
        void on_action_Reset_View_triggered();
        void on_action_Reset_Zoom_triggered();
        void on_action_Horizontal_Flip_triggered();
        void on_action_Vertical_Flip_triggered();
        void on_action_Move_Grid_triggered();
        void on_actionShow_Knot_line_triggered(bool checked);
        void on_action_Unlink_triggered();
        void on_action_Rotate_triggered();
        void on_action_Scale_triggered();
        void on_actionEdge_List_triggered();
        void on_tabWidget_currentChanged(QWidget *arg1);
        void on_tabWidget_tabCloseRequested(int index);
        void on_actionSave_A_ll_triggered();
        void on_actionS_nap_to_grid_triggered();
        void on_action_Print_triggered();
        void on_actionPage_Set_up_triggered();
        void on_actionPr_int_Preview_triggered();
        void on_action_Highlight_loops_triggered(bool checked);
};

#endif // KNOT_WINDOW_HPP
