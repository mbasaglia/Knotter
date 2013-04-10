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
#include <QSettings>
#include <QMessageBox>

Main_Window::Main_Window(QWidget *parent) :
    QMainWindow(parent), save_ui(true)
{
    setupUi(this);
    setWindowIcon(QIcon(Resource_Manager::data("img/icon-small.svg")));

    setWindowTitle(Resource_Manager::program_name());

    load_config();

    init_menus();
    init_toolbars();
}

void Main_Window::retranslate()
{
    retranslateUi(this);
    setWindowTitle(Resource_Manager::program_name());
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
}

void Main_Window::load_config()
{
    QSettings settings("knotter","knotter");

    QString config_version = settings.value("version",VERSION).toString();
    if ( !Resource_Manager::check_least_version(config_version,0,9) )
    {
        qWarning() << tr("Warning:")
                   << tr("Discarding old configuration");
        return;
    }
    if ( config_version != VERSION)
    {
        int load_old = QMessageBox::question(this,
                    tr("Load old configuration"),
                    tr("Knotter has detected configuration for version %1,\n"
                        "this is version %2.\n"
                        "Do you want to load it anyways?")
                        .arg(config_version).arg(VERSION),
                QMessageBox::Yes,QMessageBox::No
        );
        if ( load_old != QMessageBox::Yes)
            return;
    }

    /// \todo Language

    /// \todo performance

    settings.beginGroup("gui");


    int ntoolbars = settings.beginReadArray("toolbar");

    if ( ntoolbars == 0 )
    {
        foreach(QToolBar* tb, findChildren<QToolBar*>() )
            delete tb;

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
            QString title = settings.value("title").toString();

            QToolBar *toolbar = new QToolBar(title,this);
            toolbar->setObjectName(name);
            addToolBar(Qt::TopToolBarArea,toolbar);

            QStringList items = settings.value("items").toStringList();
            foreach(const QString&item, items)
            {
                if ( item.isEmpty()  || item == "(separator)" )
                    toolbar->addSeparator();
                else
                {
                    QAction* act = action_by_name(item);
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


    save_ui = settings.value("save",save_ui).toBool();
    if ( save_ui )
    {
        restoreGeometry(settings.value("geometry").toByteArray());
        restoreState(settings.value("state").toByteArray());

        int icon_size = settings.value("icon_size",iconSize().width()).toInt();
        setIconSize(QSize(icon_size,icon_size));
        /// \todo Transformation handle size
        setToolButtonStyle ( Qt::ToolButtonStyle (
                     settings.value("buttons",toolButtonStyle()).toInt() ) );
    }
    settings.endGroup();//gui

    /// \todo recent files

    /// \todo style, grid

}

QAction *Main_Window::action_by_name(QString name) const
{
    QAction* act = findChild<QAction*>(name);

    if ( !act )
    {
        QMenu *menu = findChild<QMenu*>(name);
        if ( menu )
            act = menu->menuAction();
    }

    return act;
}
