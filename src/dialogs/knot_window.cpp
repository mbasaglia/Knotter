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
#include "knot_window.hpp"
#include "commands.hpp"
#include <QFileDialog>
#include <QMessageBox>
#include "gridconfig.hpp"
#include "error_recovery.hpp"
#include <QSvgGenerator>
#include <QSettings>
#include <QDockWidget>
#include "node_pref_dialog.hpp"
#include <QInputDialog>
#include "resource_loader.hpp"
#include <QtDebug>
#include "translator.hpp"

Knot_Window::Knot_Window(KnotGraph *graph, QWidget *parent) :
    QMainWindow(parent), save_ui ( true ), max_recent_files(5),
    save_toolbars(true), save_style(false), save_anything(true)
{

// UI from designer
    setupUi(this);
    setWindowIcon(load::icon("knotter-logo-small"));

// enable error recovery
    ErrorRecovery::recover = canvas;

// KnotView signals / context menus
    connect(&canvas->get_undo_stack(),SIGNAL(cleanChanged(bool)),SLOT(update_title(bool)));
    node_context_menu.set_view(canvas);
    node_context_menu.connect(canvas,SIGNAL(context_menu(Node*)),SLOT(activate(Node*)));
    connect(&node_context_menu,SIGNAL(request_properties(Node*)),SLOT(show_node_prefs(Node*)));
    edge_context_menu.set_view(canvas);
    edge_context_menu.connect(canvas,SIGNAL(context_menu(Edge*)),SLOT(activate(Edge*)));


// translation
    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));

// Misc UI initialization
    init_menus();
    init_toolbars();
    init_docks();


// Load config
    load_config();
    update_ui();

    init_dialogs(); // keep this as last


    if(graph)
    {
        canvas->load_graph(*graph);
        update_ui();
    }
}

Knot_Window::~Knot_Window()
{
    save_config();
}


void Knot_Window::update_ui()
{
    default_node_style_form->set_style_info(canvas->get_default_style());

    global_style_frm->blockSignals(true);
    global_style_frm->set_join_style(canvas->get_join_style());
    global_style_frm->set_knot_color(canvas->get_brush().color());
    global_style_frm->set_knot_width(canvas->get_width());
    global_style_frm->set_pen(canvas->get_pen());
    global_style_frm->blockSignals(false);


    menu_Toolbars->clear();
    foreach(QToolBar *toolb,findChildren<QToolBar *>())
        menu_Toolbars->addAction(toolb->toggleViewAction());

    update_grid_icon();

    update_recent_menu();


}

