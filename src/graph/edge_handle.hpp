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

#ifndef EDGE_HANDLE_HPP
#define EDGE_HANDLE_HPP

#include <QFlags>

enum Edge_Handle
{
    NO_HANDLE   = 0x00,
    TOP_LEFT    = 0x01,
    TOP_RIGHT   = 0x02,
    BOTTOM_LEFT = 0x04,
    BOTTOM_RIGHT= 0x08
};

Q_DECLARE_FLAGS(Handle_Flags,Edge_Handle)

#endif // EDGE_HANDLE_HPP
