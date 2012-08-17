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
