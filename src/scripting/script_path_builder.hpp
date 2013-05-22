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

#ifndef SCRIPT_PATH_BUILDER_HPP
#define SCRIPT_PATH_BUILDER_HPP
#include "path_builder.hpp"
#include <QObject>
#include "script_point.hpp"

/**
    Wrapper to Path_Builder
*/
class Script_Path_Builder : public QObject
{
    Q_OBJECT

    protected:
        Path_Builder* pb;

    public:
        Script_Path_Builder(Path_Builder* pb) : pb(pb) {}

        Q_INVOKABLE void add_line( Script_Point begin, Script_Point end )
        {
            pb->add_line(begin,end);
        }
        Q_INVOKABLE void add_cubic( Script_Point begin, Script_Point control1, Script_Point control2, Script_Point end )
        {
            pb->add_cubic(begin,control1,control2,end);
        }
        Q_INVOKABLE void add_quad ( Script_Point begin, Script_Point control, Script_Point end )
        {
            pb->add_quad(begin,control,end);
        }

        Q_INVOKABLE void new_group()
        {
            pb->new_group();
        }
};

#endif // SCRIPT_PATH_BUILDER_HPP
