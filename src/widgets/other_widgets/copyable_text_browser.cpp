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

#include "copyable_text_browser.hpp"
#include <QKeyEvent>
#include <QAction>

Copyable_Text_Browser::Copyable_Text_Browser(QWidget *parent) :
    QTextBrowser(parent),
    copy_sc(new QShortcut(QKeySequence::Copy,this,SLOT(copy()),SLOT(copy()),Qt::WidgetWithChildrenShortcut)),
    ih8u(nullptr)
{
    //copy_sc->setEnabled(false);
}


void Copyable_Text_Browser::focusInEvent(QFocusEvent *)
{
    if ( ih8u ) ih8u->setEnabled(false);
}

void Copyable_Text_Browser::focusOutEvent(QFocusEvent *)
{
    if ( ih8u ) ih8u->setEnabled(true);
}