void Knot_Window::init_menus()
{

// File menu icons/shortcuts
    action_New->setIcon(load::icon("document-new"));
    action_New->setShortcuts(QKeySequence::New);
    action_Save->setIcon(load::icon("document-save"));
    action_Save->setShortcuts(QKeySequence::Save);
    actionSave_As->setIcon(load::icon("document-save-as"));
    actionSave_As->setShortcuts(QKeySequence::SaveAs);
    action_Open->setIcon(load::icon("document-open"));
    action_Open->setShortcuts(QKeySequence::Open);
    action_Quit->setIcon(load::icon("application-exit"));
    action_Quit->setShortcuts(QKeySequence::Quit);
    action_Export->setIcon(load::icon("document-export"));
    menuOpen_Recent->setIcon(load::icon("document-open-recent"));

// undo/redo
    QAction *undos = canvas->get_undo_stack().createUndoAction(this, tr("&Undo"));
    menu_Edit->insertAction(action_Undo,undos);
    MainToolBar->insertAction(action_Undo,undos);
    delete action_Undo;
    action_Undo = undos;
    undos->setObjectName("action_Undo");
    QAction *redos =  canvas->get_undo_stack().createRedoAction(this, tr("&Redo"));
    menu_Edit->insertAction(action_Redo,redos);
    MainToolBar->insertAction(action_Redo,redos);
    delete action_Redo;
    action_Redo = redos;
    redos->setObjectName("action_Redo");

// Edit menu icons/shortcuts
    action_Copy->setIcon(load::icon("edit-copy"));
    action_Copy->setShortcuts(QKeySequence::Copy);
    action_Paste->setIcon(load::icon("edit-paste"));
    action_Paste->setShortcuts(QKeySequence::Paste);
    actionCut->setIcon(load::icon("edit-cut"));
    actionCut->setShortcuts(QKeySequence::Cut);

    action_Undo->setIcon(load::icon("edit-undo"));
    action_Undo->setShortcuts(QKeySequence::Undo);
    action_Redo->setIcon(load::icon("edit-redo"));
    action_Redo->setShortcuts(QKeySequence::Redo);
    actionSelect_All->setIcon(load::icon("edit-select-all"));
    actionSelect_All->setShortcuts(QKeySequence::SelectAll);

    action_Preferences->setIcon(load::icon("preferences-other"));

// View menu icons/shortcuts
    actionZoom_In->setIcon(load::icon("zoom-in"));
    actionZoom_In->setShortcuts(QKeySequence::ZoomIn);
    actionZoom_Out->setIcon(load::icon("zoom-out"));
    actionZoom_Out->setShortcuts(QKeySequence::ZoomOut);
    action_Reset_Zoom->setIcon(load::icon("zoom-original"));
    actionRefresh_Path->setIcon(load::icon("view-refresh"));
    actionRefresh_Path->setShortcuts(QKeySequence::Refresh);
    action_Reset_View->setIcon(load::icon("view-restore"));
    actionShow_Graph->setIcon(load::icon("toggle_graph_on"));
    action_Move_Grid->setIcon(load::icon("move_grid"));
    actionConfigure_G_rid->setIcon(load::icon("configure-grid"));

// Node menu icons
    action_Horizontal_Flip->setIcon(load::icon("object-flip-horizontal"));
    action_Vertical_Flip->setIcon(load::icon("object-flip-vertical"));
    action_Link->setIcon(load::icon("format-connect-node"));
    action_Unlink->setIcon(load::icon("format-disconnect-node"));
    action_Erase->setIcon(load::icon("format-remove-node"));
    action_Merge->setIcon(load::icon("format-join-node"));

    actionInsert_Edges->setIcon(load::icon("draw-line"));
    actionEdge_List->setIcon(load::icon("edge_list"));
    actionInsert_Polygon->setIcon(load::icon("draw-polygon"));

// Help menu icons
    action_Manual->setIcon(load::icon("help-contents"));
    action_Manual->setShortcuts(QKeySequence::HelpContents);

}

