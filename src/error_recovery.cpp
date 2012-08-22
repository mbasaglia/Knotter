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

void ErrorRecovery::sigsegv(int sig)
{
    if ( sig == SIGSEGV )
    {
        QString moreinfo;

        if ( recover )
        {
            QFile tempfile( QDir::tempPath()+"/knotter_crash.knot");
            if ( tempfile.open(QIODevice::WriteOnly | QIODevice::Text) )
            {
                recover->writeXML(&tempfile);
                if ( !tempfile.error() )
                    moreinfo = '\n'+
                        QObject::tr("A backup file has been saved at %1.").
                            arg(tempfile.fileName());
            }
        }

        QMessageBox::critical(0,QObject::tr("Segmentation Fault"),
            QObject::tr("A critical error has occurred, Knotter is about to crash.")
            + moreinfo );
    }
    exit(1);
}

void ErrorRecovery::initialize()
{
    std::signal(SIGSEGV,ErrorRecovery::sigsegv);
}


KnotView* ErrorRecovery::recover;
