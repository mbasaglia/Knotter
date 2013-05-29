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

Dock_Script_Log::Dock_Script_Log(QWidget *parent) :
    QDockWidget(parent), current_line(0)
{
    setupUi(this);
    connect(Resource_Manager::pointer,SIGNAL(script_error(QString,int,QString,QStringList)),
            SLOT(script_error(QString,int,QString,QStringList)));
    connect(Resource_Manager::pointer,SIGNAL(script_output(QString)),
            SLOT(script_output(QString)));
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
/*
void Dock_Script_Log::keyPressEvent(QKeyEvent *ev)
{
    if ( ev->key() == Qt::UpArrow && current_line > 0)
    {
        current_line--;
        if ( current_line < user_input.size() )
            script_input->setText(user_input[current_line]);

    }
    else if ( ev->key() == Qt::DownArrow )
    {
        current_line++;
        if ( current_line >= user_input.size() )
            script_input->clear();
        else
            script_input->setText(user_input[current_line]);
    }
}
*/
void Dock_Script_Log::script_error(QString file, int line, QString msg, QStringList trace)
{
    text_output->moveCursor (QTextCursor::End) ;
    text_output->insertHtml(QString("<div><span style='color:cyan'>%1</span>:%2:"
                        "<span style='color:red'>%3</span>: %4<br/>%5</div>")
                            .arg(file).arg(line).arg(tr("Error")).arg(msg)
                            .arg("Stack trace:")
                        );
    QString trace_ul = "<ul style='margin-top:0; padding-top:0'>";

    foreach ( QString s , trace )
    {
        trace_ul += "<li>"+Qt::escape(s)+"</li>";
    }
    trace_ul += "</ul>";

    text_output->insertHtml(trace_ul+"<p></p>");
    text_output->moveCursor (QTextCursor::End) ;
    text_output->ensureCursorVisible() ;


}

void Dock_Script_Log::script_output(QString text)
{
    text_output->moveCursor (QTextCursor::End) ;
    text_output->insertHtml("<pre>"+text+"</pre><p></p>");
    text_output->moveCursor (QTextCursor::End) ;
    text_output->ensureCursorVisible() ;
}

void Dock_Script_Log::on_script_input_returnPressed()
{
    text_output->moveCursor (QTextCursor::End) ;
    text_output->insertHtml("<div style='color:green'>"+script_input->text()+"</div><p></p>");
    user_input += script_input->text();
    current_line = user_input.size();
    QScriptValue v = Resource_Manager::run_script(script_input->text(),"Script console",user_input.size());
    script_input->clear();
    if ( !v.isError() && !v.isUndefined() )
        text_output->insertHtml(v.toString()+"<p></p>");
    text_output->moveCursor (QTextCursor::End) ;
    text_output->ensureCursorVisible() ;
}
