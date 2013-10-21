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
#include "main_window.hpp"
#include "resource_manager.hpp"
#include <QMessageBox>
#include "dialog_preferences.hpp"
#include <QDockWidget>
#include <QDesktopServices>
#include <QUrl>
#include "icon_dock_style.hpp"
#include <QFileDialog>
#include "xml_exporter.hpp"
#include <QClipboard>
#include <QBuffer>
#include "xml_loader.hpp"
#include <QDrag>
#include <QMouseEvent>
#include "commands.hpp"
#include <QPrintDialog>
#include <QPageSetupDialog>
#include <QPrintPreviewDialog>
#include "dialog_confirm_close.hpp"

Main_Window::Main_Window(QWidget *parent) :
    QMainWindow(parent), zoomer(nullptr), view(nullptr),
    dialog_export_image(this), about_dialog(this),
    dialog_plugins(this)
{
    setupUi(this);
    setWindowIcon(QIcon(Resource_Manager::data("img/icon-small.svg")));

    setWindowTitle(Resource_Manager::program_name());

    init_statusbar();
    init_docks();

    create_tab();

    init_menus();

    load_config();

    // init_toolbars must come load_config in order to configure properly user-defined toolbars
    init_toolbars();


    connect(Resource_Manager::pointer,SIGNAL(language_changed()),this,SLOT(retranslate()));
    connect(this,SIGNAL(destroyed()),Resource_Manager::pointer,SLOT(abort_script()));
    connect(Resource_Manager::pointer,SIGNAL(plugins_changed()),SLOT(update_plugin_menu()));

    foreach(Plugin* p, Resource_Manager::plugins())
    {
        connect(p,SIGNAL(enabled(bool)),SLOT(update_plugin_menu()));
    }


}

void Main_Window::retranslate()
{
    retranslateUi(this);
    retranslate_docks();
    update_title();
}



void Main_Window::init_menus()
{
    // Menu File
    action_New->setShortcut(QKeySequence::New);
    action_Open->setShortcut(QKeySequence::Open);
    action_Save->setShortcut(QKeySequence::Save);
    action_Save_As->setShortcut(QKeySequence::SaveAs);
    action_Close->setShortcut(QKeySequence::Close);
    action_Print->setShortcut(QKeySequence::Print);
    action_Exit->setShortcut(QKeySequence::Quit);
    connect(action_Export,SIGNAL(triggered()),&dialog_export_image,SLOT(show()));
    update_recent_files();

    // Menu Edit
    action_Undo->setShortcut(QKeySequence::Undo);
    action_Redo->setShortcut(QKeySequence::Redo);
    action_Cut->setShortcut(QKeySequence::Cut);
    action_Copy->setShortcut(QKeySequence::Copy);
    action_Paste->setShortcut(QKeySequence::Paste);
    action_Select_All->setShortcut(QKeySequence::SelectAll);
    action_Preferences->setShortcut(QKeySequence::Preferences);


    // Menu View

    action_Zoom_In->setShortcut(QKeySequence::ZoomIn);
    action_Zoom_Out->setShortcut(QKeySequence::ZoomOut);

    // Menu Nodes
    QActionGroup* transform_mode = new QActionGroup(this);
    transform_mode->addAction(action_Scale);
    transform_mode->addAction(action_Rotate);

    QActionGroup* edit_mode = new QActionGroup(this);
    edit_mode->addAction(action_Edit_Graph);
    edit_mode->addAction(action_Edge_Loop);
    edit_mode->addAction(action_Toggle_Edges);

    // Menu Tools
    action_Refresh_Path->setShortcut(QKeySequence::Refresh);

    // Menu Plugins
    update_plugin_menu();

    // Menu Help
    action_Manual->setShortcut(QKeySequence::HelpContents);
    connect(action_About,SIGNAL(triggered()),&about_dialog,SLOT(show()));



}

void Main_Window::init_toolbars()
{
    foreach(QToolBar* tb, findChildren<QToolBar*>())
        menu_Toolbars->insertAction(0,tb->toggleViewAction());
}

void Main_Window::init_statusbar()
{
    zoomer = new QDoubleSpinBox(this);
    zoomer->setMinimum(0.01);
    zoomer->setMaximum(800);
    zoomer->setSuffix("%");
    zoomer->setValue(100);
    statusBar()->addPermanentWidget(new QLabel(tr("Zoom")));
    statusBar()->addPermanentWidget(zoomer);
    connect(zoomer,SIGNAL(valueChanged(double)),this,SLOT(apply_zoom()));
}

