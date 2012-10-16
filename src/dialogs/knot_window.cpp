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
#include <QClipboard>
#include <QBuffer>
#include <QDropEvent>
#include <QPrintDialog>
#include <QPageSetupDialog>
#include <QPrintPreviewDialog>

Knot_Window::Knot_Window(KnotGraph *graph, QWidget *parent) :
    QMainWindow(parent), save_ui ( true ), max_recent_files(5),
    save_toolbars(true), save_style(false), save_anything(true), canvas(0)
{

// UI from designer
    setupUi(this);
    setWindowIcon(load::icon("knotter-logo-small"));
// translation
    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));

    tabWidget->blockSignals(true);
    new_tab();
    canvas=dynamic_cast<KnotView*>(tabWidget->currentWidget());
    tabWidget->blockSignals(false);

// Misc UI initialization

    init_menus();
    init_toolbars();
    init_docks();

    connect(&node_context_menu,SIGNAL(request_properties(Node*)),SLOT(show_node_prefs(Node*)));

// Load config

    load_config();

    init_dialogs(); // keep this as last


    if(graph)
    {
        knotview()->load_graph(*graph);
    }


    update_ui();
    connect_view();
}

Knot_Window::~Knot_Window()
{
    save_config();
}

void Knot_Window::connect_view()
{

    connect(&knotview()->get_undo_stack(),SIGNAL(cleanChanged(bool)),SLOT(set_clean_icon(bool)));
    connect(&knotview()->get_undo_stack(),SIGNAL(undoTextChanged(QString)),SLOT(set_undo_text(QString)));
    connect(&knotview()->get_undo_stack(),SIGNAL(redoTextChanged(QString)),SLOT(set_redo_text(QString)));
    action_Undo->connect(&(knotview()->get_undo_stack()),SIGNAL(canUndoChanged(bool)),SLOT(setEnabled(bool)));
    action_Redo->connect(&(knotview()->get_undo_stack()),SIGNAL(canRedoChanged(bool)),SLOT(setEnabled(bool)));

    node_context_menu.connect(knotview(),SIGNAL(context_menu(Node*)),SLOT(activate(Node*)));

    edge_context_menu.connect(knotview(),SIGNAL(context_menu(Edge*)),SLOT(activate(Edge*)));

    knotview()->connect(default_node_style_form,SIGNAL(style_changed(styleinfo)),
                    SLOT(set_default_style(styleinfo)));

    knotview()->connect(global_style_frm,SIGNAL(knot_color_changed(QColor)), SLOT(set_brush_color(QColor)));
    knotview()->connect(global_style_frm,SIGNAL(knot_color_accepted()), SLOT(accept_brush()));

    knotview()->connect(global_style_frm,SIGNAL(join_style_changed(Qt::PenJoinStyle)), SLOT(set_join_style(Qt::PenJoinStyle)));

    knotview()->connect(global_style_frm,SIGNAL(knot_width_changed(double)), SLOT(set_width(double)));
    knotview()->connect(global_style_frm,SIGNAL(knot_width_accepted()), SLOT(accept_width()));

    knotview()->connect(global_style_frm,SIGNAL(pen_changed(QPen)), SLOT(set_pen(QPen)));

    zoomer->connect(knotview(),SIGNAL(zoom_changed(double)),SLOT(setValue(double)));
    knotview()->connect(zoomer,SIGNAL(valueChanged(double)),SLOT(set_zoom(double)));

    connect(knotview(),SIGNAL(mouse_moved(QPointF)),SLOT(mouse_moved(QPointF)));
    connect(knotview(),SIGNAL(selectionChanged()),SLOT(enable_selection_action()));

}

void Knot_Window::disconnect_view()
{
    if ( knotview() )
    {
        knotview()->get_undo_stack().disconnect(this);
        knotview()->disconnect(&node_context_menu);
        knotview()->disconnect(&edge_context_menu);
        default_node_style_form->disconnect(knotview());
        global_style_frm->disconnect(knotview());
        zoomer->disconnect(knotview());
        knotview()->disconnect(zoomer);
        knotview()->disconnect(this);
    }
}


