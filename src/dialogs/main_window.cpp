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
#include "preferences_dialog.hpp"
#include <QDockWidget>
#include <QDesktopServices>
#include <QUrl>
#include "icon_dock_style.hpp"


Main_Window::Main_Window(QWidget *parent) :
    QMainWindow(parent), zoomer(nullptr), view(nullptr)
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

}

void Main_Window::retranslate()
{
    retranslateUi(this);
    setWindowTitle(Resource_Manager::program_name());
}

Main_Window::~Main_Window()
{
    Resource_Manager::settings.save_window(this);
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

    // Menu Tools
    action_Refresh_Path->setShortcut(QKeySequence::Refresh);

    // Menu Help
    action_Manual->setShortcut(QKeySequence::HelpContents);
    connect(action_About_Qt,SIGNAL(triggered()),qApp, SLOT(aboutQt()));



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
    // Knot Display
    dock_knot_display = new Dock_Knot_Display(this);
    addDockWidget(Qt::RightDockWidgetArea,dock_knot_display);

    // Background
    dock_background = new Dock_Background(this);
    dock_background->hide();
    dock_background->setFloating(true);

    // Grid config
    dock_grid = new Dock_Grid(this);
    addDockWidget(Qt::RightDockWidgetArea,dock_grid);

    // Action History Dock
    undo_view = new QUndoView(&undo_group);
    QDockWidget* undo_dock  = new QDockWidget;
    undo_dock->setWidget(undo_view);
    undo_dock->setObjectName("Action_History");
    undo_dock->setWindowTitle(tr("Action History"));
    undo_dock->setWindowIcon(QIcon::fromTheme("view-history"));
    addDockWidget(Qt::RightDockWidgetArea,undo_dock);
    tabifyDockWidget(undo_dock,dock_grid);
    undo_dock->raise();


    connect(&undo_group,SIGNAL(cleanChanged(bool)),SLOT(set_clean_icon(bool)));
    connect(&undo_group,SIGNAL(undoTextChanged(QString)),SLOT(set_undo_text(QString)));
    connect(&undo_group,SIGNAL(redoTextChanged(QString)),SLOT(set_redo_text(QString)));
    connect(&undo_group,SIGNAL(canUndoChanged(bool)),action_Undo,SLOT(setEnabled(bool)));
    connect(&undo_group,SIGNAL(canRedoChanged(bool)),action_Redo,SLOT(setEnabled(bool)));
    connect(action_Undo,SIGNAL(triggered()),&undo_group,SLOT(undo()));
    connect(action_Redo,SIGNAL(triggered()),&undo_group,SLOT(redo()));



    // Menu entries
    foreach(QDockWidget* dw, findChildren<QDockWidget*>())
    {
        QAction *a = dw->toggleViewAction();
        a->setIcon(dw->windowIcon());
        menu_Docks->insertAction(0,a);
        dw->setStyle(new Icon_Dock_Style(dw));
    }
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

    // zoom/view
    zoomer->setValue(v->get_zoom_factor()*100);
    connect(v,SIGNAL(zoomed(double)),zoomer,SLOT(setValue(double)));

    // edit mode
    connect(action_Edit_Graph,SIGNAL(triggered()),v,SLOT(set_mode_edit_graph()),
            Qt::UniqueConnection);
    connect(action_Edge_Loop,SIGNAL(triggered()),v,SLOT(set_mode_edge_chain()),
            Qt::UniqueConnection);

    // undo/redo
    v->undo_stack_pointer()->setActive(true);

    // grid editor
    dock_grid->set_grid(&v->grid());
    connect(dock_grid,SIGNAL(move_grid()),v,SLOT(set_mode_move_grid()));

}

void Main_Window::disconnect_view(Knot_View *v)
{
    if ( v != nullptr )
    {
        disconnect(v,SIGNAL(zoomed(double)),zoomer,SLOT(setValue(double)));
        disconnect(dock_grid,SIGNAL(move_grid()),v,SLOT(set_mode_move_grid()));
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
}

void Main_Window::apply_zoom()
{
    view->set_zoom(zoomer->value()/100);
}

void Main_Window::on_action_Preferences_triggered()
{
    Preferences_Dialog(this).exec();
}


void Main_Window::create_tab(QString file)
{
    Knot_View *v = new Knot_View(file);
    int t = tabWidget->addTab(v,file.isEmpty() ? tr("New Knot") : file);
    undo_group.addStack(v->undo_stack_pointer());
    switch_to_tab(t);

}

void Main_Window::switch_to_tab(int i)
{
    tabWidget->setCurrentIndex(i);
    setWindowTitle(tr("%1 - %2").arg(Resource_Manager::program_name())
                   .arg(tabWidget->tabText(i)));
    disconnect_view(view);
    connect_view(dynamic_cast<Knot_View*>(tabWidget->currentWidget()));
    update_title();
}

void Main_Window::close_tab(int i)
{
    /// \todo check for edited file
    Knot_View* kv = dynamic_cast<Knot_View*>(tabWidget->widget(i));
    if ( kv )
    {
        undo_group.removeStack(kv->undo_stack_pointer());
        delete kv;
    }
    tabWidget->removeTab(i);
    if ( tabWidget->count() == 0 )
        create_tab();
}

void Main_Window::on_action_Show_Graph_toggled(bool arg1)
{
    /// \todo actually toggle
    if ( arg1 )
        action_Show_Graph->setIcon(QIcon::fromTheme("knot-graph-on"));
    else
        action_Show_Graph->setIcon(QIcon::fromTheme("knot-graph-off"));
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
    view->resetTransform();
}

void Main_Window::on_action_Report_Bugs_triggered()
{
    QDesktopServices::openUrl(QUrl(BUG_URL));
}

void Main_Window::on_action_Manual_triggered()
{
    QDesktopServices::openUrl(QUrl(DOC_URL));
}
