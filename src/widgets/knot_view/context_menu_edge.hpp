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

#ifndef CONTEXT_MENU_EDGE_HPP
#define CONTEXT_MENU_EDGE_HPP

#include <QMenu>
#include "knot_view.hpp"
#include <QSignalMapper>

class Context_Menu_Edge : public QMenu
{
    Q_OBJECT
private:
    Knot_View*      view;
    Edge*           edge;
    QAction*        action_snap;
    QMenu*          menu_edge_types;
    QSignalMapper   mapper;
    QActionGroup*   edge_type_actions;
    QAction*        action_reset_style;

public:
    explicit Context_Menu_Edge(Knot_View *parent = 0);

    void popup(Edge*e, QPoint pos);

public slots:
    void snap();
    void change_edge_type(QString type_name);
    void remove();
    void break_intersections();
    void subdivide();
    void properties();
    void reset_custom_style();
    
};

#endif // CONTEXT_MENU_EDGE_HPP