void Knot_Window::new_tab( QString file )
{
    int tabindex;
    KnotView *kv = new KnotView(tabWidget);
    if ( file.isEmpty() )
        tabindex = tabWidget->addTab(kv,load::icon("document-save"),tr("New Knot"));
    else
    {

        kv->set_saved(true);
        tabindex = tabWidget->addTab(kv,file);
    }
    ErrorRecovery::insert(&kv->graph());
    tabWidget->setCurrentIndex(tabindex);
    kv->reset_view();
}

void Knot_Window::dropEvent(QDropEvent *event)
{
    if ( event->mimeData()->hasUrls() )
    {
        foreach ( QUrl url, event->mimeData()->urls() )
        {
#ifdef HAS_QT_4_8
            if ( url.isLocalFile() )
#endif
            {
                open(url.toLocalFile());
            }
        }
    }
    if ( event->mimeData()->hasFormat("application/x-knotter") )
    {
        new_tab();

        QByteArray clip_data = event->mimeData()->data("application/x-knotter");
        QBuffer read_data(&clip_data);
        read_data.open(QIODevice::ReadOnly|QIODevice::Text);
        KnotView* kv = knotview();
        kv->graph().load_xml(&read_data);
        kv->reload_graph();
    }
}

void Knot_Window::dragEnterEvent(QDragEnterEvent *event)
{

     if (event->mimeData()->hasFormat("text/uri-list") ||
         event->mimeData()->hasFormat("application/x-knotter") )
         event->acceptProposedAction();
}

KnotView *Knot_Window::canvas_at(int index)
{
    return dynamic_cast<KnotView*>(tabWidget->widget(index));
}



void Knot_Window::drag_tab_away(int tab)
{

    KnotView* kv = canvas_at(tab);

    if ( kv )
    {
        KnotGraph& graph = kv->graph();
        QMimeData *data = new QMimeData;

        graph.to_mime(data);

        QDrag* drag = new QDrag(this);
        drag->setMimeData(data);
        drag->exec();

        //on_tabWidget_tabCloseRequested(tab);
    }
}

