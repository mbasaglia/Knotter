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

#include "about_dialog.hpp"
#include "resource_manager.hpp"
#include <QDir>
#include <QContextMenuEvent>
#include <QClipboard>
#include <QDesktopServices>
#include <QUrl>

About_Dialog::About_Dialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    label_title->setText(QString("%1 %2").arg(Resource_Manager::program_name())
                         .arg(Resource_Manager::program_version() ));
    label_icon->setPixmap(QIcon(Resource_Manager::data("img/icon-big.svg")).pixmap(64));
    setWindowTitle(tr("About %1").arg(Resource_Manager::program_name()));


    list_plugin_dirs->addItems(Resource_Manager::data_directories_unckecked("plugins"));

    list_data_dirs->addItems(Resource_Manager::data_directories_unckecked(""));

    text_settings_file->setText(Resource_Manager::settings.settings_file());

    text_witable_data_dir->setText(Resource_Manager::writable_data_directory(""));

    context_menu.addAction(QIcon::fromTheme("edit-copy"),"Copy directory name",
                           this, SLOT(click_copy()));
    context_menu.addAction(QIcon::fromTheme("document-open"),"Open in file manager",
                           this, SLOT(click_open()));
}

void About_Dialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            setWindowTitle(tr("About %1").arg(Resource_Manager::program_name()));
            break;
        default:
            break;
    }
}


void About_Dialog::on_button_qt_clicked()
{
    QApplication::aboutQt();
}

void About_Dialog::contextmenu_line(QPoint pos)
{
    QLineEdit* ed = static_cast<QLineEdit*>(sender());

    show_context_menu(ed->text(),ed->mapToGlobal(pos));

}

void About_Dialog::contextmenu_list(QPoint pos)
{
    QListWidget* list = static_cast<QListWidget*>(sender());
    QListWidgetItem * it = list->itemAt(pos);
    if ( it )
    {
        show_context_menu(it->text(),list->mapToGlobal(pos));
    }
}

void About_Dialog::show_context_menu(QString file, QPoint pos)
{
    clicked_entry = file;
    if ( !clicked_entry.isEmpty() )
    {
        QFileInfo finfo(clicked_entry);

        context_menu.actions()[0]->setText(tr("Copy directory name"));
        context_menu.actions()[1]->setText(tr("Open in file manager"));

        if ( finfo.exists() )
        {
            context_menu.actions()[1]->setEnabled(true);
            if ( finfo.isFile() )
            {
                context_menu.actions()[0]->setText(tr("Copy file name"));
                context_menu.actions()[1]->setText(tr("Open file"));
            }
        }
        else
        {
            context_menu.actions()[1]->setEnabled(false);
        }
        context_menu.popup(pos);
    }
}

void About_Dialog::click_copy()
{
    QApplication::clipboard()->setText(clicked_entry);
}

void About_Dialog::click_open()
{
    QDesktopServices::openUrl(QUrl("file:///"+clicked_entry));
}
