/**
\file

\author Mattia Basaglia

\section License

    Copyright (C) 2013 Mattia Basaglia
    
    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with Color Widgets.  If not, see <http://www.gnu.org/licenses/>.
    
*/

#include "dialog_download_plugin.hpp"
#include "resource_manager.hpp"
#include <QMessageBox>

Dialog_Download_Plugin::Dialog_Download_Plugin(QVariantMap plugin, QWidget *parent) :
    QDialog(parent), reply(nullptr), current_file(-1), current_progress(nullptr)
{
    setupUi(this);

    files = plugin["plugin_files"].toStringList();
    plugin_name = plugin["name"].toString();
    setWindowTitle(tr("Installing %1").arg(plugin_name));

    error = false;

    if ( files.empty() )
        error = true;
    else
    {
        current_file = 0;
        foreach ( QString f, files)
        {
            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);
            tableWidget->setItem(row,0,new QTableWidgetItem(f));
            tableWidget->setItem(row,1,new QTableWidgetItem(tr("Queued")));
        }
    }

    QString destdir = Resource_Manager::writable_data_directory("");
    if ( destdir.isEmpty() )
        error = true;
    else
    {
        destination.setPath(destdir);
        if(!destination.exists("plugins"))
        {
            if ( !destination.mkdir("plugins") )
                error = true;
        }

        if ( ! destination.cd("plugins") )
            error = true;
    }

    step(0);
}

void Dialog_Download_Plugin::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            retranslateUi(this);
            break;
        default:
            break;
    }
}

void Dialog_Download_Plugin::download_file(int file_index)
{
    current_file = file_index;
    QString file = files[current_file];

    current_progress = new QProgressBar;
    tableWidget->setCellWidget(current_file,1,current_progress);

    reply = Resource_Manager::network_get(QString(PLUGIN_REPO)+"plugins/"+file);
    connect(reply,SIGNAL(finished()),SLOT(reply_finished()));
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(reply_progress(qint64,qint64)));
    connect(reply,SIGNAL(destroyed()),SLOT(reply_destroyed()));

}

void Dialog_Download_Plugin::step(int index)
{
    if ( error )
    {
        QMessageBox::critical(this,tr("Download Error"),
                              tr("Error during the installation of %1")
                              .arg(plugin_name));
    }
    else if ( index >= files.size() )
    {
        progressBar->setValue(100);
        QMessageBox::information(this,tr("Download Complete"),
                              tr("%1 was installed successfully")
                              .arg(plugin_name));
        buttonBox->setStandardButtons(buttonBox->standardButtons()|QDialogButtonBox::Ok);

    }
    else
    {
        progressBar->setValue(index*100/files.size());
        download_file(index);
    }
}

void Dialog_Download_Plugin::reply_finished()
{
    tableWidget->setCellWidget(current_file,1,nullptr);
    current_progress = nullptr;
    bool current_error = false;
    if ( reply->error() != QNetworkReply::NoError )
    {
        current_error = true;
    }
    else
    {

        QFileInfo finfo = files[current_file];
        destination.mkpath(finfo.dir().path());
        QFile file ( destination.filePath(finfo.filePath()));
        if ( !file.open(QFile::WriteOnly) )
        {
            current_error = true;
        }
        else
        {
            file.write(reply->readAll());
        }
    }

    if ( current_error )
    {
        error = true;

        tableWidget->setItem(current_file,1,new QTableWidgetItem(
                                 QIcon::fromTheme("dialog-error"),
                                 tr("Download Error")
                            ));
    }
    else
    {
        tableWidget->setItem(current_file,1,new QTableWidgetItem(
                                 QIcon::fromTheme("dialog-ok"),
                                 tr("Download Successful")
                            ));
    }

    reply->deleteLater();
    reply = nullptr;
    step(current_file+1);
}

void Dialog_Download_Plugin::reply_progress(qint64 bytes, qint64 total)
{
    current_progress->setValue(bytes*100/total);
}

void Dialog_Download_Plugin::reply_destroyed()
{
    if ( reply == sender() )
        reply = nullptr;
}

void Dialog_Download_Plugin::on_Dialog_Download_Plugin_finished(int)
{
    if ( reply )
    {
        disconnect(reply, 0, this, 0);
        reply->abort();
        reply->deleteLater();
        reply = nullptr;
    }
}