void Main_Window::init_docks()
{


    // Edge style
    widget_edge_style = new Crossing_Style_Widget;
    QDockWidget* edge_style_dock = new QDockWidget;
    edge_style_dock->setWidget(widget_edge_style);
    edge_style_dock->setObjectName("edge_style_dock");
    edge_style_dock->setWindowIcon(QIcon::fromTheme("edge-crossing"));
    addDockWidget(Qt::RightDockWidgetArea,edge_style_dock);

    // Node style
    widget_node_style = new Cusp_Style_Widget;
    QDockWidget* node_style_dock = new QDockWidget;
    node_style_dock->setWidget(widget_node_style);
    node_style_dock->setObjectName("node_style_dock");
    node_style_dock->setWindowIcon(QIcon::fromTheme("cusp-pointed"));
    addDockWidget(Qt::RightDockWidgetArea,node_style_dock);
    tabifyDockWidget(edge_style_dock,node_style_dock);

    // Knot Display
    dock_knot_display = new Dock_Knot_Display(this);
    addDockWidget(Qt::RightDockWidgetArea,dock_knot_display);

    // Knot Border
    dock_borders = new Dock_Borders(this);
    addDockWidget(Qt::RightDockWidgetArea,dock_borders);
    tabifyDockWidget(dock_knot_display,dock_borders);

    // Global style
    dock_knot_style = new Dock_Knot_Style;
    addDockWidget(Qt::RightDockWidgetArea,dock_knot_style);
    tabifyDockWidget(dock_borders,dock_knot_style);


    // Background
    dock_background = new Dock_Background(this);
    addDockWidget(Qt::RightDockWidgetArea,dock_background);

    // Grid config
    dock_grid = new Dock_Grid(this);
    addDockWidget(Qt::RightDockWidgetArea,dock_grid);
    menu_Grid->insertAction(0,dock_grid->toggleViewAction());

    // Action History Dock
    undo_view = new QUndoView(&undo_group);
    QDockWidget* undo_dock  = new QDockWidget;
    undo_dock->setWidget(undo_view);
    undo_dock->setObjectName("Action_History");
    undo_dock->setWindowIcon(QIcon::fromTheme("view-history"));
    addDockWidget(Qt::RightDockWidgetArea,undo_dock);
    tabifyDockWidget(undo_dock,dock_grid);
    tabifyDockWidget(dock_grid,dock_background);
    undo_dock->raise();


    connect(&undo_group,SIGNAL(cleanChanged(bool)),SLOT(set_clean_icon(bool)));
    connect(&undo_group,SIGNAL(undoTextChanged(QString)),SLOT(set_undo_text(QString)));
    connect(&undo_group,SIGNAL(redoTextChanged(QString)),SLOT(set_redo_text(QString)));
    connect(&undo_group,SIGNAL(canUndoChanged(bool)),action_Undo,SLOT(setEnabled(bool)));
    connect(&undo_group,SIGNAL(canRedoChanged(bool)),action_Redo,SLOT(setEnabled(bool)));

    connect(action_Undo,SIGNAL(triggered()),&undo_group,SLOT(undo()));
    connect(action_Redo,SIGNAL(triggered()),&undo_group,SLOT(redo()));

    connect(action_Undo,SIGNAL(triggered()),SLOT(update_style()));
    connect(action_Redo,SIGNAL(triggered()),SLOT(update_style()));
    connect(undo_view,SIGNAL(pressed(QModelIndex)),SLOT(update_style()));

    // Plugin log
    dock_script_log = new Dock_Script_Log(this);
    dock_script_log->setFloating(true);
    dock_script_log->hide();
    connect(&dialog_plugins,SIGNAL(edit_plugin(Plugin*)),
            dock_script_log,SLOT(load_plugin(Plugin*)));

    // Menu entries
    foreach(QDockWidget* dw, findChildren<QDockWidget*>())
    {
        QAction *a = dw->toggleViewAction();
        a->setIcon(dw->windowIcon());
        menu_Docks->insertAction(0,a);
        dw->setStyle(new Icon_Dock_Style(dw));
    }

    retranslate_docks();
}

void Main_Window::retranslate_docks()
{
    QDockWidget* undo_dock  = findChild<QDockWidget*>("Action_History");
    undo_dock->setWindowTitle(tr("Action History"));


    QDockWidget* node_style_dock  = findChild<QDockWidget*>("node_style_dock");
    node_style_dock->setWindowTitle(tr("Selected Nodes"));

    QDockWidget* edge_style_dock  = findChild<QDockWidget*>("edge_style_dock");
    edge_style_dock->setWindowTitle(tr("Selected Edges"));

}

void Main_Window::load_config()
{
    connect(&Resource_Manager::settings,SIGNAL(icon_size_changed(int)),this,
            SLOT(set_icon_size(int)));
    connect(&Resource_Manager::settings,
            SIGNAL(tool_button_style_changed(Qt::ToolButtonStyle)),
            this,SLOT(set_tool_button_style(Qt::ToolButtonStyle)));

    if ( !Resource_Manager::settings.least_version(0,9) )
    {
        qWarning() << tr("Warning:")
                   << tr("Discarding old configuration");
        return;
    }
    if ( !Resource_Manager::settings.current_version() )
    {
        int load_old = QMessageBox::question(this,
                    tr("Load old configuration"),
                    tr("Knotter has detected configuration for version %1,\n"
                        "this is version %2.\n"
                        "Do you want to load it anyways?")
                        .arg(Resource_Manager::settings.version())
                        .arg(Resource_Manager::program_version()),
                QMessageBox::Yes,
                QMessageBox::No
        );
        if ( load_old != QMessageBox::Yes)
            return;
    }
    Resource_Manager::settings.initialize_window(this);
}

