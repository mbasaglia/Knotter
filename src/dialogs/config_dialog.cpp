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
#include "config_dialog.hpp"
#include "resource_loader.hpp"

config_dialog::config_dialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    // Bad Designer... >:^(
    stackedWidget->connect(tableWidget,SIGNAL(cellClicked(int,int)),SLOT(setCurrentIndex(int)));

    /// \warning Icons are in Oxygen theme but not in freedesktop std
    tableWidget->item(0,0)->setIcon(load::icon("configure-toolbars"));
    tableWidget->item(1,0)->setIcon(load::icon("configure"));
    tableWidget->item(2,0)->setIcon(load::icon("preferences-system-performance"));

    butt_move_up->setIcon(load::icon("go-up"));
    butt_move_down->setIcon(load::icon("go-down"));
    butt_move_left->setIcon(load::icon("list-remove"));
    butt_move_right->setIcon(load::icon("list-add"));
    butt_separator->setIcon(load::icon("insert-horizontal-rule"));

}

void config_dialog::set_icon_size(QSize size)
{
    if ( size.width() < 20 )
        icon_size_combo->setCurrentIndex(0);
    else if ( size.width() < 28 )
        icon_size_combo->setCurrentIndex(1);
    else if ( size.width() < 40 )
        icon_size_combo->setCurrentIndex(2);
    else
        icon_size_combo->setCurrentIndex(3);
}

QSize config_dialog::get_icon_size() const
{
    switch ( icon_size_combo->currentIndex() )
    {
        case 0: return QSize(16,16);
        default:
        case 1: return QSize(24,24);
        case 2: return QSize(32,32);
        case 3: return QSize(48,48);
    }
}

Qt::ToolButtonStyle config_dialog::get_tool_button_style() const
{
    switch ( icon_show_combo->currentIndex() )
    {
        default: return Qt::ToolButtonFollowStyle;
        case 0: return Qt::ToolButtonIconOnly;
        case 1: return Qt::ToolButtonTextOnly;
        case 2: return Qt::ToolButtonTextBesideIcon;
        case 3: return Qt::ToolButtonTextUnderIcon;
    }
}

void config_dialog::set_tool_button_style(Qt::ToolButtonStyle style)
{
    int index;
    switch ( style )
    {
        default:
        case Qt::ToolButtonIconOnly:        index = 0; break;
        case Qt::ToolButtonTextOnly:        index = 1; break;
        case Qt::ToolButtonTextBesideIcon:  index = 2; break;
        case Qt::ToolButtonTextUnderIcon:   index = 3; break;
    }
    icon_show_combo->setCurrentIndex(index);
}

void config_dialog::set_menu(QMenu *menu)
{
    menu_items->clear();
    foreach(QAction*act,menu->actions())
    {
        if ( !act->text().isNull() ) // discard separators
            menu_items->addItem(new QListWidgetItem(act->icon(),act->iconText()));
    }
}

void config_dialog::set_toolbar(QToolBar* tb)
{
    toolbar_items->clear();
    foreach(QAction*act,tb->actions())
    {
        if ( !act->text().isNull() )
            toolbar_items->addItem(new QListWidgetItem(act->icon(),act->iconText()));
        else
        {
            toolbar_items->addItem(new QListWidgetItem("--"));
        }

    }
}
