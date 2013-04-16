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
#include "graphics_item.hpp"
#include "graph.hpp"

class Knot_Command : public QUndoCommand
{
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
    virtual ~Knot_Command(){}
    virtual void retranslate() = 0;

};

class Create_Node : public Knot_Command
{
    Node*          node;
    Graphics_Item* item;
public:
    Create_Node(Node* node, Knot_View* kv);
    ~Create_Node();
    void undo();
    void redo();
    void retranslate();
};


#endif // COMMANDS_HPP
