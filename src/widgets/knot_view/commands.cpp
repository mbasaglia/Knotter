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

#include "commands.hpp"

int Knot_Command::auto_id = 0;


int Knot_Command::generate_id()
{
    return auto_id++;
}


Create_Node::Create_Node(Node *node, Knot_View *kv)
    : Knot_Command(kv), node(node), item(new Graphics_Item(node))
{
    retranslate();
}

Create_Node::~Create_Node()
{
    delete item;
}

void Create_Node::undo()
{
    scene->removeItem(item);
    graph->remove_node(node);

}

void Create_Node::redo()
{
    scene->addItem(item);
    graph->add_node(n);
}

void Create_Node::retranslate()
{
    setText(tr("Create Node"));
}

