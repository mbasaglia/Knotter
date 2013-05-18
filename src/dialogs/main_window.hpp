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
#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "ui_main_window.h"
#include <QDoubleSpinBox>
#include "knot_view.hpp"
#include <QUndoView>
#include <QUndoGroup>
#include "export_image_dialog.hpp"
#include "dock_grid.hpp"
#include "dock_knot_display.hpp"
#include "dock_background.hpp"
#include "cusp_style_widget.hpp"
#include "about_dialog.hpp"
#include "dialog_insert_polygon.hpp"
#include <QPrinter>
#include "dialog_plugins.hpp"

class Main_Window : public QMainWindow, private Ui::Main_Window
{
    Q_OBJECT

private:
    QDoubleSpinBox*         zoomer;     ///< Zoom on statusbar
    Knot_View*              view;       ///< Active Knot_View (hopefully never NULL)
    QUndoView*              undo_view;  ///< Action history
    QUndoGroup              undo_group; ///< Groups undo stacks
    Export_Image_Dialog     dialog_export_image;
    Dock_Grid*              dock_grid;  ///< Grid conf dock
    Dock_Knot_Display*      dock_knot_display;///< Display conf dock
    Dock_Background*        dock_background;
    Cusp_Style_Widget*      global_style;
    Cusp_Style_Widget*      selection_style;
    About_Dialog            about_dialog;
    Dialog_Insert_Polygon   dialog_insert_polygon;
    QPrinter                printer;
    Dialog_Plugins          dialog_plugins;

public:
    explicit Main_Window(QWidget *parent = 0);



public slots:
    /// Change all the strings to their translated version
    void retranslate();

    /// Create tab and load given file name
    void create_tab(QString file = QString());

    /**
     *  \brief Switch to the gien tab
     *  \param i tab index
     *  \pre i is a valid index
     */
    void switch_to_tab(int i);

    /**
     *  \brief Close the tab with the given index
     *  \param i Tab index
     *  \param confirm_if_changed Whether to show a dialog asking to save the file
     *  \pre i is a valid index
     *  \post The tab is closed. If it were the last tab, a new one is created
     */
    void close_tab(int i, bool confirm_if_changed = true);

    void print (QPrinter* pr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *) override;
    void closeEvent(QCloseEvent *) override;

private:
    /// Initialize menus
    void init_menus();
    /// Initialize toolbars
    void init_toolbars();
    /// Initialize statusbar
    void init_statusbar();
    /// Initialize dock widgets
    void init_docks();
    /// Load saved configuration
    void load_config();

    /**
     *  \brief Ensure view is connected to the proper signals/slots
    */
    void connect_view(Knot_View* v);
    /**
     *  \brief Ensure view is disconnected connected from the relevant signals/slots
    */
    void disconnect_view(Knot_View* v);

    /**
     * \brief Display a dialog to select unsaved files to be saved
     * \return False if the user does not want to close any more
     */
    bool check_close_all();

private slots:
    void set_icon_size(int);
    /**
     *  \brief Toggle tab icon to show whether the file has been modified
    */
    void set_clean_icon(bool clean);
    /**
     *  \brief Update title to current file and add a *star* if there are unsaved changes
     */
    void update_title();
    void set_undo_text(QString txt);
    void set_redo_text(QString txt);
    void set_tool_button_style(Qt::ToolButtonStyle);
    /**
     *  \brief Sets the current view zoom factor to the value of the slider in the statusbar
    */
    void apply_zoom();

    /**
     *  \brief Update selected style dialog
     */
    void udate_selection(QList<Node*> nodes);

    /**
     * \brief Save design to file
     * \param force_select Whether a file dialog must be shown
     */
    void save(bool force_select, int tab_index);

    void update_mouse_pos(QPointF pos);

    void update_grid_icon(int shape);

    void update_recent_files();
    void click_recent_file();


    void on_action_Preferences_triggered();
    void on_action_Display_Graph_toggled(bool arg1);
    void on_action_Zoom_In_triggered();
    void on_action_Zoom_Out_triggered();
    void on_action_Reset_Zoom_triggered();
    void on_action_Reset_View_triggered();
    void on_action_Report_Bugs_triggered();
    void on_action_Manual_triggered();
    void on_action_Refresh_Path_triggered();
    void on_action_Open_triggered();
    void on_action_Save_triggered();
    void on_action_Save_As_triggered();
    void on_action_Mirror_Horizontal_triggered();
    void on_action_Mirror_Vertical_triggered();
    void on_action_Select_All_triggered();
    void on_actionSelect_Connected_triggered();
    void on_action_Insert_Polygon_triggered();
    void on_action_Copy_triggered();
    void on_action_Paste_triggered();
    void on_action_Cut_triggered();
    void on_tabWidget_dragAway(int tab);
    void on_action_Snap_to_Grid_triggered();
    void on_action_Erase_triggered();
    void on_action_Close_triggered();
    void on_action_Close_All_triggered();
    void on_action_Save_All_triggered();
    void on_action_Connect_triggered();
    void on_action_Disconnect_triggered();
    void on_action_Merge_triggered();
    void on_action_Rotate_triggered(bool checked);
    void on_action_Scale_triggered(bool checked);
    void on_action_Print_triggered();
    void on_action_Page_Setup_triggered();
    void on_action_Print_Preview_triggered();
    void on_action_Configure_Plugins_triggered();
};

#endif // MAIN_WINDOW_HPP
