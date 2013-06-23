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

#ifndef SCRIPT_EDGE_HPP
#define SCRIPT_EDGE_HPP

#include <QObject>

#include "script_node.hpp"
#include "script_line.hpp"
#include "edge.hpp"


/**
 *  \brief Wrapper to Edge
 */
class Script_Edge : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject* vertex1 READ vertex1)
    Q_PROPERTY(QObject* vertex2 READ vertex2)
    Q_PROPERTY(Script_Line line READ line)
    Q_PROPERTY(Script_Point midpoint READ midpoint)
    Q_PROPERTY(QString type READ type WRITE set_type)

    Edge* wrapped;
    Script_Graph* graph;

public:
    explicit Script_Edge(Edge* wrapped, Script_Graph* graph );

    Q_INVOKABLE bool is_vertex ( const QObject* node ) const;

    Q_INVOKABLE QObject* other ( QObject* node);

    Script_Node* vertex1() const;
    Script_Node* vertex2() const;
    Script_Line line();
    Script_Point midpoint();

    Edge* wrapped_edge() const { return wrapped; }

    Q_INVOKABLE QString toString() const;

    QString type();
    void set_type(QString type_name);

signals:
    void type_changed(Edge_Type* type);
};

Q_DECLARE_METATYPE(Script_Edge*)

#endif // SCRIPT_EDGE_HPP