void Main_Window::connect_view(Knot_View *v)
{
    // set current
    view = v;
    if ( !v ) return;

    // zoom/view
    zoomer->setValue(v->get_zoom_factor()*100);
    connect(v,SIGNAL(zoomed(double)),zoomer,SLOT(setValue(double)));
    connect(action_Fit_View,SIGNAL(triggered()),v,SLOT(view_fit()));

    // statusbar
    connect(v,SIGNAL(mose_position_changed(QPointF)),SLOT(update_mouse_pos(QPointF)));

    // edit mode
    connect(action_Edit_Graph,SIGNAL(triggered()),v,SLOT(set_mode_edit_graph()),
            Qt::UniqueConnection);
    connect(action_Edge_Loop,SIGNAL(triggered()),v,SLOT(set_mode_edge_chain()),
            Qt::UniqueConnection);
    connect(action_Toggle_Edges,SIGNAL(triggered()),v,SLOT(set_mode_toggle_edges()),
            Qt::UniqueConnection);
    action_Edit_Graph->setChecked(v->edit_graph_mode_enabled());
    action_Edge_Loop->setChecked(v->edge_loop_mode_enabled());
    action_Toggle_Edges->setChecked(v->toggle_edges_mode_enabled());

    // undo/redo
    v->undo_stack_pointer()->setActive(true);
    undo_group.setActiveStack(v->undo_stack_pointer());
    //undo_view->setStack(v->undo_stack_pointer());

    // grid editor
    dock_grid->set_grid(&v->grid());
    connect(dock_grid,SIGNAL(move_grid()),v,SLOT(set_mode_move_grid()));
    action_Enable_Grid->setChecked(v->grid().is_enabled());
    connect(action_Enable_Grid,SIGNAL(triggered(bool)),&v->grid(),SLOT(enable(bool)));
    connect(&v->grid(),SIGNAL(enabled(bool)),action_Enable_Grid,SLOT(setChecked(bool)));
    connect(&v->grid(),SIGNAL(shape_changed(int)),SLOT(update_grid_icon(int)));

    // background
    connect(dock_background,SIGNAL(background_changed(QColor)),
            v,SLOT(set_background_color(QColor)));
    connect(dock_background,SIGNAL(image_toogled(bool)),
            &v->background_image(),SLOT(enable(bool)));
    connect(dock_background,SIGNAL(image_scaled(double)),
            &v->background_image(),SLOT(set_scale_percent(double)));
    connect(dock_background,SIGNAL(image_loaded(QString)),
            &v->background_image(),SLOT(load_file(QString)));
    connect(dock_background,SIGNAL(move_image()),
            v,SLOT(set_mode_move_background()));

    // display
    dock_knot_display->set_colors(v->knot_colors());
    dock_knot_display->set_join_style(v->graph().join_style());
    dock_knot_display->set_brush_style(v->graph().brush_style());
    dock_knot_display->set_width(v->graph().width());
    dock_knot_display->toggle_custom_colors(v->graph().custom_colors());
    connect(dock_knot_display,SIGNAL(colors_changed(QList<QColor>)),
            v,SLOT(set_knot_colors(QList<QColor>)));
    connect(dock_knot_display,SIGNAL(width_changed(double)),
            v,SLOT(set_stroke_width(double)) );
    connect(dock_knot_display,SIGNAL(join_style_changed(Qt::PenJoinStyle)),
            v,SLOT(set_join_style(Qt::PenJoinStyle)));
    connect(dock_knot_display,SIGNAL(colors_enabled(bool)),
            v,SLOT(set_knot_custom_colors(bool)));
    connect(dock_knot_display,SIGNAL(brush_style_changed(Qt::BrushStyle)),
            v,SLOT(set_brush_style(Qt::BrushStyle)));
    // border
    dock_borders->set_borders(v->knot_borders());
    dock_borders->enable_borders(v->graph().paint_border());
    connect(dock_borders,SIGNAL(borders_changed(Border_List)),
            v,SLOT(set_knot_borders(Border_List)));
    connect(dock_borders,SIGNAL(borders_enabled(bool)),
            v,SLOT(set_knot_display_border(bool)));

    // global style
    dock_knot_style->set_style(v->graph().default_node_style(),
                                   v->graph().default_edge_style());
    connect(dock_knot_style,SIGNAL(cusp_angle_changed(double)),
            v,SLOT(set_knot_cusp_angle(double)));
    connect(dock_knot_style,SIGNAL(cusp_distance_changed(double)),
            v,SLOT(set_knot_cusp_distance(double)));
    connect(dock_knot_style,SIGNAL(handle_length_changed(double)),
            v,SLOT(set_knot_handle_lenght(double)));
    connect(dock_knot_style,SIGNAL(cusp_shape_changed(Cusp_Shape*)),
            v,SLOT(set_knot_cusp_shape(Cusp_Shape*)));
    connect(dock_knot_style,SIGNAL(crossing_distance_changed(double)),
            v,SLOT(set_knot_crossing_distance(double)));
    connect(dock_knot_style,SIGNAL(crossing_distance_changed(double)),
            v,SLOT(set_knot_crossing_distance(double)));
    connect(dock_knot_style,SIGNAL(edge_slide_changed(double)),
            v,SLOT(set_knot_ege_slide(double)));

    //style
    update_selection(v->selected_nodes(),v->selected_edges());
    connect(v,SIGNAL(selection_changed(QList<Node*>,QList<Edge*>)),
            SLOT(update_selection(QList<Node*>,QList<Edge*>)));
    // node style
    connect(widget_node_style,SIGNAL(cusp_angle_changed(double)),
            v,SLOT(set_selection_cusp_angle(double)));
    connect(widget_node_style,SIGNAL(cusp_distance_changed(double)),
            v,SLOT(set_selection_cusp_distance(double)));
    connect(widget_node_style,SIGNAL(handle_length_changed(double)),
            v,SLOT(set_selection_handle_lenght_nodes(double)));
    connect(widget_node_style,SIGNAL(cusp_shape_changed(Cusp_Shape*)),
            v,SLOT(set_selection_cusp_shape(Cusp_Shape*)));
    connect(widget_node_style,SIGNAL(enabled_styles_changed(Node_Style::Enabled_Styles)),
            v,SLOT(set_selection_enabled_styles_nodes(Node_Style::Enabled_Styles)));
    // edge style
    connect(widget_edge_style,SIGNAL(crossing_distance_changed(double)),
            v,SLOT(set_selection_crossing_distance(double)));
    connect(widget_edge_style,SIGNAL(edge_slide_changed(double)),
            v,SLOT(set_selection_edge_slide(double)));
    connect(widget_edge_style,SIGNAL(edge_type_changed(Edge_Type*)),
            v,SLOT(set_selection_edge_type(Edge_Type*)));
    connect(widget_edge_style,SIGNAL(handle_length_changed(double)),
            v,SLOT(set_selection_handle_lenght_edges(double)));
    connect(widget_edge_style,SIGNAL(enabled_styles_changed(Edge_Style::Enabled_Styles)),
            v,SLOT(set_selection_enabled_styles_edges(Edge_Style::Enabled_Styles)));

    //export
    dialog_export_image.set_view(v);

    // modes
    v->set_display_graph(action_Display_Graph->isChecked());
    action_Rotate->setChecked(v->transform_mode() == Transform_Handle::ROTATE);
    action_Scale->setChecked(v->transform_mode() == Transform_Handle::SCALE);

    // Performance
    v->set_fluid_refresh(Resource_Manager::settings.fluid_refresh());
    v->enable_cache(Resource_Manager::settings.graph_cache());
    v->set_antialiasing(Resource_Manager::settings.antialiasing());

}


