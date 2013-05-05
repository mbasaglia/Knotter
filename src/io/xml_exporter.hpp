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

#ifndef XML_EXPORTER_HPP
#define XML_EXPORTER_HPP
#include <QXmlStreamWriter>
#include "graph.hpp"
#include <QMimeData>

class XML_Exporter : public QObject
{
    /**
     * \note Inherits QObject to benefit from the black magic of staticQtMetaObject
     */
    Q_OBJECT


    /// Version of the Knot file format
    static const int version = 3;

    QXmlStreamWriter xml;
    QMap<Node*,int> node_ids;

public:
    XML_Exporter(QIODevice* output);

    /// Create xml document
    void export_graph(const Graph *graph);

protected:
    void begin ();
    void end();

    void start_element ( QString name );
    void end_element ();

    void save_style (const Graph *graph );

    void save_cusp ( QString name, Node_Style style );

    void save_node ( Node* node );

    void save_edge ( Edge* edge );

    void save_color(QString name, QColor col);

    /**
     *  \brief Get the unique ID associated with the given node
     */
    int node_id ( Node* node );
};


bool export_xml(const Graph& graph, QIODevice &file );

void export_xml_mime_data(const Graph& graph, QMimeData* data);

#endif // XML_EXPORTER_HPP
