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

#ifndef KNOT_VIEW_HPP
#define KNOT_VIEW_HPP

#include <QGraphicsView>
#include "graph.hpp"
#include <QUndoStack>
#include "snapping_grid.hpp"
#include "background_image.hpp"
#include "node_mover.hpp"
#include <QStack>
#include "pen_join_style_metatype.hpp"
#include "knot_tool.hpp"

class Context_Menu_Node;
class Context_Menu_Edge;

class Knot_View : public QGraphicsView
{
    Q_OBJECT

    friend class Knot_Command;
    friend class Knot_Tool;

    enum Mouse_Mode_Enum
    {
        NO_MODE     = 0x000, ///< No special action
        /*EDIT_GRAPH  = 0x001, ///< Move and select
        EDGE_CHAIN  = 0x002, ///< Keep inserting connected node, RMB selects
        TOGGLE_EDGES= 0x004, ///< Single click adds or removes edges*/

        MOVE_GRID   = 0x010, ///< Move grid origin
        MOVE_BG_IMG = 0x020, ///<  Move background image
        MOVE_BACK   = MOVE_GRID|MOVE_BG_IMG, ///< Mask with both MOVE_GRID and MOVE_BG_IMG

        DRAG_HANDLE = 0x100, ///< Dragging a transform handle
        RUBBERBAND  = 0x200, ///< Dragging the rubberband
        MOVE_NODES  = 0x400, ///< Moving the selection
        /**
         *  Used with MOVE_NODES when the movement is triggered
         *  by an external factor (such as pasting)
         *  \sa insert
         */
        EXTERNAL    = 0x800
    };
    Q_DECLARE_FLAGS(Mouse_Mode,Mouse_Mode_Enum)

    QPoint              move_center; ///< Point aligned to the cursor during movement
    Graph               m_graph;
    QUndoStack          undo_stack;
    QStack<class Knot_Macro*> macro_stack;
    Snapping_Grid       m_grid;
    Background_Image    bg_img;
    Mouse_Mode          mouse_mode;
    //Node*               last_node;   ///< Last node in a chain
    //QGraphicsLineItem   guide;       ///< Tiny line showing the edge being edited
    QGraphicsRectItem   rubberband;  ///< Draggable selection rectangle
    Node_Mover          node_mover;  ///< Helper to manage movement of the nodes
    QString             m_file_name; ///< Full name of the open file
    bool                paint_graph; ///< Whether to paint the graph
    bool                m_fluid_refresh; ///< Whether to update the graph while moving nodes
    Context_Menu_Node*  context_menu_node;
    Context_Menu_Edge*  context_menu_edge;


    Knot_Tool*       active_tool;
    Select_Tool      tool_select;
    Edge_Chain_Tool  tool_edge_chain;
    Toggle_Edge_Tool tool_toggle_edge;

public:

    /**
     *  \param file File name, if empty no file is loaded
    */
    Knot_View ( QString file = QString() );

    QString file_name() const { return m_file_name; }
    void set_file_name(QString name) {m_file_name = name;}

    QUndoStack* undo_stack_pointer() { return &undo_stack; }

    Snapping_Grid& grid() { return m_grid; }

    Background_Image& background_image() { return bg_img; }
    const Background_Image& background_image() const { return bg_img; }

    const Graph& graph() const { return m_graph; }

    void copy_graph_style(const Graph& g);

    /// Overload QGraphicsView::translate
    void translate(QPointF d) { QGraphicsView::translate(d.x(),d.y()); }

    /**
     *  \brief Translate and resize sceneRect
     *
     *  Translate the scene, if the result is not contained within sceneRect,
     *  the sceneRect is expanded
     */
    void translate_view(QPointF delta);

    /**
     *  \brief Creates a node in the given location
     *
     *  \return The created node
    */
    Node* add_node(QPointF pos);

    /**
     *  \brief Creates a node in the given location
     *
     *  If at location there is an edge, the node will split that edge
     *
     *  \return The created node
    */
    Node* add_breaking_node(QPointF pos);

