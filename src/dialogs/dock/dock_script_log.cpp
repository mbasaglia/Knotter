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

#include "dock_script_log.hpp"
#include "resource_manager.hpp"
#include <QKeyEvent>
#include "main_window.hpp"
#include <QShortcut>
#include <QFileDialog>
#include <QFile>
#include <QDesktopServices>

Dock_Script_Log::Dock_Script_Log(Main_Window *mw) :
    QDockWidget(mw), target_window(mw), local_run(false)
{
    setupUi(this);
    connect(Resource_Manager::pointer,SIGNAL(script_error(QString,int,QString,QStringList)),
            SLOT(script_error(QString,int,QString,QStringList)));
    connect(Resource_Manager::pointer,SIGNAL(script_output(QString)),
            SLOT(script_output(QString)));

    connect(button_stop,SIGNAL(clicked()),Resource_Manager::pointer,SLOT(abort_script()));
    connect(Resource_Manager::pointer,SIGNAL(running_script(bool)),
            button_stop,SLOT(setEnabled(bool)));

    connect(source_editor,SIGNAL(sizeChanged(QSize)),SLOT(editor_resized(QSize)));

    foreach(Plugin* p,Resource_Manager::plugins())
    {
        if ( !p->is_valid() )
        {
            script_error(p->string_data("plugin_file"),0,p->string_data("error"));
        }
    }

    text_output->h8(mw->findChild<QAction*>("action_Copy"));

    set_tool_button_style(mw->toolButtonStyle());

    editor_resized(source_editor->size());

    connect(button_open,SIGNAL(clicked()),SLOT(open_file()));
    connect(button_save,SIGNAL(clicked()),SLOT(save_file()));
    connect(button_save_as,SIGNAL(clicked()),SLOT(save_file_as()));
    connect(button_new,SIGNAL(clicked()),SLOT(new_file()));

    connect(button_dialog,SIGNAL(toggled(bool)),SLOT(toggle_dialog(bool)));
    connect(button_external,SIGNAL(clicked()),SLOT(open_external_editor()));
}

void Dock_Script_Log::set_tool_button_style(Qt::ToolButtonStyle style)
{
    foreach(QToolButton* b, findChildren<QToolButton*>())
    {
        b->setToolButtonStyle(style);
    }
}

void Dock_Script_Log::setVisible(bool visible)
{
    if ( splitter->parentWidget() == parentWidget() )
        splitter->setVisible(visible);
    else
        QDockWidget::setVisible(visible);
}

void Dock_Script_Log::changeEvent(QEvent *e)
{
    QDockWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}


QString Dock_Script_Log::escape_html(QString s)
{
#if HAS_QT_5
        return s.toHtmlEscaped();
#else
        return Qt::escape(s);
#endif
}

void Dock_Script_Log::script_error(QString file, int line, QString msg, QStringList trace)
{
    text_output->moveCursor (QTextCursor::End) ;
    text_output->insertHtml(QString("<div><span style='color:cyan'>%1</span>:%2:"
                        "<span style='color:red'>%3</span>: %4<br/>%5</div>")
                            .arg(escape_html(file))
                            .arg(line)
                            .arg(tr("Error"))
                            .arg(escape_html(msg))
                            .arg(trace.empty()?"":"Stack trace:")
                        );
    if ( !trace.empty() )
    {
        QString trace_ul = "<ul style='margin-top:0; padding-top:0'>";

        foreach ( QString s , trace )
        {
            trace_ul += "<li>"+escape_html(s)+"</li>";
        }
        trace_ul += "</ul>";

        text_output->insertHtml(trace_ul+"<p></p>");
    }

    text_output->moveCursor (QTextCursor::End) ;
    text_output->ensureCursorVisible() ;

    if ( local_run || file == filename )
    {
        source_editor->error_line( line );
    }


}

