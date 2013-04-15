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

#ifndef GRAPH_ITEM_HPP
#define GRAPH_ITEM_HPP
#include <QObject>
#include <QPainter>
#include "point_math.hpp"
class Graph;


class Graph_Item : public QObject
{
    Q_OBJECT

public:
    Graph_Item(Graph* parent);

    virtual QRectF bounding_box() const = 0;
    virtual double distance_squared(QPointF to) const = 0;
    /**
     *  \param painter  The painter that draws the item
     *  \param hidden   Whether the item should be painted as hidden
     *  \param selected Whether the item results selected
     *  \param active   Whether to put extra emphasis
    */
    virtual void paint(QPainter *painter,
                       bool hidden, bool selected, bool active) const = 0;
};

#endif // GRAPH_ITEM_HPP
