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

class XML_Loader : QObject
{
    Q_OBJECT

    QMap<QString,Node*> nodes;
    QDomDocument xml;
    int m_version;

    static const int min_version = 3;
    static const int max_version = 3;

    Node* get_node(QDomElement element);


    Edge* get_edge(QDomElement element);

    /**
     *  \brief Parse /knot/style/cusp or //node/shape
     *  \param element      Element containing the data
     *  \param everything   If true will always enable all styles
     */
    Node_Style get_node_style ( QDomElement element, bool everything );

    /// Parse /knot/style
    void get_style(QDomElement element, Graph* graph);

    /// Parse /knot/graph
    void get_graph(QDomElement element, Graph* graph);

    QColor get_color(QDomElement e);


public:
    XML_Loader() : m_version(0) {}

    /**
     *  \brief Load file
     *
     *  \param input    Input file
     *  \param[out] graph   Graph into which the file will be loaded
     *
     *  \return True if file has been loaded correctly
     *
     *  \post If the file is a Knot file versions() returns the file version
     *  \post If the input file is valid, its contents will be appended to the graph
     */
    bool load(QIODevice *input, Graph* graph);

    /**
     * \brief Version of the loaded Knot file
     *
     *  If the loaded file is not recognised as a Knot file it will return 0
     */
    int version();
};

bool import_xml(QIODevice* file, Graph& graph);

#endif // XML_LOADER_HPP