void Main_Window::update_style()
{
    // display
    dock_knot_display->blockSignals(true);
    dock_knot_display->set_colors(view->knot_colors());
    dock_knot_display->set_join_style(view->graph().join_style());
    dock_knot_display->set_brush_style(view->graph().brush_style());
    dock_knot_display->set_width(view->graph().width());
    dock_knot_display->toggle_custom_colors(view->graph().custom_colors());
    dock_knot_display->blockSignals(false);

    // border
    dock_borders->blockSignals(true);
    dock_borders->set_borders(view->knot_borders());
    dock_borders->enable_borders(view->graph().paint_border());
    dock_borders->blockSignals(false);

    // style
    dock_knot_style->blockSignals(true);
    dock_knot_style->set_style(view->graph().default_node_style(),
                                   view->graph().default_edge_style());
    dock_knot_style->blockSignals(false);

}

void Main_Window::disconnect_view(Knot_View *v)
{
    if ( v != nullptr )
    {
        disconnect(v);

        disconnect(v,SIGNAL(zoomed(double)),zoomer,SLOT(setValue(double)));
        disconnect(action_Fit_View,SIGNAL(triggered()),v,SLOT(view_fit()));

        dock_grid->disconnect(v);
        dock_grid->unset_grid(&v->grid());
        v->grid().disconnect(this);
        v->grid().disconnect(action_Enable_Grid);
        action_Enable_Grid->disconnect(&v->grid());

        dock_background->disconnect(v);
        dock_background->disconnect(&v->background_image());

        dock_knot_display->disconnect(v);
        dock_borders->disconnect(v);

        dock_knot_style->disconnect(v);

        widget_node_style->disconnect(v);
        widget_edge_style->disconnect(v);
        update_selection(QList<Node*>(),QList<Edge*>());
    }
}

void Main_Window::set_icon_size(int sz)
{
    setIconSize(QSize(sz,sz));
}

void Main_Window::set_clean_icon(bool clean)
{
    tabWidget->setTabIcon(tabWidget->currentIndex(),clean ?
                              QIcon() : QIcon::fromTheme("document-save"));
    update_title();
}