void Knot_Window::update_ui()
{

    undoView->setStack(&knotview()->get_undo_stack());

    node_context_menu.set_view(knotview());
    edge_context_menu.set_view(knotview());

    default_node_style_form->blockSignals(true);
    default_node_style_form->set_style_info(knotview()->get_default_style());
    default_node_style_form->blockSignals(false);

    global_style_frm->blockSignals(true);
    global_style_frm->set_join_style(knotview()->get_join_style());
    global_style_frm->set_knot_color(knotview()->get_brush().color());
    global_style_frm->set_knot_width(knotview()->get_width());
    global_style_frm->set_pen(knotview()->get_pen());
    global_style_frm->blockSignals(false);


    export_dialog.set_knot_view(knotview());

    back_config.set_view(knotview());


    menu_Toolbars->clear();
    foreach(QToolBar *toolb,findChildren<QToolBar *>())
        menu_Toolbars->addAction(toolb->toggleViewAction());

    enable_selection_action();

    update_grid_icon();

    update_recent_menu();

    update_title();
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
    actionSave_A_ll->setIcon(load::icon("document-save-all"));
    action_Open->setIcon(load::icon("document-open"));
    action_Open->setShortcuts(QKeySequence::Open);
    action_Quit->setIcon(load::icon("application-exit"));
    action_Quit->setShortcuts(QKeySequence::Quit);
    action_Export->setIcon(load::icon("document-export"));
    menuOpen_Recent->setIcon(load::icon("document-open-recent"));
    action_Print->setIcon(load::icon("document-print"));
    action_Print->setShortcut(QKeySequence::Print);
    actionPr_int_Preview->setIcon(load::icon("document-print-preview"));

    QIcon page_setup = load::icon("document-page-setup");
    if ( page_setup.isNull() )
        page_setup = load::icon("document-properties");
    actionPage_Set_up->setIcon(page_setup);


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
    action_Background->setIcon(load::icon("preferences-desktop-wallpaper"));

// Node menu icons
    action_Horizontal_Flip->setIcon(load::icon("object-flip-horizontal"));
    action_Vertical_Flip->setIcon(load::icon("object-flip-vertical"));
    action_Link->setIcon(load::icon("format-connect-node"));
    action_Unlink->setIcon(load::icon("format-disconnect-node"));
    action_Erase->setIcon(load::icon("format-remove-node"));
    action_Merge->setIcon(load::icon("format-join-node"));

    action_Scale->setIcon(load::icon("transform-scale"));
    action_Rotate->setIcon(load::icon("transform-rotate"));
    QActionGroup* transform_mode = new QActionGroup(this);
    transform_mode->addAction(action_Scale);
    transform_mode->addAction(action_Rotate);

    actionS_nap_to_grid->setIcon(load::icon("snap-grid"));


// Tools menu
    QActionGroup* edit_mode = new QActionGroup(this);
    edit_mode->addAction(actionInsert_Edges);
    edit_mode->addAction(actionEdge_List);
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
    undoView = new QUndoView(&knotview()->get_undo_stack());
    QDockWidget* undoDock  = new QDockWidget;
    undoDock->setWidget(undoView);
    undoDock->setObjectName("Action_History");
    undoDock->setWindowTitle(tr("Action History"));
    undoDock->show();

// Default Node Style Dock
    QDockWidget*    default_node_style_dock;
    default_node_style_form = new node_style_form(0);
    default_node_style_form->global_style_mode();
    default_node_style_dock = new QDockWidget;
    default_node_style_dock->setWidget(default_node_style_form);
    default_node_style_dock->setObjectName("Default_Node_Style");
    default_node_style_dock->setWindowTitle(tr("Default Style"));

//Selected Node Style Dock
    QDockWidget* selected_node_style_dock;
    selected_node_style_form = new node_style_form(0);
    selected_node_style_form->setEnabled(false);
    selected_node_style_dock = new QDockWidget;
    selected_node_style_dock->setWidget(selected_node_style_form);
    selected_node_style_dock->setObjectName("selected_node_style_dock");
    selected_node_style_dock->setWindowTitle(tr("Selection Style"));

    connect(selected_node_style_form,SIGNAL(style_changed(styleinfo)),
                    SLOT(set_selected_style(styleinfo)));

// Knot Style Dock
    QDockWidget*    global_style_dock;
    global_style_frm = new global_style_form;

    global_style_dock = new QDockWidget;
    global_style_dock->setWidget(global_style_frm);
    global_style_dock->setObjectName("Knot_Style");
    global_style_dock->setWindowTitle(tr("Knot Style"));

// Dock default positioning
    addDockWidget(Qt::RightDockWidgetArea,global_style_dock);
    tabifyDockWidget(global_style_dock,default_node_style_dock);
    tabifyDockWidget(default_node_style_dock,selected_node_style_dock);
    global_style_dock->raise();
    //addDockWidget(Qt::RightDockWidgetArea,default_node_style_dock);
    addDockWidget(Qt::RightDockWidgetArea,undoDock);

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
    addToolBar(Qt::TopToolBarArea, TransformBar);

    // Statusbar...
    zoomer = new QDoubleSpinBox;
    zoomer->setMinimum(0.01);
    zoomer->setMaximum(800);
    zoomer->setSuffix("%");
    zoomer->setValue(100);
    statusBar()->addPermanentWidget(new QLabel(tr("Zoom")));
    statusBar()->addPermanentWidget(zoomer);
}