void Knot_Window::init_docks()
{

// Action History Dock
    undoView = new QUndoView(&canvas->get_undo_stack());
    QDockWidget* undoDock  = new QDockWidget;
    undoDock->setWidget(undoView);
    undoDock->setObjectName("Action_History");
    undoDock->setWindowTitle(tr("Action History"));
    undoDock->show();

// Default Node Style Dock
    QDockWidget*    default_node_style_dock;
    default_node_style_form = new node_style_form;
    canvas->connect(default_node_style_form,SIGNAL(style_changed(styleinfo)),
                    SLOT(set_default_style(styleinfo)));
    default_node_style_dock = new QDockWidget;
    default_node_style_dock->setWidget(default_node_style_form);
    default_node_style_dock->setObjectName("Default_Node_Style");
    default_node_style_dock->setWindowTitle(tr("Default Node Style"));

// Knot Style Dock
    QDockWidget*    global_style_dock;
    global_style_frm = new global_style_form;

    canvas->connect(global_style_frm,SIGNAL(knot_color_changed(QColor)), SLOT(set_brush_color(QColor)));
    canvas->connect(global_style_frm,SIGNAL(knot_color_accepted()), SLOT(accept_brush()));

    canvas->connect(global_style_frm,SIGNAL(join_style_changed(Qt::PenJoinStyle)), SLOT(set_join_style(Qt::PenJoinStyle)));

    canvas->connect(global_style_frm,SIGNAL(knot_width_changed(double)), SLOT(set_width(double)));
    canvas->connect(global_style_frm,SIGNAL(knot_width_accepted()), SLOT(accept_width()));

    canvas->connect(global_style_frm,SIGNAL(pen_changed(QPen)), SLOT(set_pen(QPen)));
    global_style_dock = new QDockWidget;
    global_style_dock->setWidget(global_style_frm);
    global_style_dock->setObjectName("Knot_Style");
    global_style_dock->setWindowTitle(tr("Knot Style"));

// Dock default positioning
    addDockWidget(Qt::RightDockWidgetArea,global_style_dock);
    addDockWidget(Qt::RightDockWidgetArea,default_node_style_dock);

    //undoDock->setFloating(true);
    undoView->setGeometry(0,0,128,128);
    undoView->resize(128,128);
    undoDock->resize(128,128);
    undoView->setMinimumHeight(16);
    addDockWidget(Qt::RightDockWidgetArea,undoDock);
    //undoDock->setFloating(false);

// Dock toogle actions
    menu_Docks->clear();
    menu_Docks->addAction(undoDock->toggleViewAction());
    menu_Docks->addAction(global_style_dock->toggleViewAction());
    menu_Docks->addAction(default_node_style_dock->toggleViewAction());

}

void Knot_Window::init_toolbars()
{

    // overcome bug in code generator from ui file
    addToolBar(Qt::LeftToolBarArea, EditBar);
    addToolBar(Qt::TopToolBarArea, MainToolBar);
    addToolBar(Qt::TopToolBarArea, ViewBar);
}


void Knot_Window::insert_toolbar(QToolBar *tbr)
{
    addToolBar(Qt::TopToolBarArea,tbr);
    menu_Toolbars->addAction(tbr->toggleViewAction());
}


void Knot_Window::init_dialogs()
{

// Export
    export_dialog.set_knot_view(canvas);
    export_dialog.setAttribute(Qt::WA_QuitOnClose, false);

// Cofig
    connect(config_dlg.clear_recent,SIGNAL(clicked()),SLOT(clear_recent_files()));
    connect(&config_dlg,SIGNAL(insert_toolbar(QToolBar*)),SLOT(insert_toolbar(QToolBar*)));

    foreach(QAction *menu,QMainWindow::menuBar()->actions())
        config_dlg.add_menu(menu);
    foreach(QToolBar *toolb,findChildren<QToolBar *>())
        config_dlg.add_toolbar(toolb);
    config_dlg.set_menu(menu_File);
    config_dlg.set_toolbar(MainToolBar);

// Manual
    help_view.setWindowIcon(load::icon("help-contents"));
    help_view.setAttribute(Qt::WA_QuitOnClose, false);

    action_About->setIcon(load::icon("help-about"));


}

