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

#ifndef CONTEXT_MENU_NODE_HPP
#define CONTEXT_MENU_NODE_HPP

#include <QMenu>
#include "knot_view.hpp"

class Context_Menu_Node : public QMenu
{
    Q_OBJECT
private:
    Knot_View* view;
    Node*      node;
public:
    explicit Context_Menu_Node(Knot_View *parent = 0);
    
signals:
    
public slots:
    
};

#endif // CONTEXT_MENU_NODE_HPP