    /**
     *  \brief Creates an edge connecting the given nodes
     *
     *  \pre n1 and n2 must be in the scene and in the graph
     *
     *  \return The newly created edge
     */
    Edge* add_edge(Node*n1,Node*n2);

    /**
     *  \brief Removes an edge from the view and graph
    */
    void remove_edge(Edge* edge);

    /**
     *  \brief Removes a node and connected edges
    */
    void remove_node(Node* node);

    /**
     *  \brief Begins a command macro
     */
    void begin_macro(QString name);
    /**
     *  \brief Ends a command macro
     */
    void end_macro();
    /**
     *  \brief Creates a command and adds it either to the current macro or to the command stack
     */
    void push_command( class Knot_Command* cmd );



    /**
     *  \brief Get the grobal zoom factor
     *
     *  \return A value representing the scaling factor, 1 = 100%
    */
    double get_zoom_factor() const { return transform().m11(); }

    /**
     *  \brief List of currenty selected nodes on the view
    */
    QList<Node*> selected_nodes() const;
    /**
     *  \brief List of currenty selected edges on the view
    */
    QList<Edge *> selected_edges() const;


    /**
     *  \brief Colors used to display the knot
     */
    const QList<QColor>& knot_colors() const { return m_graph.colors(); }


    const Border_List& knot_borders() const { return m_graph.borders(); }



    /**
     *  \brief Get knot stroke width
     */
    double stroke_width() const { return m_graph.width(); }

    Qt::PenJoinStyle join_style() const { return m_graph.join_style(); }

    /**
     * \brief Get whether mode is edit graph
     */
    bool edit_graph_mode_enabled() const;
    bool edge_loop_mode_enabled() const;
    bool toggle_edges_mode_enabled() const;

    bool load_file(QIODevice &device, QString action_name);

    void set_display_graph(bool enable);

    /**
     *  \brief Place new nodes on the view
     *
     *  Takes ownership of every object inside graph and adds them to the viewed graph.
     *  The inserted nodes will follow the mouse until the next click.
     *  The first node of the graph is taken as pivot point
     *
     *  \param graph        Graph that needs to be inserted.
     *  \param macro_name   Name of the undo command associated with the
     *                      insertion of the graph
     *
     *  \return Whether the insertion has been accepted
     */
    bool insert(const Graph& m_graph, QString macro_name );

    void enable_cache(bool enable) { m_graph.enable_cache(enable); }
    bool cache_enabled() const { return m_graph.cache_enabled(); }

    void set_fluid_refresh(bool enable);
    bool fluid_refresh() const { return m_fluid_refresh; }


    void set_antialiasing(bool enable);
    bool has_antialiasing() const;

    void set_transform_mode(Transform_Handle::Mode mode) { node_mover.set_mode(mode); }
    Transform_Handle::Mode transform_mode() const { return node_mover.mode(); }

public slots:
    /**
     * \brief Zoom view by factor
     *
     *  The zooming is performed relative to the current transformation
     *
     *  \param factor scaling factor ( 1 = don't zoom )
     */
    void zoom_view(double factor);
    /**
     * \brief Set zoom factor
     *
     *  The zooming is performed absolutely
     *
     *  \param factor scaling factor ( 1 = no zoom )
     */
    void set_zoom(double factor);

    /**
     *  \brief Sets mouse mode to edit graph
     */
    void set_mode_edit_graph();

    /**
     *  \brief Sets mouse mode to edge chain
     */
    void set_mode_edge_chain();


    /**
     *  \brief Sets mouse mode to toggle edges
     */
    void set_mode_toggle_edges();

    /**
     *  \brief Sets mouse mode to move grid
     *
     *  This will be reset on the next mouse click
     */
    void set_mode_move_grid();

    /**
     *  \brief Sets mouse mode to move the background image
     *
     *  This will be reset on the next mouse click
     */
    void set_mode_move_background();

    void set_background_color(QColor c) { setBackgroundBrush(c); }

    /**
     * \brief Render the knot again and repaint
     */
    void update_knot();

    /**
     *  \brief Set the colors used to display the knot
     */
    void set_knot_colors(const QList<QColor>& l);