void Main_Window::update_title()
{
    if ( !view )
    {
        setWindowTitle(Resource_Manager::program_name());
        return;
    }

    bool clean = view->undo_stack_pointer()->isClean();
    QString filename = view->windowFilePath();
    if ( filename.isEmpty() )
        filename = tr("New Knot");

    /*: Main window title
     *  %1 is the program name
     *  %2 is the file name
     *  %3 is a star * or an empty string depending on whether the file was modified
     */
    setWindowTitle(tr("%1 - %2%3").arg(Resource_Manager::program_name())
                   .arg(filename).arg(clean?"":"*"));
}

void Main_Window::set_tool_button_style(Qt::ToolButtonStyle tbs)
{
    setToolButtonStyle(tbs);
    dock_script_log->set_tool_button_style(tbs);
}

void Main_Window::apply_zoom()
{
    view->set_zoom(zoomer->value()/100);
}

void Main_Window::update_selection(QList<Node *> nodes, QList<Edge*> edges)
{
    widget_node_style->setEnabled(!nodes.isEmpty());
    widget_edge_style->setEnabled(!edges.isEmpty());

    widget_node_style->blockSignals(true);

    Node_Style ns = view->graph().default_node_style();
    widget_node_style->set_style(ns); // set defaults

    if ( nodes.isEmpty() )
        ns.enabled_style = Node_Style::NOTHING;
    else
    {
        ns = nodes[0]->style();
    }

    widget_node_style->set_style(ns); // set actual style


    widget_node_style->blockSignals(false);


    widget_edge_style->blockSignals(true);

    Edge_Style es = view->graph().default_edge_style();
    widget_edge_style->set_style(es); // set defaults

    if ( edges.isEmpty() )
        es.enabled_style = Edge_Style::NOTHING;
    else
    {
        es = edges[0]->style();
    }

    widget_edge_style->set_style(es); // set actual style


    widget_edge_style->blockSignals(false);
}


void Main_Window::on_action_Preferences_triggered()
{
    Dialog_Preferences(this).exec();
    view->set_fluid_refresh(Resource_Manager::settings.fluid_refresh());
    view->enable_cache(Resource_Manager::settings.graph_cache());
    view->set_antialiasing(Resource_Manager::settings.antialiasing());
    update_recent_files();
}


bool Main_Window::create_tab(QString file)
{
    bool error = false;
    if ( view && view->file_name().isEmpty() &&
             !view->undo_stack_pointer()->canUndo() &&
             !view->undo_stack_pointer()->canRedo() )
    {
        error = !view->load_file(file);
        if ( !error )
        {
            dock_knot_style->blockSignals(true);
            dock_knot_style->set_style(view->graph().default_node_style(),
                                           view->graph().default_edge_style());
            dock_knot_style->blockSignals(false);


            dock_knot_display->blockSignals(true);
            dock_knot_display->set_colors(view->graph().colors());
            dock_knot_display->set_join_style(view->graph().join_style());
            dock_knot_display->set_brush_style(view->graph().brush_style());
            dock_knot_display->set_width(view->graph().width());
            dock_knot_display->blockSignals(false);

            dock_borders->blockSignals(true);
            dock_borders->set_borders(view->knot_borders());
            dock_borders->enable_borders(view->graph().paint_border());
            dock_borders->blockSignals(false);

            dialog_export_image.set_view(view);

            update_title();

            tabWidget->setTabText(tabWidget->currentIndex(),view->file_name());
        }
    }
    else
    {
        if ( view != nullptr )
            Resource_Manager::settings.set_knot_style(view->graph());
        Knot_View *v = new Knot_View();
        error = !v->load_file(file);
        int t = tabWidget->addTab(v,file.isEmpty() ? tr("New Knot") : file);
        undo_group.addStack(v->undo_stack_pointer());
        if ( t != tabWidget->currentIndex() )
            switch_to_tab(t);
    }

    if ( error && !file.isEmpty() )
    {
        QMessageBox::warning(this,tr("File Error"),
                             tr("Error while reading \"%1\".").arg(file));
        return false;
    }
    else if ( !error && !file.isEmpty() )
    {
        Resource_Manager::settings.add_recent_file(file);
        update_recent_files();
    }


    view->grid().set_shape(Resource_Manager::settings.grid_shape());
    view->grid().set_size(Resource_Manager::settings.grid_size());
    view->grid().enable(Resource_Manager::settings.grid_enabled());

    return true;
}

void Main_Window::switch_to_tab(int i)
{
    tabWidget->setCurrentIndex(i);
    /*setWindowTitle(tr("%1 - %2").arg(Resource_Manager::program_name())
                   .arg(tabWidget->tabText(i)));*/
    disconnect_view(view);
    connect_view(dynamic_cast<Knot_View*>(tabWidget->currentWidget()));
    update_title();
}

void Main_Window::close_tab(int i, bool confirm_if_changed)
{
    Knot_View* kv = dynamic_cast<Knot_View*>(tabWidget->widget(i));
    if ( kv )
    {
        if ( Resource_Manager::settings.check_unsaved_files() &&
             confirm_if_changed && !kv->undo_stack_pointer()->isClean() )
        {
            int r = QMessageBox::question(this,tr("Close File"),
                    tr("The file \"%1\" has been modified.\nDo you want to save changes?")
                        .arg(tabWidget->tabText(i)),
                    QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel
                    );
            if ( r == QMessageBox::Yes )
                save(false,i);
            else if ( r == QMessageBox::Cancel )
                return;
        }

        if ( kv == view )
        {
            disconnect_view(kv);
            view = nullptr;
        }


        undo_group.removeStack(kv->undo_stack_pointer());

        emit tab_closing(kv);

        delete kv;
    }
    //tabWidget->removeTab(i);
    if ( tabWidget->count() == 0 )
        create_tab();
}