void Knot_Window::load_config()
{
    QSettings settings("knotter","knotter");

    QString config_version = settings.value("version",VERSION).toString();
    if ( config_version != VERSION )
    {
        int load_old = QMessageBox::question(this,
                tr("Load old configuration"),
                tr("Knotter has detected configuration for version %1,\n"
                    "this is version %2.\n"
                    "Load old configuration?").arg(config_version).arg(VERSION),
            QMessageBox::Yes,QMessageBox::No
            );
        if ( load_old != QMessageBox::Yes)
            return;
    }

    Translator::object.change_lang_code (
            settings.value("language",Translator::object.current_lang_code()).toString()
        );

    canvas->set_cache_mode ( QGraphicsItem::CacheMode (
                                settings.value("performance/cache",
                                    canvas->get_cache_mode()).toInt()
                            ) );
    canvas->enable_fluid_redraw ( QGraphicsItem::CacheMode (
                                settings.value("performance/fluid_redraw",
                                    canvas->fluid_redraw_enabled()).toBool()
                            ) );


    settings.beginGroup("gui");

    save_toolbars = settings.value("save_toolbars",save_toolbars).toBool();
    if ( save_toolbars )
    {
        int ntoolbars = settings.beginReadArray("toolbar");
        for ( int i = 0; i < ntoolbars; i++ )
        {
            settings.setArrayIndex(i);

            QString name = settings.value("name").toString();
            if ( name.isEmpty() )
            {
                 qWarning() << tr("Warning:")
                            << tr("Not loading toolbar without name");
                continue;
            }

            QToolBar *toolbar = findChild<QToolBar*>(name);
            if ( !toolbar )
            {
                toolbar = new QToolBar(this);
                toolbar->setObjectName(name);
                addToolBar(Qt::TopToolBarArea,toolbar);
            }

            QString title = settings.value("title").toString();
            if ( title.isEmpty() )
                title = name;
            toolbar->setWindowTitle(title);

            /// \todo use this in config dialog as well instead of direct modification
            QStringList items = settings.value("items").toStringList();
            if ( !items.isEmpty() )
            {
                toolbar->clear();
                foreach(const QString&item, items)
                {
                    if ( item.isEmpty() )
                    {
                         qWarning() << tr("Warning:")
                                    << tr("Not loading action without name");
                        continue;
                    }
                    else if ( item == "(separator)" )
                        toolbar->addSeparator();
                    else /// \warning exposing internal action names
                    {
                        QAction* act = findChild<QAction*>(item);
                        if ( act )
                            toolbar->addAction ( act );
                        else
                            qWarning() << tr("Warning:")
                                      << tr("Unknown action %1").arg(item);
                    }
                }
            }
        }
        settings.endArray();//toolbars
    }

    save_ui = settings.value("save",save_ui).toBool();
    if ( save_ui )
    {
        restoreGeometry(settings.value("geometry").toByteArray());
        restoreState(settings.value("state").toByteArray());

        int icon_size = settings.value("icon_size",iconSize().width()).toInt();
        setIconSize(QSize(icon_size,icon_size));
        setToolButtonStyle ( Qt::ToolButtonStyle (
                     settings.value("buttons",toolButtonStyle()).toInt() ) );
    }
    settings.endGroup();//gui

    settings.beginGroup("recent_files");
    recent_files = settings.value("list").toStringList();
    max_recent_files = settings.value("max",max_recent_files).toInt();
    settings.endGroup();// recent_files

    settings.beginGroup("grid");
    snapping_grid &grid = canvas->get_grid();
    grid.enable ( settings.value("enable",grid.is_enabled()).toBool() );
    grid.set_size(settings.value("size",grid.get_size()).toDouble());
    grid.set_shape(snapping_grid::grid_shape(
                    settings.value("type",int(grid.get_shape())).toInt() ));
    settings.endGroup();// grid



    settings.beginGroup("style");
    save_style = settings.value("save",save_style).toBool();
    if ( save_style )
    {
        QPen stroke(canvas->get_brush(),canvas->get_width());
        stroke.setJoinStyle(canvas->get_join_style());
        stroke = settings.value("stroke",stroke).value<QPen>();
        canvas->set_brush(stroke.brush());
        canvas->set_width(stroke.widthF());
        canvas->set_join_style(stroke.joinStyle());


        canvas->set_pen ( settings.value("outline",canvas->get_pen()).value<QPen>() );

        styleinfo si = canvas->get_default_style();
        QString style_name = settings.value("cusp-style",
                                knot_curve_styler::name(si.curve_style)
                            ).toString();
        si.curve_style = knot_curve_styler::idof(style_name);

        si.cusp_angle = settings.value("cusp-min-angle",si.cusp_angle).toDouble();
        si.cusp_distance = settings.value("cusp-distance",si.cusp_distance).toDouble();
        si.crossing_distance = settings.value("gap",si.crossing_distance).toDouble();
        si.handle_length = settings.value("handle-length",si.handle_length).toDouble();

        canvas->set_default_style(si);
    }
    settings.endGroup();


}

