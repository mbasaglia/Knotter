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

#ifndef KNOT_TOOL_HPP
#define KNOT_TOOL_HPP

#include "graph.hpp"
#include <QGraphicsScene>
#include <QMouseEvent>
#include <qcursor.h>

class Knot_View;

/**
 * @brief Contain the parameters rewuired to perform a tool event
 */
struct Mouse_Event
{
    QPointF mouse_pos;  ///< Mouse position in the scene
    QPointF snapped_pos;///< Mouse position, snapped to the grid
    QMouseEvent *event; ///< Raw mouse event

    Mouse_Event(QPointF mouse_pos, QPointF snapped_pos, QMouseEvent *event)
        : mouse_pos(mouse_pos), snapped_pos(snapped_pos), event(event) {}
};

class Knot_Tool
{
protected:
    Knot_View * view;
    Graph     * graph;
    bool        m_enabled;

public:
    explicit Knot_Tool(Knot_View* view, Graph* graph = nullptr);
    virtual ~Knot_Tool() {}

    //void set_view(Knot_View* view) { this->view = view; }
    void set_graph(Graph* graph) { this->graph = graph; }

    /**
     * @brief Mouse press
     * @param event Mouse event
     * @return Whether the tool wants to perform an operation.
     *         If false the view will perform the rubberband selection
     */
    virtual bool press ( const Mouse_Event& event ) = 0;
    /**
     * @brief Mouse move
     * @param event            Mouse event
     * @param [out] notify_pos The mouse position that will be notified to the user,
     *                          Unchanged means mouse_pos
     */
    virtual void move ( const Mouse_Event& event, QPointF& notify_pos ) = 0;

    /**
     * @brief Mouse release
     *
     *  Default behaviour is no-op
     *
     * @param event  Mouse event
     */
    virtual void release ( const Mouse_Event& event );

    /**
     * @brief Mouse double click
     *
     *  Default behaviour is no-op
     *
     * @param event  Mouse event
     */
    virtual void double_click ( const Mouse_Event& event );

    /**
     * @brief Enable or diable the tool
     * @param e Whether to enable or diable the tool
     */
    void enable(bool enable);

    bool enabled() const { return m_enabled; }

    /**
     * @brief Cursor for this tool
     */
    virtual QCursor cursor() const { return Qt::ArrowCursor; }

protected:
    /**
     * @brief Finalize use of the tool
     *
     *  This is called when the tool is disabled
     */
    virtual void deactivate() {}

    /**
     * @brief Initialize usage of the tool
     *
     *  This is called when the tool is enabled
     */
    virtual void activate() {}

    /**
     * @brief Get node at position
     * @param pos Scene position
     * @return The node at that position or nullprt if not found
     * \remarks Don't modify the node directly, use knot commands instead
     */
    Node* node_at(QPointF pos)  const ;
    /**
     * @brief Get edge at position
     * @param pos Scene position
     * @return The edge at that position or nullprt if not found
     * \remarks Don't modify the edge directly, use knot commands instead
     */
    Edge* edge_at(QPointF pos)  const ;

    /**
     *  \see Knot_View::mouse_select()
     */
    bool select_nodes(QList<Node*> nodes, bool modifier, bool clear);

    /**
     * \brief Initialize movement of the selected nodes
     */
    void initialize_movement(QPointF pivot);


};

/**
 * \brief Tool to select and move
 */
class Select_Tool : public Knot_Tool
{
public:
    explicit Select_Tool(Knot_View* view, Graph* graph = nullptr)
        : Knot_Tool(view,graph){}

    bool press(const Mouse_Event &event) override;

    void double_click(const Mouse_Event &event) override;

    void move(const Mouse_Event &event, QPointF &notify_pos) override;


};

/**
 * \brief Tool to create nodes and edges in succession
 */
class Edge_Chain_Tool : public Knot_Tool
{

    Node*               last_node;   ///< Last node in a chain
    QGraphicsLineItem   guide;       ///< Tiny line showing the edge being edited

public:
    explicit Edge_Chain_Tool (Knot_View* view, Graph* graph = nullptr);

    bool press(const Mouse_Event &event) override;

    void move(const Mouse_Event &event, QPointF &notify_pos) override;

    QCursor cursor() const override;

    void set_last_node(Node *n);

protected:
    void deactivate() override;

private:

    void break_chain();
};

/**
 * \brief Tool to toggle edges
 */
class Toggle_Edge_Tool : public Knot_Tool
{
    Node*               start_node;  ///< Dragged node
    QGraphicsLineItem   guide;       ///< Tiny line showing the edge being edited
public:
    explicit Toggle_Edge_Tool (Knot_View* view, Graph* graph = nullptr);

    bool press(const Mouse_Event &event) override;

    void release(const Mouse_Event &event) override;

    void deactivate() override;

    void move(const Mouse_Event &event, QPointF &notify_pos) override;
};

#endif // KNOT_TOOL_HPP
