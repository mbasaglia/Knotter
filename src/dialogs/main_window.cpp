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


Main_Window::Main_Window(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    setWindowIcon(QIcon(Resource_Manager::data("img/icon-small.svg")));

    setWindowTitle(Resource_Manager::program_name());

    load_config();

    init_menus();
    init_toolbars();

    create_tab();
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
    /// \todo docks
    QActionGroup* display_mode = new QActionGroup(this);
    display_mode->addAction(action_Normal);
    display_mode->addAction(action_Highlight_Links);

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



}

void Main_Window::init_toolbars()
{
    foreach(QToolBar* tb, findChildren<QToolBar*>())
        menu_Toolbars->insertAction(0,tb->toggleViewAction());


    // Statusbar...
    zoomer = new QDoubleSpinBox(this);
    zoomer->setMinimum(0.01);
    zoomer->setMaximum(800);
    zoomer->setSuffix("%");
    zoomer->setValue(100);
    statusBar()->addPermanentWidget(new QLabel(tr("Zoom")));
    statusBar()->addPermanentWidget(zoomer);
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

void Main_Window::set_icon_size(int sz)
{
    setIconSize(QSize(sz,sz));
}//....

void Main_Window::set_tool_button_style(Qt::ToolButtonStyle tbs)
{
    setToolButtonStyle(tbs);
}

void Main_Window::on_action_Preferences_triggered()
{
    Preferences_Dialog(this).exec();
}


void Main_Window::create_tab(QString file)
{
    int t = tabWidget->addTab(new Knot_View(file),file.isEmpty() ? tr("New Knot") : file);
    switch_to_tab(t);

}

void Main_Window::switch_to_tab(int i)
{
    tabWidget->setCurrentIndex(i);
    setWindowTitle(tr("%1 - %2").arg(Resource_Manager::program_name())
                   .arg(tabWidget->tabText(i)));
}

void Main_Window::close_tab(int i)
{
    /// \todo check for edited file
    //delete tabWidget->widget(i);
    tabWidget->removeTab(i);
    if ( tabWidget->count() == 0 )
        create_tab();
}
