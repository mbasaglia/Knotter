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

#ifndef SCRIPT_GRID_HPP
#define SCRIPT_GRID_HPP

#include <QObject>
#include "snapping_grid.hpp"
#include "script_point.hpp"

class Script_Grid : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double size READ size WRITE set_size)
    Q_PROPERTY(Script_Point origin READ origin WRITE set_origin)
    Q_PROPERTY(bool enabled READ enabled WRITE set_enabled)
    Q_PROPERTY(QString shape READ shape WRITE set_shape)


    Snapping_Grid* wrapped;
public:
    explicit Script_Grid(Snapping_Grid* wrapped, QObject *parent = 0);

    double size() const { return wrapped->size(); }
    void set_size(double size) { wrapped->set_size(size); }

    Script_Point origin() const { return wrapped->origin(); }
    void set_origin(Script_Point o)  { wrapped->set_origin(o); }

    bool enabled() const { return wrapped->is_enabled(); }
    void set_enabled(bool e) { wrapped->enable(e); }

    QString shape() const;
    void set_shape(QString s);

    Q_INVOKABLE QString toString() const;

    Q_INVOKABLE void enable();
    Q_INVOKABLE void disable();
};

#endif // SCRIPT_GRID_HPP