void Main_Window::set_undo_text(QString txt)
{
    action_Undo->setText(tr("Undo %1").arg(txt));
}

void Main_Window::set_redo_text(QString txt)
{
    action_Redo->setText(tr("Redo %1").arg(txt));
}


void Main_Window::on_action_Zoom_In_triggered()
{
    view->zoom_view(1.25);
}

void Main_Window::on_action_Zoom_Out_triggered()
{
     view->zoom_view(0.8);
}

void Main_Window::on_action_Reset_Zoom_triggered()
{
    view->set_zoom(1);
}

void Main_Window::on_action_Reset_View_triggered()
{
    view->reset_view();
}

void Main_Window::on_action_Report_Bugs_triggered()
{
    QDesktopServices::openUrl(QUrl(BUG_URL));
}

void Main_Window::on_action_Manual_triggered()
{
    QDesktopServices::openUrl(QUrl(DOC_URL));
}

void Main_Window::on_action_Refresh_Path_triggered()
{
    view->update_knot();
}

void Main_Window::on_action_Open_triggered()
{

    QStringList files = QFileDialog::getOpenFileNames(this,tr("Open Knot"),
                view->file_name(),
                "Knot files (*.knot);;XML files (*.xml);;All files (*)" );

    foreach ( QString file, files )
        create_tab(file);
}

void Main_Window::on_action_Save_triggered()
{
    save(false,tabWidget->currentIndex());
}

void Main_Window::on_action_Save_As_triggered()
{
    save(true,tabWidget->currentIndex());
}


void Main_Window::save(bool force_select, int tab_index)
{
    Knot_View *v = qobject_cast<Knot_View*>(tabWidget->widget(tab_index));

    if ( !v )
        return;

    QString file = v->file_name();
    if ( file.isEmpty() || force_select )
    {
        QString selected_filter;
        QString filters = tr("Knot files (*.knot);;XML files (*.xml);;All files (*)");
        file = QFileDialog::getSaveFileName(this,tr("Save Knot"),
                    v->file_name(), filters, &selected_filter
                );

        QFileInfo finfo(file);
        if ( !file.isEmpty() && finfo.suffix().isEmpty() && filters.startsWith(selected_filter))
            file += ".knot";
    }
    if ( !file.isEmpty() )
    {
        if ( v->save_file(file) )
        {
            update_title();
            tabWidget->setTabText(tab_index,v->windowFilePath());

            Resource_Manager::settings.add_recent_file(file);
            update_recent_files();
        }
        else
            QMessageBox::warning(this,tr("File Error"),
                    tr("Failed to save file \"%1\".").arg(file) );
    }
}

void Main_Window::update_mouse_pos(QPointF pos)
{
    //: Displaying mouse position, %1 = x, %2 = y
    statusBar()->showMessage(tr("(%1,%2)").arg(pos.x()).arg(pos.y()));
}

void Main_Window::update_grid_icon(int shape)
{
    QAction* act = findChild<QAction*>("action_Enable_Grid");
    if ( !act )
        return;
    if ( shape == Snapping_Grid::TRIANGLE1 )
        act->setIcon(QIcon::fromTheme("grid-triangle-h"));
    else if ( shape == Snapping_Grid::TRIANGLE2 )
        act->setIcon(QIcon::fromTheme("grid-triangle-v"));
    else
        act->setIcon(QIcon::fromTheme("grid-square"));
}

void Main_Window::update_recent_files()
{
    menu_Open_Recent->clear();

    if ( Resource_Manager::settings.recent_files().empty() )
        menu_Open_Recent->addAction(tr("No recent files"))->setEnabled(false);
    else
        foreach ( QString savefile, Resource_Manager::settings.recent_files() )
        {
            QAction *a = menu_Open_Recent->addAction(
                        QIcon(Resource_Manager::data("img/icon-small.svg")),
                        savefile);
            connect(a, SIGNAL(triggered()), this, SLOT(click_recent_file()));
        }
}

void Main_Window::click_recent_file()
{

    QAction*a = qobject_cast<QAction*>(sender());
    if ( a )
        create_tab(a->text());
}