    void set_knot_borders(const Border_List& b);

    /**
     *  \brief Enable/Disable custom colors
     */
    void set_knot_custom_colors(bool b);

    /**
     *  \brief Enable/Disable borders
     */
    void set_knot_display_border(bool b);


    /**
     *  \brief Set knot stroke width
     */
    void set_stroke_width(double w);

    void set_join_style(Qt::PenJoinStyle s);
    void set_brush_style(Qt::BrushStyle s);

    /**
     * \brief Load file and update file name
     * \post If no problem was encountered, the graph contains the contents of the file
     *       and the file name is updated to match the parameter
     * \return Whether the file was loaded successfully
     */
    bool load_file(QString fname);

    /**
     * \brief Save file and update file name
     * \post If no proble was encountered the clean state and file name are updated
     * \return Whether the file was saved successfully
     */
    bool save_file(QString fname);

    // knot style

    void set_knot_handle_lenght(double v);
    void set_knot_crossing_distance(double v);
    void set_knot_cusp_angle(double v);
    void set_knot_cusp_distance(double v);
    void set_knot_cusp_shape(Cusp_Shape* v);

    void set_selection_handle_lenght(double v);
    void set_selection_crossing_distance(double v);
    void set_selection_cusp_angle(double v);
    void set_selection_cusp_distance(double v);
    void set_selection_cusp_shape(Cusp_Shape* v);
    void set_selection_edge_type(Edge_Style* v);
    void set_selection_enabled_styles(Node_Style::Enabled_Styles v);

    // misc

    void flip_horiz_selection();
    void flip_vert_selection();

    /**
     *  \brief updates the transform handles
     *  \param select_edges whether the edges bewteen the selected nodes should be selected
     */
    void update_selection(bool select_edges=true);

signals:

    /// emitted when the graph structure or style is changed
    void graph_changed();

    /**
     *  \brief Emit when zoom is changed
     *  \param percent Zoom percentage
     */
    void zoomed(double percent);

    void selection_changed(QList<Node*>,QList<Edge*>);

    void mose_position_changed(QPointF);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

    /**
     *  \brief Expand sceneRect to contain the visible area
     *  \param margin optional margin to grow the visible area
    */
    void expand_scene_rect(int margin=0);

    /**
     *  \brief select nodes
     *
     *  \param nodes        List of nodes to be added/removed from the selection
     *  \param modifier
     *      If \c modifier is false, \c nodes will be the only selected nodes.
     *      If \c modifier is true and some of \c nodes are not selected,
     *      \c nodes and the previous selection will be both selected.
     *      If \c modifier is true and all \c nodes are selected, they will be unselected.
     *  \param select_edges whether the edges bewteen the selected nodes should be selected
     *
     *  \return Whether nodes have been selected or deselected
    */
    void rubberband_select(QList<Node*> nodes, bool modifier);
    
private slots:
    /// Expand scene rect when scrollbars reach margin
    void update_scrollbars() { expand_scene_rect(2); }

private:
    /**
     *  \brief Get node at location
     *  \return The found node or NULL
     *  \sa edge_at item_at
    */
    Node* node_at(QPointF p) const;

    /// Returns all nodes within the rubberband rectangle
    QList<Node*> nodes_in_rubberband() const;


    /**
     *  \brief Get edge at location
     *  \return The found edge or NULL
     *  \sa node_at item_at
    */
    Edge *edge_at(QPointF p) const;

    /**
     *  \brief Get item at location
     *  \return The found edge or NULL
     *  \sa node_at edge_at
    */
    Graph_Item* item_at(QPointF p) const;

    /**
     * @brief Set mouse mode and update cursor
     * \note Always use this instead of changing the value of mouse_mode
     * @param mode The new mouse mode
     */
    void set_mouse_mode(Mouse_Mode mode);

    /**
     * @brief Change mouse cursor depending on the mouse mode
     */
    void update_mouse_cursor();

    void set_active_tool(Knot_Tool& tool);

};

#endif // KNOT_VIEW_HPP
