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
#ifndef CONTEXT_MENU_HPP
#define CONTEXT_MENU_HPP

#include <QMenu>
class Node;
class Edge;
class KnotView;

/**
    \brief Node context menu
*/
class node_cxmn : public QMenu
{
        Q_OBJECT

    protected:
        Node* n;
        KnotView* kv;
        QAction* snap_grid;
        QAction* unset_style;
    public:
        node_cxmn ( QWidget * parent = 0 );

        void set_view ( KnotView* k_v );

    public slots:
        void activate ( Node* node );

    private slots:
        void snap();
        void properties();
        void no_custom_style();
        void remove();
    signals:
        void request_properties(Node*);
};

/**
    \brief Edge context menu
*/
class edge_cxmn : public QMenu
{
        Q_OBJECT

    protected:
        Node* n1;
        Node* n2;
        KnotView* kv;
        QAction* snap_grid;
        QMap<int,QAction*> type_action;

    public:
        edge_cxmn ( QWidget * parent = 0 );

        void set_view ( KnotView* k_v );

    public slots:
        void activate ( Edge* edge );

    private slots:
        void snap();
        void properties();
        void remove();
        void set_regular();
        void set_inverted();
        void set_wall();
        void set_hole();
        void break_edge_intersections();
        void break_edge_equal();

    signals:
        void request_properties(Edge*);

};

#endif // CONTEXT_MENU_HPP
