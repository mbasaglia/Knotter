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
#include "help_view.hpp"
#include <QDesktopServices>
#include "resource_loader.hpp"
#include "translator.hpp"


#include <QDir>
QStringList test_manual ( QString base_dir, QString name )
{
    QStringList attempts;



    if ( base_dir.startsWith(":/") ) // resource
    {
        attempts << "qrc"+base_dir+'/'+name + " (resource)";
    }

    QDir path ( base_dir ); // install dir

    attempts << "file://"+QDir::cleanPath(path.absoluteFilePath(name)) + " (install dir)";

    if ( !path.exists(name) )
    {
        path.setPath( QCoreApplication::applicationDirPath() ); // executable dir
        attempts << "file://"+QDir::cleanPath(path.absoluteFilePath(name)) + " (executable dir)";
    }

    if ( !path.exists(name) )
    {
        path.setPath( QDir::currentPath() ); // current dir
        attempts << "file://"+QDir::cleanPath(path.absoluteFilePath(name)) + " (current dir)";
    }


    if ( !path.exists(name) )
    {
        attempts << "(Not found)";
    }

    return attempts;
}

Help_View::Help_View(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    QUrl manual = load::resource_url(DOC_DIR,"user_guide/user_guideff.htm");

    QStringList test = test_manual(DOC_DIR,"user_guide/user_guideff.htm");
    QString test_string;
    foreach(QString s, test)
        test_string+="<li>"+s+"</li>";

    web_view->setHtml("<html><head><title></title><body>"+
                        tr("Knotter manual not loaded")+
                        "<br/>Attempting to load manual from:<ul>"+
                        test_string+
                        "</ul></body></html>");

    #ifndef NO_WEBKIT
        web_view->load( manual );
        web_view->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
        connect(web_view,SIGNAL(linkClicked(QUrl)),SLOT(link_clicked(QUrl)));
    #else
        web_view->setSource ( manual );
        web_view->setOpenExternalLinks(true);
        //connect(web_view,SIGNAL(anchorClicked(QUrl)),SLOT(link_clicked(QUrl)));
    #endif

    connect(&Translator::object,SIGNAL(language_changed()),SLOT(retranslate()));
}

void Help_View::link_clicked(const QUrl &arg1)
{
    if ( arg1.scheme() != "qrc" && arg1.scheme() != "file" )
        QDesktopServices::openUrl(arg1);
}

void Help_View::retranslate()
{
    retranslateUi(this);
}