void Dock_Script_Log::script_output(QString text)
{
    text_output->moveCursor (QTextCursor::End) ;
    text_output->insertHtml("<pre>"+escape_html(text)+"</pre><p></p>");
    text_output->moveCursor (QTextCursor::End) ;
    text_output->ensureCursorVisible() ;
}

void Dock_Script_Log::run_script(const QString &source, QString file_name,
                                 int line_number, bool echo)
{

    text_output->moveCursor (QTextCursor::End) ;

    if ( echo )
    {
        text_output->insertHtml("<div style='color:green; white-space:pre;'>"
                                +escape_html(source)
                                +"</div><p></p>");
    }

    Script_Window sw(target_window);
    Resource_Manager::script_param("window",&sw);
    Resource_Manager::script_param("document",sw.document());


    QScriptValue v = Resource_Manager::run_script(source,file_name,line_number);

    if ( !v.isError() && !v.isUndefined() )
        text_output->insertHtml(escape_html(v.toString())+"<p></p>");
    text_output->moveCursor (QTextCursor::End) ;
    text_output->ensureCursorVisible() ;
}

void Dock_Script_Log::on_button_run_clicked()
{
    local_run = true;
    run_script(source_editor->toPlainText(),tr("Script Editor"),1,false);
    local_run = false;
}

void Dock_Script_Log::editor_resized(QSize sz)
{
    if ( sz.height() <= 24 )
    {
        frame_editor_buttons->hide();
        button_run_2->show();
    }
    else if ( sz.height() > 24 + frame_editor_buttons->height() +
                                    frame_advanced->layout()->spacing() )
    {
        button_run_2->hide();
        frame_editor_buttons->show();
    }
    else if ( frame_editor_buttons->isVisible() )
    {
        button_run_2->hide();
    }
    else
    {
        button_run_2->show();
    }

    button_run->setVisible(!button_run_2->isVisible());
}

void Dock_Script_Log::new_file()
{
    source_editor->clear();
    filename = "";
}

void Dock_Script_Log::open_file()
{
    QString new_file = QFileDialog::getOpenFileName(this,tr("Open Script"),
        filename,"JavaScript Sources (*.js *.es);;JSON Files (*.json);;All Files (*)");

    if ( !new_file.isEmpty() )
    {
        open_script_file(new_file);
    }
}

void Dock_Script_Log::save_file()
{
    if ( filename.isEmpty() )
    {
        save_file_as();
    }
    else
        save_file(filename);
}

void Dock_Script_Log::save_file_as()
{
    QString new_file = QFileDialog::getSaveFileName(this,tr("Save Script"),
         filename,"JavaScript Sources (*.js *.es);;JSON Files (*.json);;All Files (*)");

    if ( !new_file.isEmpty() )
    {
        save_file(new_file);
    }
}


void Dock_Script_Log::save_file(QString file_name)
{
    filename = file_name;
    QFile file(filename);
    file.open(QFile::Text|QFile::WriteOnly);
    file.write( source_editor->toPlainText().toUtf8() );
}

void Dock_Script_Log::toggle_dialog(bool dialog)
{
    if ( dialog )
    {
        //setWindowFlags(windowFlags()|Qt::Dialog);
        //show();
        hide();
        splitter->setParent(parentWidget(),Qt::Dialog);
        splitter->move(x(),y());
        splitter->show();
    }
    else
    {
        //setWindowFlags(windowFlags()&~Qt::Dialog);
        //show();
        splitter->setParent(widget(),Qt::Widget);
        widget()->layout()->addWidget(splitter);
        splitter->show();
        show();
        raise();
    }
}

void Dock_Script_Log::open_external_editor()
{
    if ( !filename.isEmpty() )
    {
        QDesktopServices::openUrl("file:///"+filename);
    }
}


void Dock_Script_Log::open_script_file(QString new_file)
{
    filename = new_file;
    source_editor->clear();
    QFile file(filename);
    file.open(QFile::Text|QFile::ReadOnly);
    source_editor->setPlainText(file.readAll());
    show();
}
