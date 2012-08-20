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
#include "node_style_form.hpp"
#include "global_style_form.hpp"

Knot_Window::Knot_Window(QWidget *parent) :
    QMainWindow(parent), save_ui ( true ), max_recent_files(5)
{
    setupUi(this);

    export_dialog.set_knot_view(canvas);
    export_dialog.setAttribute(Qt::WA_QuitOnClose, false);

    ErrorRecovery::recover = canvas;

    action_New->setIcon(QIcon::fromTheme("document-new"));
    action_New->setShortcuts(QKeySequence::New);
    action_Save->setIcon(QIcon::fromTheme("document-save"));
    action_Save->setShortcuts(QKeySequence::Save);
    actionSave_As->setIcon(QIcon::fromTheme("document-save-as"));
    actionSave_As->setShortcuts(QKeySequence::SaveAs);
    action_Open->setIcon(QIcon::fromTheme("document-open"));
    action_Open->setShortcuts(QKeySequence::Open);
    action_Quit->setIcon(QIcon::fromTheme("application-exit"));
    action_Quit->setShortcuts(QKeySequence::Quit);
    action_Export->setIcon(QIcon::fromTheme("image-x-generic"));


    QAction *undos = canvas->get_undo_stack().createUndoAction(this, tr("&Undo"));
    menu_Edit->insertAction(action_Undo,undos);
    MainToolBar->insertAction(action_Undo,undos);
    delete action_Undo;
    action_Undo = undos;
    QAction *redos =  canvas->get_undo_stack().createRedoAction(this, tr("&Redo"));
    menu_Edit->insertAction(action_Redo,redos);
    MainToolBar->insertAction(action_Redo,redos);
    delete action_Redo;
    action_Redo = redos;

    action_Copy->setIcon(QIcon::fromTheme("edit-copy"));
    action_Copy->setShortcuts(QKeySequence::Copy);
    action_Paste->setIcon(QIcon::fromTheme("edit-paste"));
    action_Paste->setShortcuts(QKeySequence::Paste);
    actionCut->setIcon(QIcon::fromTheme("edit-cut"));
    actionCut->setShortcuts(QKeySequence::Cut);

    action_Undo->setIcon(QIcon::fromTheme("edit-undo"));
    action_Undo->setShortcuts(QKeySequence::Undo);
    action_Redo->setIcon(QIcon::fromTheme("edit-redo"));
    action_Redo->setShortcuts(QKeySequence::Redo);
    actionSelect_All->setIcon(QIcon::fromTheme("edit-select-all"));
    actionSelect_All->setShortcuts(QKeySequence::SelectAll);

    action_Preferences->setIcon(QIcon::fromTheme("preferences-other"));


    action_Erase->setIcon(QIcon::fromTheme("edit-delete"));
    //action_Link->setIcon(QIcon::fromTheme("insert-link"));


    actionZoom_In->setIcon(QIcon::fromTheme("zoom-in"));
    actionZoom_In->setShortcuts(QKeySequence::ZoomIn);
    actionZoom_Out->setIcon(QIcon::fromTheme("zoom-out"));
    actionZoom_Out->setShortcuts(QKeySequence::ZoomOut);
    action_Reset_Zoom->setIcon(QIcon::fromTheme("zoom-original"));
    actionRefresh_Path->setIcon(QIcon::fromTheme("view-refresh"));
    actionRefresh_Path->setShortcuts(QKeySequence::Refresh);
    action_Reset_View->setIcon(QIcon::fromTheme("view-restore"));

    // overcome bug in code generator from ui file
    removeToolBar(EditBar);
    removeToolBar(MainToolBar);
    addToolBar(Qt::TopToolBarArea, MainToolBar);
    addToolBar(Qt::TopToolBarArea, EditBar);
    MainToolBar->show();
    EditBar->show();


    this->connect(&canvas->get_undo_stack(),SIGNAL(cleanChanged(bool)),SLOT(update_title(bool)));

    QUndoView *undoView = new QUndoView(&canvas->get_undo_stack());
    undoDock = new closable_dock(this);
    undoDock->setFloating(true);
    undoDock->setWidget(undoView);
    undoDock->setObjectName("Action_History");
    undoDock->setWindowTitle(tr("Action History"));
    undoDock->show();
    addDockWidget(static_cast<Qt::DockWidgetArea>(2), undoDock);
    actionAction_History->connect(undoDock,SIGNAL(openChanged(bool)),SLOT(setChecked(bool)));
    undoDock->connect(actionAction_History,SIGNAL(triggered(bool)),SLOT(setVisible(bool)));

    node_style_form* default_node_style_form = new node_style_form;
    canvas->connect(default_node_style_form,SIGNAL(style_changed(styleinfo)),
                    SLOT(set_default_style(styleinfo)));
    default_node_style_dock = new closable_dock(this);
    default_node_style_dock->setWidget(default_node_style_form);
    default_node_style_dock->setObjectName("Default_Node_Style");
    default_node_style_dock->setWindowTitle(tr("Default Node Style"));
    actionDefault_Node_Style->connect(default_node_style_dock,
                            SIGNAL(openChanged(bool)),SLOT(setChecked(bool)));
    default_node_style_dock->connect(actionDefault_Node_Style,
                            SIGNAL(triggered(bool)),SLOT(setVisible(bool)));



    global_style_form* global_style_frm = new global_style_form;
    global_style_frm->set_join_style(canvas->get_join_style());
    global_style_frm->set_knot_color(canvas->get_brush().color());
    global_style_frm->set_knot_width(canvas->get_width());
    global_style_frm->set_pen(canvas->get_pen());
    canvas->connect(global_style_frm,SIGNAL(knot_color_changed(QColor)),
                    SLOT(set_brush_color(QColor)));
    canvas->connect(global_style_frm,SIGNAL(join_style_changed(Qt::PenJoinStyle)),
                    SLOT(set_join_style(Qt::PenJoinStyle)));
    canvas->connect(global_style_frm,SIGNAL(knot_width_changed(double)),
                    SLOT(set_width(double)));
    canvas->connect(global_style_frm,SIGNAL(pen_changed(QPen)),
                    SLOT(set_pen(QPen)));
    global_style_dock = new closable_dock(this);
    global_style_dock->setWidget(global_style_frm);
    global_style_dock->setObjectName("Knot_Style");
    global_style_dock->setWindowTitle(tr("Knot Style"));
    actionKnot_Style->connect(global_style_dock,
                            SIGNAL(openChanged(bool)),SLOT(setChecked(bool)));
    global_style_dock->connect(actionKnot_Style,
                            SIGNAL(triggered(bool)),SLOT(setVisible(bool)));


    this->connect(config_dlg.clear_recent,SIGNAL(clicked()),SLOT(clear_recent_files()));

    action_Manual->setIcon(QIcon::fromTheme("help-contents"));
    help_view.setWindowIcon(QIcon::fromTheme("help-contents"));
    help_view.setAttribute(Qt::WA_QuitOnClose, false);

    action_About->setIcon(QIcon::fromTheme("help-about"));

    load_config();
    update_recent_menu();
}

