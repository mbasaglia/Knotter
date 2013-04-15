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

#ifndef GRAPHICS_ITEM_HPP
#define GRAPHICS_ITEM_HPP

#include <QGraphicsItem>
#include "graph_item.hpp"

class Graphics_Item : public QGraphicsItem
{
    enum Internal_Type
    {
        Type_Node = UserType + 0x01,
        Type_Edge = UserType + 0x02
    };

    Graph_Item *    item;
    bool            highlighted;
    Internal_Type   int_type;


public:
    Graphics_Item(Graph_Item *item);

    QRectF boundingRect() const { return item->bounding_box(); }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
    {
        item->paint(painter,!isVisible(),isSelected(),highlighted);
    }

    int type() const { return int(int_type); }
};

#endif // GRAPHICS_ITEM_HPP