void Knot_Window::insert_toolbar(QToolBar *tbr)
{
    addToolBar(Qt::TopToolBarArea,tbr);
    menu_Toolbars->addAction(tbr->toggleViewAction());
}


void Knot_Window::init_dialogs()
{

// Export
    export_dialog.setAttribute(Qt::WA_QuitOnClose, false);

// Cofig
    connect(config_dlg.clear_recent,SIGNAL(clicked()),SLOT(clear_recent_files()));
    connect(&config_dlg,SIGNAL(insert_toolbar(QToolBar*)),SLOT(insert_toolbar(QToolBar*)));

    foreach(QAction *menu,QMainWindow::menuBar()->actions())
        config_dlg.add_menu(menu);
    config_dlg.add_menu(menu_Zoom->menuAction());

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

    knotview()->set_cache_mode ( QGraphicsItem::CacheMode (
                                settings.value("performance/cache",
                                    knotview()->get_cache_mode()).toInt()
                            ) );
    knotview()->enable_fluid_redraw ( QGraphicsItem::CacheMode (
                                settings.value("performance/fluid_redraw",
                                    knotview()->fluid_redraw_enabled()).toBool()
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
        Transform_Handle::size = icon_size;
        setToolButtonStyle ( Qt::ToolButtonStyle (
                     settings.value("buttons",toolButtonStyle()).toInt() ) );
    }
    settings.endGroup();//gui

    settings.beginGroup("recent_files");
    recent_files = settings.value("list").toStringList();
    max_recent_files = settings.value("max",max_recent_files).toInt();
    settings.endGroup();// recent_files

    settings.beginGroup("grid");
    snapping_grid &grid = knotview()->get_grid();
    grid.enable ( settings.value("enable",grid.is_enabled()).toBool() );
    grid.set_size(settings.value("size",grid.get_size()).toDouble());
    grid.set_shape(snapping_grid::grid_shape(
                    settings.value("type",int(grid.get_shape())).toInt() ));
    settings.endGroup();// grid



    settings.beginGroup("style");
    save_style = settings.value("save",save_style).toBool();
    if ( save_style )
    {
        QPen stroke(knotview()->get_brush(),knotview()->get_width());
        stroke.setJoinStyle(knotview()->get_join_style());
        stroke = settings.value("stroke",stroke).value<QPen>();
        knotview()->set_brush(stroke.brush());
        knotview()->set_width(stroke.widthF());
        knotview()->set_join_style(stroke.joinStyle());


        knotview()->set_pen ( settings.value("outline",knotview()->get_pen()).value<QPen>() );

        styleinfo si = knotview()->get_default_style();
        QString style_name = settings.value("cusp-style",
                                knot_curve_styler::name(si.curve_style)
                            ).toString();
        si.curve_style = knot_curve_styler::idof(style_name);

        si.cusp_angle = settings.value("cusp-min-angle",si.cusp_angle).toDouble();
        si.cusp_distance = settings.value("cusp-distance",si.cusp_distance).toDouble();
        si.crossing_distance = settings.value("gap",si.crossing_distance).toDouble();
        si.handle_length = settings.value("handle-length",si.handle_length).toDouble();

        knotview()->set_default_style(si);
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

    settings.setValue("performance/cache",knotview()->get_cache_mode());
    settings.setValue("performance/fluid_redraw",knotview()->fluid_redraw_enabled());

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
    settings.setValue("enable",knotview()->get_grid().is_enabled());
    settings.setValue("size",knotview()->get_grid().get_size());
    settings.setValue("type",int(knotview()->get_grid().get_shape()));
    settings.endGroup();


    settings.beginGroup("style");
    settings.setValue("save",save_style);
    if ( save_style )
    {
        QPen stroke(knotview()->get_brush(),knotview()->get_width());
        stroke.setJoinStyle(knotview()->get_join_style());
        settings.setValue("stroke",stroke);
        settings.setValue("outline",knotview()->get_pen());
        styleinfo si = knotview()->get_default_style();
        settings.setValue("cusp-style",knot_curve_styler::name(si.curve_style));
        settings.setValue("cusp-min-angle",si.cusp_angle);
        settings.setValue("cusp-distance",si.cusp_distance);
        settings.setValue("gap",si.crossing_distance);
        settings.setValue("handle-length",si.handle_length);
    }
    settings.endGroup();
}


void Knot_Window::set_undo_text(QString txt)
{
    action_Undo->setText(tr("Undo %1").arg(txt));
}

void Knot_Window::set_redo_text(QString txt)
{
    action_Redo->setText(tr("Redo %1").arg(txt));
}

void Knot_Window::enable_selection_action()
{
    node_list selection = knotview()->selected_nodes();
    int n = selection.size();

    action_Horizontal_Flip->setEnabled ( n );
    action_Vertical_Flip->setEnabled ( n );
    actionS_nap_to_grid->setEnabled ( n );
    action_Erase->setEnabled ( n );
    action_Merge->setEnabled ( n >= 2 );
    action_Horizontal_Flip->setEnabled ( n >= 2 );
    action_Vertical_Flip->setEnabled ( n >= 2);
    action_Link->setEnabled ( n >= 2);
    action_Unlink->setEnabled ( n >= 2 );

    selected_node_style_form->blockSignals(true);
    selected_node_style_form->from_multi_nodes(selection, knotview()->get_default_style());
    selected_node_style_form->blockSignals(false);
}

void Knot_Window::set_selected_style(styleinfo si)
{
    node_list selection =  knotview()->selected_nodes();

    if ( !selection.empty() )
    {
        /// \todo add overload that takes only style info
        knotview()->set_custom_style ( selection, si );
    }
}

void Knot_Window::mouse_moved(QPointF p)
{
    statusBar()->showMessage(QString("%1,%2").arg(p.x()).arg(p.y()));
}

void Knot_Window::copy()
{
    node_list insert_nodes = knotview()->selected_nodes();

    if ( !insert_nodes.empty() )
    {
        KnotGraph graph;
        graph.copy_style(knotview()->graph());

        QPointF center;
        foreach(Node* n, insert_nodes)
            center += n->pos();
        center /= insert_nodes.size();

        QMap<Node*,Node*> ids;
        foreach(Node* n, insert_nodes)
        {
            Node* clone = new Node(n->pos()-center);
            if ( n->has_custom_style() )
                clone->set_custom_style(n->get_custom_style());
            ids[n] = clone;
            graph.add_node(clone);
        }

        foreach(Node* n, insert_nodes)
        {
            foreach ( Edge* e, n->links() )
            {
                Node* clone1 = ids.value(e->vertex1(),NULL);
                Node* clone2 = ids.value(e->vertex2(),NULL);
                if ( clone1 == 0 || clone2 == 0 )
                {
                    continue; // edge with a vertex outside the selection
                }
                graph.add_edge(clone1,clone2);
                graph.set_edge_type(clone1,clone2,e->type);
            }
        }

        QMimeData *data = new QMimeData;
        graph.to_mime(data);
        QApplication::clipboard()->setMimeData(data);


        foreach(Node* n, graph.get_nodes())
            delete n;
        foreach(Edge* e, graph.get_edges())
            delete e;
    }

}

void Knot_Window::cut()
{
    knotview()->begin_macro(tr("Cut"));

    copy();

    node_list sel = knotview()->selected_nodes();
    foreach ( Node* n, sel )
        knotview()->remove_node(n);

    knotview()->end_macro();
}

void Knot_Window::paste()
{
    const QMimeData *mimeData = QApplication::clipboard()->mimeData();

    KnotGraph graph;

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
    read_data.open(QIODevice::ReadOnly|QIODevice::Text);
    if ( !graph.load_xml(&read_data) )
        return; // invalid data

    if ( graph.get_nodes().size() < 1 )
        return; // empty graph

    QPointF p = knotview()->mapToScene( knotview()->mapFromGlobal(QCursor::pos()) );

    knotview()->begin_macro(KnotView::tr("Paste"));

    knotview()->scene()->clearSelection();

    foreach(Node* n, graph.get_nodes())
    {
        n->setPos(n->pos()+p);
        knotview()->add_node(n);
        n->setSelected(true);
    }

    foreach(Edge* e, graph.get_edges())
    {
        knotview()->add_edge(e->vertex1(),e->vertex2());
    }

    knotview()->end_macro();

    knotview()->mode_moving_new(graph.get_nodes()[0]->pos());

}

void Knot_Window::clear()
{
    new_tab();
}


void Knot_Window::save(QString file)
{
    QFile quf(file);
    if ( ! quf.open(QIODevice::WriteOnly | QIODevice::Text) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Could not write to \"%1\".").arg(file));
        return;
    }

    knotview()->graph().save_xml(&quf);
    knotview()->get_undo_stack().setClean();
    knotview()->set_saved(true);

    push_recent_file(file);

    quf.close();

    tabWidget->setTabText(tabWidget->currentIndex(),file);
    tabWidget->setTabIcon(tabWidget->currentIndex(),QIcon());

    update_title();
}