Knot_Window::~Knot_Window()
{
    save_config();
}


void Knot_Window::load_config()
{
    QSettings settings("knotter","knotter");


    if ( settings.value("version",VERSION).toString() != VERSION )
        return; // don't load settings from different version

    settings.beginGroup("gui");
    save_ui = settings.value("save",true).toBool();
    if ( save_ui )
    {
        restoreGeometry(settings.value("geometry").toByteArray());
        restoreState(settings.value("state").toByteArray());

        actionAction_History->setChecked(undoDock->isVisible());
        actionDefault_Node_Style->setChecked(default_node_style_dock->isVisible());
        actionKnot_Style->setChecked(global_style_dock->isVisible());

        int icon_size = settings.value("icon_size",iconSize().width()).toInt();
        setIconSize(QSize(icon_size,icon_size));
        setToolButtonStyle ( Qt::ToolButtonStyle (
                     settings.value("buttons",toolButtonStyle()).toInt() ) );
    }
    settings.endGroup();

    settings.beginGroup("recent_files");
    recent_files = settings.value("list").toStringList();
    max_recent_files = settings.value("max",max_recent_files).toInt();
    settings.endGroup();

    settings.beginGroup("grid");
    snapping_grid &grid = canvas->get_grid();
    grid.set_size(settings.value("size",grid.get_size()).toDouble());
    grid.set_shape(snapping_grid::grid_shape(
                    settings.value("type",int(grid.get_shape())).toInt() ));
    update_grid_icon();
    settings.endGroup();


    /// \todo save style?


}

void Knot_Window::save_config()
{
    QSettings settings("knotter","knotter");

    settings.setValue("version",VERSION);

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
    settings.endGroup();

    settings.beginGroup("recent_files");
    settings.setValue("list",recent_files);
    settings.setValue("max",max_recent_files);
    settings.endGroup();

    settings.beginGroup("grid");
    settings.setValue("size",canvas->get_grid().get_size());
    settings.setValue("type",int(canvas->get_grid().get_shape()));
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

    canvas->writeXML(&quf);
    canvas->get_undo_stack().setClean();

    push_recent_file(file);

    quf.close();
}

bool Knot_Window::open(QString file, bool silent)
{

    QFile quf(file);
    if ( ! quf.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        if ( !silent )
            QMessageBox::warning(this,tr("File Error"),tr("Could not read \"%1\".").arg(file));
        return false;
    }
    filename = file;

    canvas->clear();
    canvas->get_undo_stack().setClean();

    if ( !canvas->readXML(&quf) && !silent )
    {
        QMessageBox::warning(this,tr("File Error"),tr("Error while reading \"%1\".").arg(filename));
        return false;
    }


    push_recent_file(filename);

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
    config_dlg.max_recent->setValue(max_recent_files);
    if ( !config_dlg.exec() )
        return;
    setToolButtonStyle(config_dlg.get_tool_button_style());
    setIconSize(config_dlg.get_icon_size());
    save_ui = config_dlg.saveui_check->isChecked();
    max_recent_files = config_dlg.max_recent->value();
    if ( max_recent_files < recent_files.size() )
        recent_files.erase ( recent_files.begin()+max_recent_files, recent_files.end() );
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
            QAction *a = menuOpen_Recent->addAction(QIcon(":/img/logo.svg"), savefile);
            connect(a, SIGNAL(triggered()), this, SLOT(click_recent()));
        }
}

void Knot_Window::update_grid_icon()
{
    snapping_grid::grid_shape gs = canvas->get_grid().get_shape();
    if ( gs == snapping_grid::TRIANGLE1 )
        actionEnable_Grid->setIcon(QIcon(":/img/triangular_grid.svg"));
    else if ( gs == snapping_grid::TRIANGLE2 )
        actionEnable_Grid->setIcon(QIcon(":/img/triangular_grid2.svg"));
    else
        actionEnable_Grid->setIcon(QIcon(":/img/square_grid.svg"));
}

void Knot_Window::click_recent()
{
    QAction*a = dynamic_cast<QAction*>(sender());
    if ( a )
        open(a->text());
}



void Knot_Window::on_action_About_triggered()
{
    QMessageBox mb(QMessageBox::Information, tr("About Knotter"), BUILD_INFO );
    mb.setAttribute(Qt::WA_QuitOnClose, false);
    mb.setWindowIcon(QIcon::fromTheme("help-about"));
    mb.exec();
}
