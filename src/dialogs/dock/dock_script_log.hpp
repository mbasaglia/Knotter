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

#ifndef DOCK_SCRIPT_LOG_HPP
#define DOCK_SCRIPT_LOG_HPP

#include "ui_dock_script_log.h"
#include "script_window.hpp"


class Dock_Script_Log : public QDockWidget, private Ui::Dock_Script_Log
{
    Q_OBJECT

    Script_Window sw;
    bool          local_run;    ///< If is currently being executed code from the console itself

public:
    explicit Dock_Script_Log(Main_Window* mw);

    void set_tool_button_style(Qt::ToolButtonStyle style);
    
protected:
    void changeEvent(QEvent *e);

private:
    static QString escape_html(QString s);

private slots:
    void script_error(QString file,int line,QString msg, QStringList trace = QStringList());
    void script_output(QString text);
    void run_script(const QString &source, QString file_name, int line_number, bool echo);
    void on_button_run_clicked();
    void editor_resized(QSize sz);
};

#endif // DOCK_SCRIPT_LOG_HPP
