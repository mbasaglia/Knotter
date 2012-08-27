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
#include "translator.hpp"

config_dialog::config_dialog(QWidget *parent) :
    QDialog(parent), current_menu(NULL), current_toolbar(NULL)
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
    butt_remove->setIcon(load::icon("list-remove"));
    butt_insert->setIcon(load::icon("list-add"));
    butt_separator->setIcon(load::icon("insert-horizontal-rule"));

    add_toolbar_btn->setIcon(load::icon("list-add"));
    rm_toolbar_btn->setIcon(load::icon("list-remove"));

    language_combo->addItems(Translator::object.available_languages());
    language_combo->setCurrentIndex ( language_combo->findText(Translator::object.current_lang_name()) );
    Translator::object.connect(language_combo,SIGNAL(activated(QString)),SLOT(change_lang_name(QString)));
    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));

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
    if ( menu )
    {
        current_menu = menu;
        menu_items->clear();
        foreach(QAction*act,menu->actions())
        {
            if ( !act->isSeparator() )
                menu_items->addItem(new QListWidgetItem(act->icon(),act->iconText()));
            else
            {
                menu_items->addItem(new QListWidgetItem("--"));
            }
        }
        menu_combo->setCurrentIndex(menu_combo->findText(menu->menuAction()->iconText()));
    }
}

void config_dialog::set_toolbar(QToolBar* tb)
{
    if ( tb )
    {
        current_toolbar = tb;
        toolbar_items->clear();
        foreach(QAction*act,tb->actions())
        {
            if ( !act->isSeparator() )
                toolbar_items->addItem(new QListWidgetItem(act->icon(),act->iconText()));
            else
            {
                toolbar_items->addItem(new QListWidgetItem("--"));
            }

        }
        toolbar_combo->setCurrentIndex(toolbar_combo->findText(tb->windowTitle()));
    }
}

void config_dialog::add_menu(QAction *act)
{
    QMenu* menu = act->menu();
    if ( menu && !menus.contains(menu) )
    {
        menus.push_back(menu);
        menu_combo->addItem(menu->menuAction()->iconText());
    }
}

void config_dialog::add_toolbar(QToolBar *tb)
{
    if ( tb && !toolbars.contains(tb) )
    {
        toolbars.push_back(tb);
        toolbar_combo->addItem(tb->windowTitle());
    }
}


void config_dialog::retranslate()
{
    retranslateUi(this);
}

void config_dialog::on_menu_combo_activated(const QString &arg1)
{
    foreach(QMenu* menu,menus)
    {
        if ( arg1 == menu->menuAction()->iconText() )
        {
            set_menu(menu);
            return;
        }
    }
}

void config_dialog::on_toolbar_combo_activated(const QString &arg1)
{
    foreach(QToolBar* toolb,toolbars)
    {
        if ( arg1 == toolb->windowTitle() )
        {
            set_toolbar(toolb);
            return;
        }
    }
}

void config_dialog::on_butt_move_up_clicked()
{
    if ( current_toolbar )
    {
        int curr_index = toolbar_items->currentIndex().row();
        if ( curr_index <= 0 || curr_index >= current_toolbar->actions().size() )
            return;
        QAction* curr_act = current_toolbar->actions()[curr_index];
        QAction* prev_act = current_toolbar->actions().value(curr_index-1,NULL);
        current_toolbar->removeAction(curr_act);
        current_toolbar->insertAction(prev_act,curr_act);

        QListWidgetItem *curr_itm = toolbar_items->takeItem ( curr_index );
        toolbar_items->insertItem(curr_index-1,curr_itm);
        toolbar_items->setCurrentRow(curr_index-1);
    }
}

void config_dialog::on_butt_move_down_clicked()
{
    if ( current_toolbar )
    {
        int curr_index = toolbar_items->currentIndex().row();
        if ( curr_index < 0 || curr_index >= current_toolbar->actions().size()-1 )
            return;
        QAction* curr_act = current_toolbar->actions()[curr_index];
        QAction* next_act = current_toolbar->actions().value(curr_index+2,NULL);
        current_toolbar->removeAction(curr_act);
        current_toolbar->insertAction(next_act,curr_act);

        QListWidgetItem *curr_itm = toolbar_items->takeItem ( curr_index );
        toolbar_items->insertItem(curr_index+1,curr_itm);
        toolbar_items->setCurrentRow(curr_index+1);
    }
}

void config_dialog::on_butt_insert_clicked()
{
    if ( current_toolbar && current_menu )
    {
        int curr_menu_index = menu_items->currentIndex().row();
        int curr_toolbar_index = toolbar_items->currentIndex().row()+1;
        if ( curr_menu_index < 0 || curr_menu_index >= current_menu->actions().size() )
            return;

        QAction* act = current_menu->actions()[curr_menu_index];
        if ( act->isSeparator() )
            on_butt_separator_clicked();
        else if ( !current_toolbar->actions().contains(act) )
        {
            QAction* next = current_toolbar->actions().value(curr_toolbar_index,NULL);
            current_toolbar->insertAction(next,act);
            QListWidgetItem* newitem = new QListWidgetItem(act->icon(),act->iconText());
            toolbar_items->insertItem(curr_toolbar_index,newitem);
            toolbar_items->setCurrentRow(curr_toolbar_index);
        }
    }
}

void config_dialog::on_butt_separator_clicked()
{
    if ( current_toolbar )
    {
        int curr_index = toolbar_items->currentIndex().row()+1;
        QAction* next_act = current_toolbar->actions().value(curr_index,NULL);
        current_toolbar->insertSeparator(next_act);
        toolbar_items->insertItem(curr_index,"--");
        toolbar_items->setCurrentRow(curr_index);
    }
}

void config_dialog::on_butt_remove_clicked()
{
    if ( current_toolbar )
    {
        int curr_index = toolbar_items->currentIndex().row();
        QAction* act = current_toolbar->actions().value(curr_index,NULL);
        if ( act )
        {
            current_toolbar->removeAction(act);
            delete toolbar_items->takeItem(curr_index);
        }
    }

}

void config_dialog::on_add_toolbar_btn_clicked()
{
    QToolBar* ntb = new QToolBar;
    QString name = tr("Toolbar%1").arg(toolbars.size());
    ntb->setWindowTitle(name);
    ntb->setObjectName(name);
    emit insert_toolbar(ntb);
    add_toolbar(ntb);
    set_toolbar(ntb);
}

void config_dialog::on_rm_toolbar_btn_clicked()
{
    if ( current_toolbar )
    {
        int indx = toolbar_combo->findText(current_toolbar->windowTitle());
        delete current_toolbar;
        toolbar_items->clear();
        toolbar_combo->removeItem(indx);
    }
}
