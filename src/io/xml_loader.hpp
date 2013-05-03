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

#ifndef XML_LOADER_HPP
#define XML_LOADER_HPP

#include <QDomDocument>
#include "graph.hpp"

class XML_Loader
{

    QMap<QString,Node*> nodes;
    QDomDocument xml;
    int m_version;
    static const int accept_version = 3;

    /**
     *  \brief get node with given ID
     *
     *  If no node is found, a new one is created
     */
    Node* node(QString id);

    Node_Style get_node_style ( QDomElement element );

public:

    /**
     *  \brief Load file
     *
     *  \param input    Input file
     *
     *  \return True if file has been loaded correctly
     *
     *  \post if the file is a Knot file versions() returns the file version
     */
    bool load(QIODevice *input);

    /**
     *  \brief Load graph from XML
     *
     *  \param[out] graph   Graph into which the file will be loaded
     *
     *  \return True if the graph has been loaded correctly
     *
     *  \pre load() has been called successfully
     *  \post If the input file is valid, its contents will be appended to the graph
     */
    bool load_graph(Graph* graph);

    /**
     * \brief Version of the oaded Knot file
     */
    int version();




};

#endif // XML_LOADER_HPP