void Knot_Window::save_config()
{
    if ( !save_anything )
        return;

    QSettings settings("knotter","knotter");

    settings.setValue("version",VERSION);

    settings.setValue("language",Translator::object.current_lang_code());

    settings.setValue("performance/cache",canvas->get_cache_mode());
    settings.setValue("performance/fluid_redraw",canvas->fluid_redraw_enabled());

    settings.beginGroup("gui");
    if ( save_ui )
    {
        settings.setValue("geometry",saveGeometry());
        settings.setValue("state",saveState());
        settings.setValue("save",true);
        settings.setValue("icon_size",iconSize().width());
        settings.setValue("buttons",toolButtonStyle());
    }
    else
        settings.setValue("save",false);

    settings.setValue("save_toolbars",save_toolbars);
    if ( save_toolbars )
    {
        settings.remove("toolbar");
        settings.beginWriteArray("toolbar");
        int i = 0;
        foreach ( QToolBar *toolbar, findChildren<QToolBar*>() )
        {
            settings.setArrayIndex(i);
            i++;

            settings.setValue("name",toolbar->objectName());
            settings.setValue("title",toolbar->windowTitle());

            QStringList items;
            foreach(QAction* act, toolbar->actions())
            {
                if ( act->isSeparator() )
                    items.push_back("(separator)");
                else
                    items.push_back(act->objectName());
            }
            settings.setValue("items",items);
        }
        settings.endArray();//toolbars
    }
    settings.endGroup();// gui

    settings.beginGroup("recent_files");
    settings.setValue("list",recent_files);
    settings.setValue("max",max_recent_files);
    settings.endGroup();

    settings.beginGroup("grid");
    settings.setValue("enable",canvas->get_grid().is_enabled());
    settings.setValue("size",canvas->get_grid().get_size());
    settings.setValue("type",int(canvas->get_grid().get_shape()));
    settings.endGroup();


    settings.beginGroup("style");
    settings.setValue("save",save_style);
    if ( save_style )
    {
        QPen stroke(canvas->get_brush(),canvas->get_width());
        stroke.setJoinStyle(canvas->get_join_style());
        settings.setValue("stroke",stroke);
        settings.setValue("outline",canvas->get_pen());
        styleinfo si = canvas->get_default_style();
        settings.setValue("cusp-style",knot_curve_styler::name(si.curve_style));
        settings.setValue("cusp-min-angle",si.cusp_angle);
        settings.setValue("cusp-distance",si.cusp_distance);
        settings.setValue("gap",si.crossing_distance);
        settings.setValue("handle-length",si.handle_length);
    }
    settings.endGroup();
}




void Knot_Window::mode_edge_list()
{
    actionInsert_Edges->setChecked(false);
    actionEdge_List->setChecked(true);
    canvas->mode_edge_chain();
}

void Knot_Window::mode_edge()
{
    actionInsert_Edges->setChecked(true);
    actionEdge_List->setChecked(false);
    canvas->mode_edit_node_edge();
}

void Knot_Window::mouse_moved(QPointF p)
{
    statusBar()->showMessage(QString("%1,%2").arg(p.x()).arg(p.y()));
}

void Knot_Window::copy()
{
    clipboard.copy ( canvas );
}

void Knot_Window::cut()
{
    canvas->get_undo_stack().beginMacro(tr("Cut"));

    clipboard.copy ( canvas );

    node_list sel = canvas->selected_nodes();
    foreach ( Node* n, sel )
        canvas->remove_node(n);

    canvas->get_undo_stack().endMacro();
}

void Knot_Window::paste()
{
    clipboard.paste ( canvas, canvas->mapToScene(
                                canvas->mapFromGlobal(QCursor::pos()) ) );
}

