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

#ifndef CUSP_STYLE_DIALOG_HPP
#define CUSP_STYLE_DIALOG_HPP

#include "ui_cusp_style_dialog.h"

class Cusp_Style_Dialog : public QDialog, private Ui::Cusp_Style_Dialog
{
    Q_OBJECT
    
public:
    explicit Cusp_Style_Dialog(QWidget *parent = 0);

    void set_style(const Node_Style &style);
    Node_Style node_style() const;
    
protected:
    void changeEvent(QEvent *e);
};

#endif // CUSP_STYLE_DIALOG_HPP