bool Knot_Window::open(QString file)
{

    QFile quf(file);
    if ( ! quf.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Could not read \"%1\".").arg(file));
        return false;
    }

    if ( knotview()->get_undo_stack().canRedo() || knotview()->get_undo_stack().canUndo() )
        new_tab(file);
    else
    {
        knotview()->set_saved(true);
        tabWidget->setTabText(tabWidget->currentIndex(),file);
    }

    knotview()->clear();
    knotview()->get_undo_stack().setClean();

    if ( !knotview()->graph().load_xml(&quf) )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Error while reading \"%1\".").arg(file));
        return false;
    }

    knotview()->reload_graph();
    push_recent_file(file);
    update_ui();

    return true;
}

KnotView *Knot_Window::knotview()
{
    return canvas;
}


void Knot_Window::save()
{
    if ( !knotview()->get_saved() )
        return saveAs();
    save(tabWidget->tabText(tabWidget->currentIndex()));
}

void Knot_Window::saveAs()
{

    QString fn = tabWidget->tabText(tabWidget->currentIndex());

    fn = QFileDialog::getSaveFileName(this,tr("Save Knot"),fn,
                "Knot files (*.knot);;XML files (*.xml);;All files (*)" );
    if ( fn.isNull() )
        return;
    save(fn);
}

