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
#include "clipboard.hpp"

ClipboardItem::ClipboardItem ( node_list insert_nodes )
{
    if ( !insert_nodes.empty() )
    {
        foreach(Node* n, insert_nodes)
            center += n->pos();
        center /= insert_nodes.size();

        QMap<Node*,int> ids;
        int next_id = 1;
        foreach(Node* n, insert_nodes)
        {
            nodes.push_back(ClipboardNode(next_id,n->pos()-center));
            ids[n] = next_id;
            next_id++;
        }

        foreach(Node* n, insert_nodes)
        {
            foreach ( Edge* e, n->links() )
            {
                int id1 = ids.value(e->vertex1(),0);
                int id2 = ids.value(e->vertex2(),0);
                if ( id1 == 0 || id2 == 0 )
                    continue; // edge with a vertex outside the selection

                // avoid redundancies
                if ( id1 > id2 )
                    qSwap(id1,id2);
                ClipboardEdge e ( id1, id2 );
                if ( !edges.contains(e) )
                    edges.push_back(e);
            }
        }
    }
}

void ClipboardItem::paste(KnotView *kv, QPointF p) const
{
    if ( !kv ) return;

    kv->get_undo_stack().beginMacro(KnotView::tr("Paste"));

    kv->scene()->clearSelection();

    QMap<int, Node*> ids;
    foreach(ClipboardNode n, nodes)
    {
        ( ids[n.id] = kv->add_node(n.pos+p) ) -> setSelected(true);
    }


    foreach ( ClipboardEdge e, edges )
    {
        kv->add_edge(ids[e.n1],ids[e.n2]);
    }

    kv->get_undo_stack().endMacro();

    kv->mode_moving_new();
}

void ClipboardItem::copy(KnotView *kv)
{
    *this = ClipboardItem(kv->selected_nodes());
}


