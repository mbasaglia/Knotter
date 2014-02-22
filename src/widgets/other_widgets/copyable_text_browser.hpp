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

#ifndef COPYABLE_TEXT_BROWSER_HPP
#define COPYABLE_TEXT_BROWSER_HPP

#include <QTextBrowser>
#include "c++.hpp"
#include <QShortcut>

class Copyable_Text_Browser : public QTextBrowser
{
    Q_OBJECT

    QShortcut *copy_sc;

    QAction* ih8u;
public:
    void h8(QAction *h8ted);
    explicit Copyable_Text_Browser(QWidget *parent = 0);
    
protected:
    void focusInEvent(QFocusEvent *);
    void focusOutEvent(QFocusEvent *);

private slots:
    void h8NoMore();
};

#endif // COPYABLE_TEXT_BROWSER_HPP
