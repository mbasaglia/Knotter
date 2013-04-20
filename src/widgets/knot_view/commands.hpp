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

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <QUndoCommand>
#include "knot_view.hpp"
#include "graph.hpp"

class Knot_Command : public QObject, public QUndoCommand
{
    Q_OBJECT

private:
    static int auto_id;
protected:
    static int generate_id();
    Knot_View*      view;
    Graph*          graph;
    QGraphicsScene* scene;

public:
    Knot_Command(Knot_View* view)
        : view(view), graph(&view->graph), scene(view->scene()) {}
    virtual void retranslate() = 0;

};

class Create_Node : public Knot_Command
{
    Q_OBJECT

    Node*          node;
public:
    Create_Node(Node* node, Knot_View* kv);
    void undo() override;
    void redo() override;
    void retranslate() override;
};

class Create_Edge : public Knot_Command
{
    Q_OBJECT

    Edge*          edge;
public:
    Create_Edge(Edge* edge, Knot_View* kv);
    void undo() override;
    void redo() override;
    void retranslate() override;
};

#endif // COMMANDS_HPP
