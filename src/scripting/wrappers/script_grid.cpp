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

#include "script_grid.hpp"
#include <QMetaEnum>

Script_Grid::Script_Grid(Snapping_Grid *wrapped, QObject *parent) :
    QObject(parent), wrapped(wrapped)
{
}

QString Script_Grid::shape() const
{
    switch(wrapped->shape())
    {
        case Snapping_Grid::SQUARE:
            return "SQUARE";
        case Snapping_Grid::TRIANGLE1:
            return "TRIANGLE1";
        case Snapping_Grid::TRIANGLE2:
            return "TRIANGLE2";
        default:
            return QString();
    }
}

void Script_Grid::set_shape(QString s)
{
    s = s.toUpper();
    if ( s == "SQUARE" )
        wrapped->set_shape(Snapping_Grid::SQUARE);
    else if ( s == "TRIANGLE1" )
            wrapped->set_shape(Snapping_Grid::TRIANGLE1);
    else if ( s == "TRIANGLE2" )
            wrapped->set_shape(Snapping_Grid::TRIANGLE2);
}

QString Script_Grid::toString() const
{
    return "[grid]";
}

void Script_Grid::enable()
{
    wrapped->enable(true);
}

void Script_Grid::disable()
{
    wrapped->enable(false);
}
