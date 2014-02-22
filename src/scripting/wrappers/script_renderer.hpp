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

#ifndef SCRIPT_RENDERER_HPP
#define SCRIPT_RENDERER_HPP

#include <QObject>
#include "graph.hpp"
#include "script_color.hpp"

/**
 * Document renderer
 */
class Script_Renderer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool draw_graph READ draw_graph WRITE set_draw_graph)

private:
    const Graph* graph;
    bool m_draw_graph;

public:
    Script_Renderer(const Graph* graph) : graph(graph), m_draw_graph(false) {}

    bool draw_graph() const { return m_draw_graph; }
    void set_draw_graph( bool draw ) { m_draw_graph = draw; }

    /**
     * \brief Render as knot file (XML)
     */
    Q_INVOKABLE QString knot();
    /**
     * \brief Render as SVG
     */
    Q_INVOKABLE QString svg();
    /**
     * \brief Render as raster image
     * \param width     Image width
     * \param height    Image height
     * \param format    Name of the output format
     * \param quality   Quality/Compression (See QPixmap::save())
     * \param background Background color
     */
    Q_INVOKABLE QByteArray raster(int width = 0,
                                  int height = 0,
                                  QString format = "PNG",
                                  int quality = 0,
                                  Script_Color background = Script_Color());

    Q_INVOKABLE QString toString();
};


#endif // SCRIPT_RENDERER_HPP
