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
#ifndef PATH_BUILDER_HPP
#define PATH_BUILDER_HPP

#include <QPainterPath>
#include "path_item.hpp"

class path_builder
{
    protected:
        typedef QList<path_item::line*> line_list;
        typedef line_list::iterator iterator;
        line_list strokes;

        path_builder(const path_builder&);
        path_builder& operator= (const path_builder&);

    public:
        path_builder();
        ~path_builder();

        void add_line ( path_item::line* l );
        void add_line( QPointF begin, QPointF end );
        void add_cubic( QPointF begin, QPointF control1, QPointF control2, QPointF end );
        void add_quad ( QPointF begin, QPointF control, QPointF end );

        QPainterPath build();
};

#endif // PATH_BUILDER_HPP