void Main_Window::update_plugin_menu()
{
    foreach(QAction*a, menu_Plugins->actions() )
    {
        if ( a->menu() )
            delete a;
    }
    QAction * before = menu_Plugins->actions().front();


    QMap<QString,QMenu*> categories;
    foreach(Plugin* p, Resource_Manager::active_plugins(Plugin::Script) )
    {
        QMenu* m;
        QString cat = p->string_data("category");
        if ( !categories.contains(cat) )
        {
            m = new QMenu(cat);
            m->setIcon(QIcon::fromTheme("preferences-plugin-script"));
            menu_Plugins->insertMenu(before,m);
            categories[cat] = m;
        }
        else
            m = categories[cat];

        QAction* a = new QAction(
            p->icon().isNull() ? QIcon::fromTheme("text-x-script") : p->icon(),
            p->string_data("name"),
            m
        );
        a->setToolTip(p->string_data("description"));
        a->setData(QVariant::fromValue(p));

        m->insertAction(nullptr,a);

        p->set_widget_parent(this);
        connect(a,SIGNAL(triggered()),SLOT(execute_plugin()));

    }
}

void Main_Window::execute_plugin()
{
    QAction* a = qobject_cast<QAction*>(sender());
    if ( a )
    {
        Plugin* p = a->data().value<Plugin*>();
        if ( p )
        {
            /*Script_Document doc(view);
            Script_Window win(this);
            Resource_Manager::script_param("document",&doc);
            Resource_Manager::script_param("window",&win);*/
            p->execute(this);
        }
    }
}

void Main_Window::on_action_Mirror_Horizontal_triggered()
{
    view->flip_horiz_selection();
}

void Main_Window::on_action_Mirror_Vertical_triggered()
{
    view->flip_vert_selection();
}

void Main_Window::on_action_Select_All_triggered()
{
    foreach(Node* node,view->graph().nodes())
        node->setSelected(true);

    view->update_selection();
}

void Main_Window::on_actionSelect_Connected_triggered()
{
    QList<Node*> nodes = view->selected_nodes();
    while ( !nodes.empty() )
    {
        Node* n1 = nodes.front();
        nodes.pop_front();
        foreach(Edge* e, n1->edges() )
        {
            Node* n2 = e->other(n1);
            if ( !n2->isSelected() )
            {
                n2->setSelected(true);
                nodes.push_back(n2);
            }
        }
    }
    view->update_selection();
}

void Main_Window::on_action_Display_Graph_toggled(bool checked)
{
    action_Display_Graph->setIcon(QIcon::fromTheme(
                                   checked ? "knot-graph-on" : "knot-graph-off"
                                   ));
    view->set_display_graph(checked);
}

void Main_Window::on_action_Copy_triggered()
{
    Graph copy = view->graph().sub_graph(view->selected_nodes());
    QMimeData* mime_data = new QMimeData;
    export_xml_mime_data(mime_data,copy);
    QApplication::clipboard()->setMimeData(mime_data);
}

void Main_Window::on_action_Paste_triggered()
{
    const QMimeData *mimeData = QApplication::clipboard()->mimeData();

    Graph graph;

    QByteArray clip_data;

    if ( mimeData->hasFormat("application/x-knotter") )
    {
        clip_data = mimeData->data("application/x-knotter");
    }
    else if ( mimeData->hasFormat("text/xml") )
    {
        clip_data = mimeData->data("text/xml");
    }
    else
        return; // invalid data MIME type

    QBuffer read_data(&clip_data);

    if ( !import_xml(read_data,graph) )
        return; // invalid data

    if ( ! view->insert(graph, tr("Paste") ) )
    {
        foreach(Node* n, graph.nodes())
            delete n;
        foreach(Edge* e, graph.edges())
            delete e;
    }
}

void Main_Window::on_action_Cut_triggered()
{
    on_action_Copy_triggered();
    view->begin_macro(tr("Cut"));
    foreach(Node*n, view->selected_nodes() )
        view->remove_node(n);
    view->end_macro();
}


void Main_Window::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list") ||
        event->mimeData()->hasFormat("application/x-knotter") )
        event->acceptProposedAction();
}

void Main_Window::on_tabWidget_dragAway(int tab)
{
    Knot_View* v = dynamic_cast<Knot_View*>(tabWidget->widget(tab));
    if ( v )
    {
        const Graph& graph = v->graph();
        QMimeData *data = new QMimeData;

        export_xml_mime_data(data,graph);

        QDrag* drag = new QDrag(this);
        drag->setMimeData(data);
        drag->exec();

    }
}

void Main_Window::dropEvent(QDropEvent *event)
{
    if ( event->mimeData()->hasUrls() )
    {
        foreach ( QUrl url, event->mimeData()->urls() )
        {
#ifdef HAS_QT_4_8
            if ( url.isLocalFile() )
#endif
            {
                create_tab(url.toLocalFile());
            }
        }
    }
    if ( event->mimeData()->hasFormat("application/x-knotter") )
    {
        create_tab();

        QByteArray clip_data = event->mimeData()->data("application/x-knotter");
        QBuffer read_data(&clip_data);
        view->load_file(read_data,tr("Drop"));
    }
}

void Main_Window::closeEvent(QCloseEvent * ev)
{
    if ( !check_close_all() )
    {
        ev->ignore();
        return;
    }

    Resource_Manager::settings.save_window(this);
    Resource_Manager::settings.set_knot_style(view->graph());
    QMainWindow::closeEvent(ev);
}

