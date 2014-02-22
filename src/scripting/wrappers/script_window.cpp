/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#include "script_window.hpp"
#include "main_window.hpp"

#include <QUiLoader>
#include <QFile>
#include <QMessageBox>
#include "resource_manager.hpp"
#include <QFileDialog>
#include <QInputDialog>
#include <limits>


Script_Window::Script_Window(Main_Window *window, QObject *parent) :
    QObject(parent), window(window), m_dialog(window)
{
    connect(window,SIGNAL(tab_closing(Knot_View*)),SLOT(close_tab(Knot_View*)));
}

bool Script_Window::open(QString name)
{
    return window->create_tab(name);
}

QString Script_Window::current_file() const
{
    return window->view->windowFilePath();
}

int Script_Window::current_tab() const
{
    return window->tabWidget->currentIndex();
}

void Script_Window::set_current_tab(int t)
{
    if ( t >= 0 && t < open_tabs() )
        window->switch_to_tab(t);
}

int Script_Window::open_tabs() const
{
    return window->tabWidget->count();
}

Script_Document* Script_Window::document()
{
    if ( !docs.contains(window->view) )
    {
        Script_Document* doc = new Script_Document(window->view,this);
        connect(doc,SIGNAL(style_changed()),window,SLOT(update_style()));
        docs[window->view] = doc;
    }

    return docs[window->view];
}

Script_Window_Dialog *Script_Window::dialog()
{
    return &m_dialog;
}

QString Script_Window::toString() const
{
    return "[Knotter window]";
}

void Script_Window::close_tab(Knot_View *view)
{
    if ( docs.contains(view) )
    {
        delete docs[view];
        docs.remove(view);
    }
}

void Script_Window::screenshot(QString output_image_file, QString widget)
{
    QWidget *paintee = window;
    if ( !widget.isEmpty() )
        paintee = window->findChild<QWidget*>(widget);
    if ( paintee == nullptr )
        paintee = window;

    QPixmap shot(paintee->size());
    paintee->render(&shot);

    shot.save(output_image_file);
}


void Script_Window_Dialog::information(QString message, QString title)
{
    QMessageBox::information(parent_widget,default_title(title), message);
}

void Script_Window_Dialog::warning(QString message, QString title)
{
    QMessageBox::warning(parent_widget,default_title(title),message);
}

void Script_Window_Dialog::critical(QString message, QString title)
{
    QMessageBox::critical(parent_widget,default_title(title),message);
}

int Script_Window_Dialog::question(QString message, QString title,
                                   QString button1, QString button2, QString button3)
{
    return QMessageBox::question(parent_widget,default_title(title),message,
                                 button1,button2,button3
                                 );
}

QString Script_Window_Dialog::get_open_file(QString title, QString filters)
{
    return QFileDialog::getOpenFileName(parent_widget,default_title(title),
                                        QString(),filters
                                        );
}

QString Script_Window_Dialog::default_title(QString title)
{
    return title.isEmpty()?resource_manager().program.name():title;
}

double Script_Window_Dialog::get_number(QString message, QString title,
                                        double default_value, double min, double max)
{
    bool ok = true;
    double result = QInputDialog::getDouble(parent_widget,default_title(title),message,
                            default_value, min, max, 2, &ok);
    return ok ? result : std::numeric_limits<double>::signaling_NaN();
}

double Script_Window_Dialog::get_integer(QString message, QString title,
                                         int default_value, int min, int max)
{
    bool ok = true;
    double result = QInputDialog::getInt(parent_widget,default_title(title),message,
                            default_value, min, max, 1, &ok);
    return ok ? result : std::numeric_limits<double>::signaling_NaN();
}

QString Script_Window_Dialog::get_text(QString message, QString title, QString default_value)
{
    return QInputDialog::getText(parent_widget,default_title(title),message,
                                 QLineEdit::Normal, default_value );
}

QString Script_Window_Dialog::get_item(QString message, QString title, QStringList items)
{
    if ( items.empty() )
        return QString();

    bool ok = true;
    QString result = QInputDialog::getItem(parent_widget,default_title(title),
                                           message,items,0,false,&ok);
    return ok ? result : "";
}

QString Script_Window_Dialog::toString() const
{
    return "[window.dialog]";
}




QWidget *Script_Window_Dialog::load_widget(QString ui_file_name)
{
    QFile ui_file(ui_file_name );
    if ( ui_file.open(QFile::ReadOnly|QFile::Text) )
    {
        QUiLoader loader;

        return loader.load(&ui_file,parent_widget);
    }

    return nullptr;
}

QWidget *Script_Window_Dialog::progress_dialog(QString message, int maximum,
                                               QString cancel_button )
{
    if ( cancel_button.isEmpty() )
        cancel_button = tr("Cancel");
    return new QProgressDialog(message,cancel_button,0,maximum,parent_widget);
}

