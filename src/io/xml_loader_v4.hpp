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

#ifndef XML_LOADER_V4_HPP
#define XML_LOADER_V4_HPP


#include <QDomDocument>
#include "graph.hpp"

class XML_Loader_v4 : QObject
{
    Q_OBJECT

    QMap<QString,Node*> nodes;
    QDomDocument xml;
    int m_version;

    static const int min_version = 4;
    static const int max_version = 4;

    Node* get_node(QDomElement element);


    Edge* get_edge(QDomElement element);

    /**
     *  \brief Parse /knot/style/cusp or //node/style
     *  \param element      Element containing the data
     *  \param everything   If true will always enable all styles
     */
    Node_Style get_node_style ( QDomElement element, bool everything );


    /**
     *  \brief Parse /knot/style/crossing or //edge/style
     *  \param element      Element containing the data
     *  \param everything   If true will always enable all styles
     */
    Edge_Style get_edge_style( QDomElement element, bool everything );

    /// Parse /knot/style
    void get_style(QDomElement element, Graph* graph);

    /// Parse /knot/graph
    void get_graph(QDomElement element, Graph* graph);

    QColor get_color(QDomElement e);


public:
    XML_Loader_v4() : m_version(0) {}

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
     * @brief Load style from XML
     * @param input  Open device containing the <style> element
     * @param graph  Output graph
     */
    void load_style(QIODevice *input, Graph* graph);

    /**
     * \brief Version of the loaded Knot file
     *
     *  If the loaded file is not recognised as a Knot file it will return 0
     */
    int version();


    /**
     * \brief Check if a given version is supported by this loades
     * \param v The version to check
     * \return Whether the version is supported
     */
    static bool supports_version(int v) { return v >= min_version && v <= max_version; }
};

#endif // XML_LOADER_V4_HPP
