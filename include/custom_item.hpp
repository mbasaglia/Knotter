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
#ifndef CUSTOM_ITEM_HPP
#define CUSTOM_ITEM_HPP


#include <QGraphicsItem>
#include "grid_scene.hpp"

class Edge;
class Node;

class CustomItem : public QGraphicsItem
{

    public:
        bool highlight;
        explicit CustomItem ( bool highlight = false ) : highlight ( highlight ) {}

        bool is_visible() const
        {
            GridScene *gs = dynamic_cast<GridScene*>(scene());
            return gs && gs->show_graph;
        }
};

#endif // CUSTOM_ITEM_HPP
