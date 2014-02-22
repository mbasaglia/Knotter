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

#ifndef DIALOG_CONFIRM_CLOSE_HPP
#define DIALOG_CONFIRM_CLOSE_HPP

#include "ui_dialog_confirm_close.h"
#include <QSignalMapper>


class Dialog_Confirm_Close : public QDialog, private Ui::Dialog_Confirm_Close
{
    Q_OBJECT

    /**
     * \brief Simple helper class
     */
    struct Save_File
    {
        QString file_name;
        bool save;
        Save_File(QString file_name="") : file_name(file_name), save(true) {}
    };

    QMap<int,Save_File> files;
    QSignalMapper mapper;
    
public:
    static const int DontSave = Accepted+1;

    explicit Dialog_Confirm_Close(QWidget *parent = 0);

    /**
     * \brief Insert file to be saved
     * \param index  An identifier used by the caller to associate to the file
     * \param name   File name
     * \post  has_file() == true
     * \note  \c index should be unique, for a single file
     */
    void add_file(int index, QString name);

    bool has_files() const { return !files.empty(); }

    /**
     * \brief Get files to be saved
     * \return A list of indices representing the files that need to be saved
     */
    QList<int> save_files();
    
protected:
    void changeEvent(QEvent *e);

private slots:
    void file_toogled(int i);
    void on_button_dont_save_clicked();
};

#endif // DIALOG_CONFIRM_CLOSE_HPP
