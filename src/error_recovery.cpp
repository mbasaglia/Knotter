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
#include "error_recovery.hpp"
#include <csignal>
#include <QMessageBox>
#include <QDir>

void ErrorRecovery::sigdying(int)
{

    QString moreinfo;

    QString file_names;
    int n = 0;
    foreach ( KnotGraph * graph, recover )
    {
        n++;
        QFile tempfile( QDir::tempPath()+"/knotter_crash/tab_"+QString::number(n)+".knot");
        if ( tempfile.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            graph->save_xml(&tempfile);
            if ( !tempfile.error() )
                file_names += '\n' + tempfile.fileName();
        }
    }
    if ( !file_names.isEmpty() )
        moreinfo = '\n'+
            QObject::tr("The following backup files have been saved:").
                arg(file_names);

    QMessageBox::critical(0,QObject::tr("Critical error"),
        QObject::tr("A critical error has occurred, Knotter is about to crash.")
        + moreinfo );

    exit(1);
}

void ErrorRecovery::initialize()
{
    std::signal(SIGSEGV,ErrorRecovery::sigdying);
    std::signal(SIGTERM,ErrorRecovery::sigdying);
    std::signal(SIGABRT,ErrorRecovery::sigdying);
}

void ErrorRecovery::insert(KnotGraph * graph)
{
    if ( graph && !recover.contains(graph) )
        recover.push_back(graph);

}

void ErrorRecovery::remove(KnotGraph *graph)
{
    recover.removeAll(graph);
}


QList<KnotGraph*> ErrorRecovery::recover;