void Knot_Window::open()
{
    QString fn = tabWidget->tabText(tabWidget->currentIndex());

    fn = QFileDialog::getOpenFileName(this,tr("Open Knot"),fn,
                "Knot files (*.knot);;XML files (*.xml);;All files (*)" );
    if ( fn.isNull() )
        return;

    open(fn);

}


void Knot_Window::export_image()
{
    export_dialog.reset_size();
    export_dialog.show();
    export_dialog.raise();
}

void Knot_Window::zoom_in()
{
    knotview()->zoom(2);
}

void Knot_Window::zoom_out()
{
    knotview()->zoom(0.5);
}

void Knot_Window::configure_grid()
{
    snapping_grid &grid = knotview()->get_grid();
    GridConfig(&grid,this).exec();

    update_grid_icon();

}

void Knot_Window::enable_grid(bool enabled)
{
    knotview()->get_grid().enable ( enabled );
    knotview()->redraw(false);
    update_grid_icon();
}

void Knot_Window::config()
{
    config_dlg.set_icon_size(iconSize());
    config_dlg.set_tool_button_style(toolButtonStyle());
    config_dlg.saveui_check->setChecked(save_ui);
    config_dlg.save_toolbars_check->setChecked(save_toolbars);
    config_dlg.save_style_check->setChecked(save_style);
    config_dlg.max_recent->setValue(max_recent_files);
    config_dlg.cache_mode->setCurrentIndex(knotview()->get_cache_mode());
    config_dlg.select_current_language();
    config_dlg.fluid_check->setChecked(knotview()->fluid_redraw_enabled());


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
    knotview()->set_cache_mode( QGraphicsItem::CacheMode( config_dlg.cache_mode->currentIndex() ) );

    knotview()->enable_fluid_redraw(config_dlg.fluid_check->isChecked());

    Transform_Handle::size = config_dlg.get_icon_size().width();

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


void Knot_Window::update_title()
{
    bool clean = knotview()->get_undo_stack().isClean();
    QString filename = tabWidget->tabText(tabWidget->currentIndex());
    setWindowTitle("Knotter - "+filename+(clean?"":"*"));
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
    snapping_grid::grid_shape gs = knotview()->get_grid().get_shape();
    if ( gs == snapping_grid::TRIANGLE1 )
        actionEnable_Grid->setIcon(load::icon("triangular_grid"));
    else if ( gs == snapping_grid::TRIANGLE2 )
        actionEnable_Grid->setIcon(load::icon("triangular_grid2"));
    else
        actionEnable_Grid->setIcon(load::icon("square_grid"));
    actionEnable_Grid->setChecked ( knotview()->get_grid().is_enabled() );

    actionS_nap_to_grid->setEnabled ( knotview()->get_grid().is_enabled() && knotview()->has_selection() );
}


void Knot_Window::click_recent()
{
    QAction*a = dynamic_cast<QAction*>(sender());
    if ( a )
        open(a->text());
}

void Knot_Window::show_node_prefs(Node *node)
{
    node_pref_dialog(knotview(),node).exec();
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
    knotview()->toggle_graph(checked);
}

void Knot_Window::on_actionInsert_Polygon_triggered()
{
    bool ok;
    int n = QInputDialog::getInt(this,tr("Insert Polygon"),tr("Sides"),4,3,32,1,&ok);
    if ( !ok )
        return;

    knotview()->begin_macro("Insert Polygon");


    knotview()->scene()->clearSelection();

    double radius = knotview()->get_grid().is_enabled() ? knotview()->get_grid().get_size() : 32;
    QLineF rad ( 0, 0, radius, 0 );
    rad.translate ( knotview()->mapToScene(
                                knotview()->mapFromGlobal(QCursor::pos()) ) );
    Node *first = NULL;
    Node* prev = NULL;
    Node *last = NULL;
    for( int i = 0; i < n; i++)
    {
        last = knotview()->add_node ( QPointF( rad.p2().x()-radius, rad.p2().y() ) );
        last->setSelected(true);
        if ( !first )
            first = last;
        else
            knotview()->add_edge(last,prev);
        prev = last;
        rad.setAngle ( rad.angle()+360./n );
    }
    knotview()->add_edge(last,first);

    knotview()->end_macro();

    knotview()->mode_moving_new(first->pos());//rad.p1());
}

void Knot_Window::retranslate()
{
    retranslateUi(this);
    undoView->setEmptyLabel(tr("<empty>"));
}

void Knot_Window::on_action_Background_triggered()
{
    back_config.show();
    back_config.raise();
}

void Knot_Window::set_clean_icon(bool clean)
{
    tabWidget->setTabIcon(tabWidget->currentIndex(),clean?QIcon():load::icon("document-save"));
    update_title();
}

void Knot_Window::on_action_Undo_triggered()
{
    knotview()->get_undo_stack().undo();
}

void Knot_Window::on_action_Redo_triggered()
{
    knotview()->get_undo_stack().redo();
}

void Knot_Window::on_action_Erase_triggered()
{
    knotview()->erase_selected();
}

void Knot_Window::on_action_Merge_triggered()
{
    knotview()->merge_selected();
}

void Knot_Window::on_action_Link_triggered()
{
    knotview()->link_selected();
}

void Knot_Window::on_actionSelect_All_triggered()
{
    knotview()->select_all();
}

void Knot_Window::on_actionInsert_Edges_triggered()
{
    knotview()->mode_edit_node_edge();
}

void Knot_Window::on_actionRefresh_Path_triggered()
{
    knotview()->redraw(true);
}

void Knot_Window::on_action_Reset_View_triggered()
{
    knotview()->reset_view();
}

void Knot_Window::on_action_Reset_Zoom_triggered()
{
    knotview()->reset_zoom();
}

void Knot_Window::on_action_Horizontal_Flip_triggered()
{
    knotview()->flip_horizontal();
}

void Knot_Window::on_action_Vertical_Flip_triggered()
{
    knotview()->flip_vertical();
}

void Knot_Window::on_action_Move_Grid_triggered()
{
    knotview()->mode_move_grid();
}

void Knot_Window::on_actionShow_Knot_line_triggered(bool checked)
{
    knotview()->toggle_knotline(checked);
}

void Knot_Window::on_action_Unlink_triggered()
{
    knotview()->unlink_selected();
}

void Knot_Window::on_action_Rotate_triggered()
{
    knotview()->transform_mode_rotate();
}

void Knot_Window::on_action_Scale_triggered()
{
    knotview()->transform_mode_scale();
}

void Knot_Window::on_actionEdge_List_triggered()
{
    knotview()->mode_edge_chain();
}

void Knot_Window::on_tabWidget_currentChanged(QWidget *arg1)
{
    KnotView* next_canvas = dynamic_cast<KnotView*>(arg1);
    if ( next_canvas )
    {
        if ( canvas )
        {
            disconnect_view();
            next_canvas->same_mode(*canvas);
        }
        canvas = next_canvas;
        update_ui();
        connect_view();
    }
}

void Knot_Window::on_tabWidget_tabCloseRequested(int index)
{
    KnotView* view = canvas_at(index);

    ErrorRecovery::remove(&view->graph());

    if ( index == tabWidget->currentIndex() )
    {
        if ( tabWidget->count() == 1 )
        {
            new_tab();
            update_ui();
        }
        else
            tabWidget->setCurrentIndex((index+1)%tabWidget->count());
    }

    delete view;


}

void Knot_Window::on_actionSave_A_ll_triggered()
{
    int curr = tabWidget->currentIndex();
    for ( int i = 0; i < tabWidget->count(); i++ )
    {
        tabWidget->setCurrentIndex(i);
        save();
    }
    tabWidget->setCurrentIndex(curr);
}

void Knot_Window::on_actionS_nap_to_grid_triggered()
{
    KnotView* kv = knotview();

    node_list sel =  kv->selected_nodes();
    if ( !sel.empty() )
    {
        kv->begin_macro(tr("Snap Nodes"));


        foreach (Node* n, sel )
            kv->move_node(n, kv->get_grid().nearest(n->pos()) );

        kv->end_macro();
    }
}

void Knot_Window::on_action_Print_triggered()
{
    QPrintDialog dialog(&printer, this);
    if (dialog.exec())
    {
        print(&printer);
    }
}


void Knot_Window::print ( QPrinter* p )
{
    QPainter painter(p);
    painter.setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);
    QRect rect = painter.viewport();
    QSize size = knotview()->viewport()->size();
    size.scale(rect.size(), Qt::KeepAspectRatio);
    painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
    painter.setWindow(knotview()->viewport()->rect());
    knotview()->graph().paint_knot(&painter,false);
}

void Knot_Window::on_actionPage_Set_up_triggered()
{
    QPageSetupDialog page_detup(&printer,this);
    page_detup.exec();
}

void Knot_Window::on_actionPr_int_Preview_triggered()
{
    QPrintPreviewDialog dialog(&printer,this);
    connect(&dialog,SIGNAL(paintRequested(QPrinter*)),SLOT(print(QPrinter*)));
    dialog.exec();
}