void Knot_Window::clear()
{
    canvas->get_undo_stack().beginMacro(tr("Clear Document"));
    filename = "";
    canvas->select_all();
    canvas->erase_selected();
    canvas->get_undo_stack().endMacro();
}


void Knot_Window::save(QString file)
{
    QFile quf(file);
    if ( ! quf.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Could not write to \"%1\".").arg(file));
        return;
    }

    canvas->graph().save_xml(&quf);
    canvas->get_undo_stack().setClean();

    push_recent_file(file);

    quf.close();
}

bool Knot_Window::open(QString file)
{

    QFile quf(file);
    if ( ! quf.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Could not read \"%1\".").arg(file));
        return false;
    }
    filename = file;

    canvas->clear();
    canvas->get_undo_stack().setClean();

    if ( !canvas->graph().load_xml(&quf) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Error while reading \"%1\".").arg(filename));
        return false;
    }
    canvas->reload_graph();
    push_recent_file(filename);
    update_ui();

    return true;
}

KnotView &Knot_Window::knotview()
{
    return *canvas;
}


void Knot_Window::save()
{
    if ( filename == "" )
        return saveAs();
    save(filename);
}

void Knot_Window::saveAs()
{
    QString fn = QFileDialog::getSaveFileName(this,tr("Save Knot"),filename,
                "Knot files (*.knot);;XML files (*.xml);;All files (*)" );
    if ( fn.isNull() )
        return;
    filename = fn;
    save(filename);
}

void Knot_Window::open()
{
    QString fn = QFileDialog::getOpenFileName(this,tr("Open Knot"),filename,
                "Knot files (*.knot);;XML files (*.xml);;All files (*)" );
    if ( fn.isNull() )
        return;

    open(fn);

}


void Knot_Window::export_image()
{
    export_dialog.reset_size();
    export_dialog.show();
}

void Knot_Window::zoom_in()
{
    canvas->zoom(2);
}

void Knot_Window::zoom_out()
{
    canvas->zoom(0.5);
}

void Knot_Window::configure_grid()
{
    snapping_grid &grid = canvas->get_grid();
    GridConfig(&grid,this).exec();

    actionEnable_Grid->setChecked(grid.is_enabled());
    update_grid_icon();
    canvas->redraw(false);

}

void Knot_Window::enable_grid(bool enabled)
{
    canvas->get_grid().enable ( enabled );
    canvas->redraw(false);
}

void Knot_Window::config()
{
    config_dlg.set_icon_size(iconSize());
    config_dlg.set_tool_button_style(toolButtonStyle());
    config_dlg.saveui_check->setChecked(save_ui);
    config_dlg.save_toolbars_check->setChecked(save_toolbars);
    config_dlg.save_style_check->setChecked(save_style);
    config_dlg.max_recent->setValue(max_recent_files);
    config_dlg.cache_mode->setCurrentIndex(canvas->get_cache_mode());
    config_dlg.select_current_language();
    config_dlg.fluid_check->setChecked(canvas->fluid_redraw_enabled());


    if ( !config_dlg.exec() )
        return;

    setToolButtonStyle(config_dlg.get_tool_button_style());
    setIconSize(config_dlg.get_icon_size());

    save_ui = config_dlg.saveui_check->isChecked();
    save_style = config_dlg.save_style_check->isChecked();
    save_toolbars = config_dlg.save_toolbars_check->isChecked();
    save_anything = config_dlg.save_anything;

    max_recent_files = config_dlg.max_recent->value();
    if ( max_recent_files < recent_files.size() )
        recent_files.erase ( recent_files.begin()+max_recent_files, recent_files.end() );
    canvas->set_cache_mode( QGraphicsItem::CacheMode( config_dlg.cache_mode->currentIndex() ) );

    canvas->enable_fluid_redraw(config_dlg.fluid_check->isChecked());

}

void Knot_Window::clear_recent_files()
{
    recent_files.clear();
    update_recent_menu();
}

