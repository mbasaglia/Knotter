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
#ifndef GRID_VIEW_HPP
#define GRID_VIEW_HPP
#include <QGraphicsScene>
#include "snapping_grid.hpp"
class GridScene : public QGraphicsScene
{
    public:
        snapping_grid* grid;
        bool           show_graph;

        GridScene ( snapping_grid* grid ) : grid ( grid ), show_graph(true) {}

    protected:
        void drawBackground(QPainter *painter, const QRectF &rect)
        {
            if ( grid )
                grid->render(painter,rect);
        }
};

#endif // GRID_VIEW_HPP
