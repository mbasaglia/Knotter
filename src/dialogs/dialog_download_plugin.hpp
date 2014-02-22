/**
\file

\author Mattia Basaglia

\section License

    Copyright (C) 2013-2014 Mattia Basaglia
    
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

#ifndef DIALOG_DOWNLOAD_PLUGIN_HPP
#define DIALOG_DOWNLOAD_PLUGIN_HPP

#include "ui_dialog_download_plugin.h"
#include <QNetworkReply>
#include <QDir>

class Dialog_Download_Plugin : public QDialog, private Ui::Dialog_Download_Plugin
{
    Q_OBJECT

    QNetworkReply* reply;
    QDir destination;
    QString plugin_name;
    QStringList files;
    int current_file;
    QProgressBar* current_progress;
    bool error;
    
public:
    explicit Dialog_Download_Plugin(QVariantMap plugin, QWidget *parent = 0);
    
protected:
    void changeEvent(QEvent *e);

private:
    void download_file(int file_index);

    void step(int index);

private slots:
    void reply_finished();
    void reply_progress(qint64 bytes, qint64 total);
    void reply_destroyed();
    void on_Dialog_Download_Plugin_finished(int result);
};

#endif // DIALOG_DOWNLOAD_PLUGIN_HPP