void Knot_Window::show_help()
{
    help_view.show();
}


void Knot_Window::update_title(bool clean)
{
    setWindowTitle("Knotter - "+(filename.isEmpty()?"knot":filename)+(clean?"":"*"));
}

void Knot_Window::cause_crash()
{
    Node *nil = 0;
    nil->~Node();
}


void Knot_Window::push_recent_file(QString path)
{
    recent_files.push_front(path);
    recent_files.removeDuplicates();
    while ( recent_files.size() > max_recent_files )
        recent_files.pop_back();
    update_recent_menu();
}

void Knot_Window::update_recent_menu()
{
    menuOpen_Recent->clear();

    if ( recent_files.empty() )
        menuOpen_Recent->addAction(tr("No recent files"))->setEnabled(false);
    else
        foreach ( QString savefile, recent_files )
        {
            QAction *a = menuOpen_Recent->addAction(load::icon("knotter-logo-small"), savefile);
            connect(a, SIGNAL(triggered()), this, SLOT(click_recent()));
        }
}

void Knot_Window::update_grid_icon()
{
    snapping_grid::grid_shape gs = canvas->get_grid().get_shape();
    if ( gs == snapping_grid::TRIANGLE1 )
        actionEnable_Grid->setIcon(load::icon("triangular_grid"));
    else if ( gs == snapping_grid::TRIANGLE2 )
        actionEnable_Grid->setIcon(load::icon("triangular_grid2"));
    else
        actionEnable_Grid->setIcon(load::icon("square_grid"));
    actionEnable_Grid->setChecked ( canvas->get_grid().is_enabled() );
}


void Knot_Window::click_recent()
{
    QAction*a = dynamic_cast<QAction*>(sender());
    if ( a )
        open(a->text());
}

void Knot_Window::show_node_prefs(Node *node)
{
    node_pref_dialog(canvas,node).exec();
}


void Knot_Window::on_action_About_triggered()
{
    //QMessageBox::about(this,tr("About Knotter"), BUILD_INFO );
    QMessageBox mb(QMessageBox::Information, tr("About Knotter"), BUILD_INFO );
    //mb.setAttribute(Qt::WA_QuitOnClose, false);
    mb.setWindowIcon(load::icon("help-about"));
    mb.exec();
}

void Knot_Window::on_actionShow_Graph_triggered(bool checked)
{
    if ( checked )
        actionShow_Graph->setIcon(load::icon("toggle_graph_on"));
    else
        actionShow_Graph->setIcon(load::icon("toggle_graph_off"));
    canvas->toggle_graph(checked);
}

void Knot_Window::on_actionInsert_Polygon_triggered()
{
    bool ok;
    int n = QInputDialog::getInt(this,tr("Insert Polygon"),tr("Sides"),4,3,32,1,&ok);
    if ( !ok )
        return;

    canvas->get_undo_stack().beginMacro("Insert Polygon");


    canvas->scene()->clearSelection();

    double radius = canvas->get_grid().is_enabled() ? canvas->get_grid().get_size() : 32;
    QLineF rad ( 0, 0, radius, 0 );
    rad.translate ( canvas->mapToScene(
                                canvas->mapFromGlobal(QCursor::pos()) ) );
    Node *first = NULL;
    Node* prev = NULL;
    Node *last = NULL;
    for( int i = 0; i < n; i++)
    {
        last = canvas->add_node ( QPointF( rad.p2().x()-radius, rad.p2().y() ) );
        last->setSelected(true);
        if ( !first )
            first = last;
        else
            canvas->add_edge(last,prev);
        prev = last;
        rad.setAngle ( rad.angle()+360./n );
    }
    canvas->add_edge(last,first);

    canvas->get_undo_stack().endMacro();

    canvas->mode_moving_new(first->pos());//rad.p1());
}

void Knot_Window::retranslate()
{
    retranslateUi(this);
    undoView->setEmptyLabel(tr("<empty>"));
}