void Main_Window::on_action_Snap_to_Grid_triggered()
{
    view->begin_macro(tr("Snap to Grid"));
    foreach(Node*node, view->selected_nodes())
    {
        view->push_command(new Move_Node(node,node->pos(),
                                         view->grid().nearest(node->pos()),view));
    }

    view->end_macro();
}

void Main_Window::on_action_Erase_triggered()
{
    QList<Node*> nodes = view->selected_nodes();
    if ( !nodes.empty() )
    {
        view->begin_macro(tr("Delete"));
        foreach(Node*n, nodes )
            view->remove_node(n);
        view->end_macro();
    }
}

void Main_Window::on_action_Close_triggered()
{
    close_tab(tabWidget->currentIndex(),true);
}

void Main_Window::on_action_Close_All_triggered()
{
    if ( check_close_all() )
        for ( int i = 0, c = tabWidget->count(); i < c ; i++ )
            close_tab(0,false);
}



bool Main_Window::check_close_all()
{
    if ( Resource_Manager::settings.check_unsaved_files() )
    {
        Dialog_Confirm_Close dialog;

        for ( int i = 0; i < tabWidget->count(); i++ )
        {
            Knot_View* kv = dynamic_cast<Knot_View*>(tabWidget->widget(i));
            if ( kv  && !kv->undo_stack_pointer()->isClean() )
                dialog.add_file(i,tabWidget->tabText(i));
        }

        if ( dialog.has_files() )
        {

            int r = dialog.exec();
            if ( r == Dialog_Confirm_Close::Rejected )
                return false; // Don't close
            else if ( r == Dialog_Confirm_Close::Accepted )
            {
                foreach( int i, dialog.save_files() )
                {
                    save(false,i);
                }
            }
        }
    }
    return true;
}

void Main_Window::on_action_Save_All_triggered()
{
    for ( int i = 0; i < tabWidget->count(); i++ )
        save(false,i);
}

void Main_Window::on_action_Connect_triggered()
{
    QList<Node*> nodes = view->selected_nodes();

    if ( nodes.size() < 2 )
        return;

    view->begin_macro(tr("Connect Nodes"));

    for(QList<Node*>::iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        for(QList<Node*>::iterator j = i+1; j != nodes.end(); j++)
            if ( !(*i)->has_edge_to(*j) )
                view->add_edge(*i,*j);
    }
    view->end_macro();
}

void Main_Window::on_action_Disconnect_triggered()
{

    QList<Node*> nodes = view->selected_nodes();

    if ( nodes.size() < 2 )
        return;

    view->begin_macro(tr("Disconnect Nodes"));
    for(QList<Node*>::iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        for(QList<Node*>::iterator j = i+1; j != nodes.end(); j++)
            if ( (*i)->has_edge_to(*j) )
                view->remove_edge((*i)->edge_to(*j));
    }
    view->end_macro();
}

void Main_Window::on_action_Merge_triggered()
{

    QList<Node*> nodes = view->selected_nodes();

    if ( nodes.size() < 2 )
        return;

    view->begin_macro(tr("Merge Nodes"));

    QPointF pos;
    QList<Node*> outlinks;
    QList<Edge_Type*> outlinks_styles;

    for(QList<Node*>::iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        pos += (*i)->pos() / nodes.size();
        foreach(Edge* e, (*i)->edges() )
            if ( !nodes.contains(e->other(*i)) && !outlinks.contains(e->other(*i)) )
            {
                outlinks.push_back(e->other(*i));
                outlinks_styles.push_back(e->style().edge_type);
            }
        view->remove_node(*i);
    }

    Node* newn = view->add_node(pos);
    for(int i = 0; i < outlinks.size(); i++ )
        view->push_command(new Create_Edge(
                        new Edge(newn,outlinks[i],outlinks_styles[i]),view));

    view->end_macro();
}

void Main_Window::on_action_Rotate_triggered(bool checked)
{
    if ( checked )
        view->set_transform_mode(Transform_Handle::ROTATE);
}

void Main_Window::on_action_Scale_triggered(bool checked)
{

    if ( checked )
        view->set_transform_mode(Transform_Handle::SCALE);
}


void Main_Window::print(QPrinter *pr)
{
    QPainter painter(pr);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);
    painter.scale(view->get_zoom_factor(),view->get_zoom_factor());
    painter.translate(-view->mapToScene(0,0));
    view->graph().const_paint(&painter);
}

void Main_Window::on_action_Print_triggered()
{
    QPrintDialog dialog(&printer, this);
    if (dialog.exec())
    {
        print(&printer);
    }
}

void Main_Window::on_action_Page_Setup_triggered()
{
    QPageSetupDialog(&printer,this).exec();
}

void Main_Window::on_action_Print_Preview_triggered()
{
    QPrintPreviewDialog dialog(&printer,this);
    connect(&dialog,SIGNAL(paintRequested(QPrinter*)),SLOT(print(QPrinter*)));
    dialog.exec();
}

void Main_Window::on_action_Configure_Plugins_triggered()
{
    dialog_plugins.show();
}


Knot_View *Main_Window::view_at(int n)
{
    return dynamic_cast<Knot_View*>(tabWidget->widget(n));
}
