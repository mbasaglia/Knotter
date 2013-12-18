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

#include "knot_view.hpp"
#include <QMouseEvent>
#include <QScrollBar>
#include "commands.hpp"
#include <QApplication>
#include "resource_manager.hpp"
#include "xml_loader.hpp"
#include "xml_exporter.hpp"
#include "context_menu_node.hpp"
#include "context_menu_edge.hpp"
#include <QFile>
//#include <QGLWidget>

Knot_View::Knot_View(QString file)
    : mouse_mode(NO_MODE), m_file_name(file),
      paint_graph(true), m_fluid_refresh(true),
      context_menu_node(new Context_Menu_Node(this)),
      context_menu_edge(new Context_Menu_Edge(this)),
      active_tool(nullptr), tool_select(this,&m_graph),
      tool_edge_chain(this,&m_graph),tool_toggle_edge(this,&m_graph)
{
    //setViewport(new QGLWidget);

    connect(Resource_Manager::pointer,SIGNAL(plugins_changed()),
            SLOT(check_plugins()));

    tool_edge_chain.set_graph(&m_graph);
    tool_select.set_graph(&m_graph);
    tool_toggle_edge.set_graph(&m_graph);
    active_tool = &tool_select;
    tool_select.enable(true);
    update_mouse_cursor();

    setWindowFilePath(file);
    setFrameStyle(StyledPanel|Plain);
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setSceneRect ( -width(), -height(), width()*2, height()*2);
    setScene(scene);
    setMouseTracking(true);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(NoAnchor);
    setResizeAnchor(AnchorViewCenter);

    scene->addItem(&m_graph);

    connect(horizontalScrollBar(),SIGNAL(valueChanged(int)),
            this,SLOT(update_scrollbars()));
    connect(verticalScrollBar(),SIGNAL(valueChanged(int)),
            this,SLOT(update_scrollbars()));


    QColor bg = QApplication::palette().color(QPalette::Highlight);
    rubberband.setPen(QPen(bg));
    bg.setAlpha(128);
    rubberband.setBrush(bg);
    rubberband.setZValue(100);


    //setCacheMode(CacheNone);
    connect(&m_grid,SIGNAL(grid_changed()),scene,SLOT(invalidate()));
    connect(&bg_img,SIGNAL(changed()),scene,SLOT(invalidate()));

    node_mover.add_handles_to_scene(scene);

    load_file(file);


}

void Knot_View::copy_graph_style(const Graph &g)
{
    begin_macro("Copy Style");

    set_knot_borders(g.borders());
    set_knot_display_border(g.paint_border());

    set_knot_colors(g.colors());
    set_knot_custom_colors(g.custom_colors());

    set_stroke_width(g.width());
    set_join_style(g.join_style());
    set_brush_style(g.brush_style());

    set_knot_crossing_distance(g.default_edge_style().crossing_distance);
    set_knot_cusp_angle(g.default_node_style().cusp_angle);
    set_knot_cusp_distance(g.default_node_style().cusp_distance);
    set_knot_cusp_shape(g.default_node_style().cusp_shape);
    set_knot_handle_lenght(g.default_node_style().handle_length);

    end_macro();
}


bool Knot_View::load_file(QIODevice &device, QString action_name )
{
    Graph loaded;
    if (  !import_xml(device,loaded) )
        return false;


    begin_macro(action_name);


    foreach(Node* n,m_graph.nodes())
    {
        push_command(new Remove_Node(n,this));
    }
    foreach(Edge* e,m_graph.edges())
    {
        push_command(new Remove_Edge(e,this));
    }

    push_command(new Knot_Width(m_graph.width(),loaded.width(),this));
    push_command(new Change_Colors(m_graph.colors(),loaded.colors(),this));
    push_command(new Change_Borders(m_graph.borders(),loaded.borders(),this));
    push_command(new Custom_Colors(m_graph.custom_colors(),loaded.custom_colors(),this));
    push_command(new Pen_Join_Style(m_graph.join_style(),loaded.join_style(),this));
    push_command(new Brush_Style(m_graph.brush_style(),loaded.brush_style(),this));
    push_command(new Knot_Style_All(m_graph.default_node_style(),
                                    loaded.default_node_style(),
                                    m_graph.default_edge_style(),
                                    loaded.default_edge_style(),
                                    this));

    foreach(Node* n,loaded.nodes())
    {
        push_command(new Create_Node(n,this));
    }
    foreach(Edge* e,loaded.edges())
    {
        push_command(new Create_Edge(e,this));
    }

    end_macro();
    undo_stack.setClean();


    view_fit();

    return true;
}


bool Knot_View::load_file(QString fname)
{
    if ( !fname.isEmpty() )
    {
        QFile file(fname);
        if ( load_file(file,tr("Load File")) )
        {
            setWindowFilePath(fname);
            m_file_name = fname;
            return true;
        }
    }
    else
    {
        Resource_Manager::settings.get_knot_style(m_graph);
    }
    return false;
}

bool Knot_View::save_file(QString fname)
{
    QFile file(fname);
    if ( export_xml(m_graph,file) )
    {
        setWindowFilePath(fname);
        m_file_name = fname;

        undo_stack.setClean();
        return true;
    }
    return false;
}

void Knot_View::set_knot_handle_lenght(double v)
{
    push_command(new Knot_Style_Handle_Lenght(
                     m_graph.default_node_style().handle_length,
                     v, this
                     ));
}

void Knot_View::set_knot_crossing_distance(double v)
{
    push_command(new Knot_Style_Crossing_Distance(
                     m_graph.default_edge_style().crossing_distance,
                     v, this
                ));
}

void Knot_View::set_knot_cusp_angle(double v)
{
    push_command(new Knot_Style_Cusp_Angle(
                     m_graph.default_node_style().cusp_angle,
                     v, this
                ));
}

void Knot_View::set_knot_cusp_distance(double v)
{
    push_command(new Knot_Style_Cusp_Distance(
                     m_graph.default_node_style().cusp_distance,
                     v, this
                ));
}

void Knot_View::set_knot_cusp_shape(Cusp_Shape *v)
{
    push_command(new Knot_Style_Cusp_Shape(
                     m_graph.default_node_style().cusp_shape,
                     v, this
                     ));
}

void Knot_View::set_knot_ege_slide(double v)
{
    push_command(new Knot_Style_Edge_Slide(
                     m_graph.default_edge_style().edge_slide,
                     v, this
                     ));
}

void Knot_View::set_selection_handle_lenght_nodes(double v)
{
    QList<Node*> nodes = selected_nodes();
    QList<double> before;
    QList<double> after;
    foreach(Node* n, nodes)
    {
        before.push_back(n->style().handle_length);
        after.push_back(v);
    }
    push_command(new Node_Style_Handle_Lenght(nodes,before,after,this));
}

void Knot_View::set_selection_crossing_distance(double v)
{
    QList<Edge*> edges = selected_edges();
    QList<double> before;
    QList<double> after;
    foreach(Edge* e, edges)
    {
        before.push_back(e->style().crossing_distance);
        after.push_back(v);
    }
    push_command(new Edge_Style_Crossing_Distance(edges,before,after,this));
}

void Knot_View::set_selection_cusp_angle(double v)
{
    QList<Node*> nodes = selected_nodes();
    QList<double> before;
    QList<double> after;
    foreach(Node* n, nodes)
    {
        before.push_back(n->style().cusp_angle);
        after.push_back(v);
    }
    push_command(new Node_Style_Cusp_Angle(nodes,before,after,this));
}

void Knot_View::set_selection_cusp_distance(double v)
{
    QList<Node*> nodes = selected_nodes();
    QList<double> before;
    QList<double> after;
    foreach(Node* n, nodes)
    {
        before.push_back(n->style().cusp_distance);
        after.push_back(v);
    }
    push_command(new Node_Style_Cusp_Distance(nodes,before,after,this));
}

void Knot_View::set_selection_cusp_shape(Cusp_Shape *v)
{
    QList<Node*> nodes = selected_nodes();
    QList<Cusp_Shape *> before;
    QList<Cusp_Shape *> after;
    foreach(Node* n, nodes)
    {
        before.push_back(n->style().cusp_shape);
        after.push_back(v);
    }
    push_command(new Node_Style_Cusp_Shape(nodes,before,after,this));
}

void Knot_View::set_selection_edge_type(Edge_Type *v)
{
    begin_macro(tr("Change Edge Type"));

    foreach(Edge* e, selected_edges())
    {
        push_command(new Change_Edge_Type(e,e->style().edge_type,v,this));
    }

    end_macro();
}

void Knot_View::set_selection_enabled_styles_nodes(Node_Style::Enabled_Styles v)
{
    QList<Node*> nodes = selected_nodes();
    QList<Node_Style::Enabled_Styles> before;
    QList<Node_Style::Enabled_Styles> after;
    foreach(Node* n, nodes)
    {
        before.push_back(n->style().enabled_style);
        after.push_back(v);
    }
    push_command(new Node_Style_Enable(nodes,before,after,this));
}

void Knot_View::set_selection_handle_lenght_edges(double v)
{
    QList<Edge*> edges = selected_edges();
    QList<double> before;
    QList<double> after;
    foreach(Edge* e, edges)
    {
        before.push_back(e->style().handle_length);
        after.push_back(v);
    }
    push_command(new Edge_Style_Handle_Lenght(edges,before,after,this));
}

void Knot_View::set_selection_enabled_styles_edges(Edge_Style::Enabled_Styles v)
{
    QList<Edge*> edges = selected_edges();
    QList<Edge_Style::Enabled_Styles> before;
    QList<Edge_Style::Enabled_Styles> after;
    foreach(Edge* e, edges)
    {
        before.push_back(e->style().enabled_style);
        after.push_back(v);
    }
    push_command(new Edge_Style_Enable(edges,before,after,this));
}

void Knot_View::set_selection_edge_slide(double v)
{
    QList<Edge*> edges = selected_edges();
    QList<double> before;
    QList<double> after;
    foreach(Edge* e, edges)
    {
        before.push_back(e->style().edge_slide);
        after.push_back(v);
    }
    push_command(new Edge_Style_Edge_Slide(edges,before,after,this));
}

void Knot_View::flip_horiz_selection()
{
    QList<Node*> nodes = selected_nodes();
    double c = 0;
    foreach(Node* n, nodes)
    {
        c += n->x() / nodes.size();
    }
    begin_macro(tr("Horizontal Flip"));
    foreach(Node* n, nodes)
        push_command(new Move_Node(n,n->pos(),QPointF(c-(n->x()-c),n->y()),this));

    end_macro();
}

void Knot_View::flip_vert_selection()
{
    QList<Node*> nodes = selected_nodes();
    double c = 0;
    foreach(Node* n, nodes)
    {
        c += n->y() / nodes.size();
    }
    begin_macro(tr("Vertical Flip"));
    foreach(Node* n, nodes)
        push_command(new Move_Node(n,n->pos(),QPointF(n->x(),c-(n->y()-c)),this));

    end_macro();
}

void Knot_View::update_selection(bool select_edges)
{
    node_mover.set_nodes(selected_nodes());

    if ( select_edges )
        foreach(Edge* e, m_graph.edges())
        {
            e->setSelected( e->vertex1()->isSelected() && e->vertex2()->isSelected() );
        }

    emit selection_changed(node_mover.nodes(),selected_edges());
}

void Knot_View::set_display_graph(bool enable)
{
    paint_graph = enable;

    foreach ( Node* n, m_graph.nodes() )
        n->set_visible( enable );

    foreach ( Edge* e, m_graph.edges() )
        e->set_visible( enable);

    scene()->invalidate();
}


void Knot_View::translate_view(QPointF delta)
{
    translate(delta);
    expand_scene_rect(10);
}

void Knot_View::translate_view_to(QPointF destination)
{
    translate_view(mapToScene(0,0)-destination);
}

Node* Knot_View::add_node(QPointF pos)
{
    Node* node = new Node(pos);
    push_command(new Create_Node(node,this));
    return node;
}

Node *Knot_View::add_breaking_node(QPointF pos)
{
    Edge* e = edge_at(pos);
    if ( !e )
        return add_node(pos);

    Node* v1 = e->vertex1();
    Node* v2 = e->vertex2();


    begin_macro(tr("Break Edge"));
    Node* new_node = add_node(e->snap(pos));
    add_edge(v1,new_node);
    add_edge(new_node,v2);
    remove_edge(e);
    end_macro();

    return new_node;
}

Edge *Knot_View::add_edge(Node *n1, Node *n2)
{
    Edge* e = new Edge(n1,n2,Resource_Manager::default_edge_type());
    push_command(new Create_Edge(e,this));
    return e;
}

void Knot_View::remove_edge(Edge *edge)
{
    push_command(new Remove_Edge(edge,this));
}

void Knot_View::remove_node(Node *node)
{
    begin_macro(tr("Remove Node"));
    foreach(Edge* e, node->edges())
    {
        if ( e->scene() == scene() )
            remove_edge(e);
    }
    push_command(new Remove_Node(node,this));
    end_macro();
}

void Knot_View::begin_macro(QString name)
{
    macro_stack.push( new Knot_Macro(name,this,nullptr) );
}

void Knot_View::end_macro()
{
    if ( macro_stack.empty() )
        return;
    Knot_Macro* macro = macro_stack.pop();
    push_command(macro);
}

void Knot_View::push_command(Knot_Command *cmd)
{
    if ( !macro_stack.isEmpty() )
        cmd->set_parent(macro_stack.top());
    else
        undo_stack.push(cmd);
}

QList<Node *> Knot_View::selected_nodes() const
{
    QList<Node*> nl;
    foreach ( QGraphicsItem* it, scene()->selectedItems() )
    {
        Node* n = dynamic_cast<Node*>(it);
        if(n)
            nl.push_back(n);
    }
    return nl;
}

QList<Edge *> Knot_View::selected_edges() const
{
    QList<Edge*> nl;
    foreach ( QGraphicsItem* it, scene()->selectedItems() )
    {
        Edge* n = dynamic_cast<Edge*>(it);
        if(n)
            nl.push_back(n);
    }
    return nl;
}



void Knot_View::zoom_view(double factor)
{
    if ( get_zoom_factor()*factor < 0.01 )
        return;

    QPoint mp = mapFromGlobal(QCursor::pos());
    QPointF sp1 = mapToScene(mp);

    QRectF r ( mapToScene(0,0), mapToScene(width()/factor,height()/factor));
    r.translate(-r.bottomRight()/2);
    setSceneRect(sceneRect().united(r));

    scale(factor,factor);
    // Anchor on mouse
    if ( rect().contains(mp) )
    {
        QPointF sp2 = mapToScene(mp);
        translate(sp2-sp1);

    }
    expand_scene_rect();

    emit zoomed(100*get_zoom_factor());
}

void Knot_View::set_zoom(double factor)
{
    if ( factor < 0.01 )
        return;
    QTransform t ( factor,           transform().m12(), transform().m13(),
                   transform().m21(),factor,            transform().m23(),
                   transform().m31(),transform().m32(), transform().m33()
                   );
    setTransform(t);
    emit zoomed(100*factor);
}


void Knot_View::set_active_tool(Knot_Tool &tool)
{

    if ( active_tool != &tool )
    {
        active_tool->enable(false);
        active_tool = &tool;
        active_tool->enable(true);
        update_mouse_cursor();
    }
}
void Knot_View::set_mode_edit_graph()
{
    set_active_tool(tool_select);
}

void Knot_View::set_mode_edge_chain()
{
    set_active_tool(tool_edge_chain);
}

void Knot_View::set_mode_toggle_edges()
{
    set_active_tool(tool_toggle_edge);
}

void Knot_View::set_mode_move_grid()
{
    set_mouse_mode(mouse_mode | MOVE_GRID);
}

void Knot_View::set_mode_move_background()
{
    set_mouse_mode ( mouse_mode | MOVE_BG_IMG );
}

void Knot_View::update_knot()
{
    m_graph.render_knot();
    scene()->invalidate();
}

void Knot_View::set_knot_colors(const QList<QColor> &l)
{
    push_command(new Change_Colors(m_graph.colors(),l,this));
}

void Knot_View::set_knot_borders(const Border_List &b)
{
    push_command(new Change_Borders(m_graph.borders(),b,this));
}

void Knot_View::set_knot_custom_colors(bool b)
{
    push_command(new Custom_Colors(m_graph.custom_colors(),b,this));
}

void Knot_View::set_knot_display_border(bool b)
{
    push_command(new Display_Border(m_graph.paint_border(),b,this));
}

void Knot_View::set_stroke_width(double w)
{
    push_command(new Knot_Width(m_graph.width(),w,this));
}

void Knot_View::set_join_style(Qt::PenJoinStyle s)
{

    push_command(new Pen_Join_Style(m_graph.join_style(),s,this));
}

void Knot_View::set_brush_style(Qt::BrushStyle s)
{
    push_command(new Brush_Style(m_graph.brush_style(),s,this));
}

void Knot_View::expand_scene_rect(int margin)
{
    QRectF vp ( mapToScene(-margin,-margin),
                mapToScene(width()+2*margin,height()+2*margin));
    QRectF sr = sceneRect();
    if ( ! sr.contains(vp) )
    {
        setSceneRect(sr.united(vp));
        /*QPoint p1 ( qMin(vp.left(),sr.left()),qMin(vp.top(),sr.top()) );
        QPoint p2 ( qMax(vp.right(),sr.right()),qMax(vp.bottom(),sr.bottom()) );
        setSceneRect(QRect(p1,p2));*/
    }

    emit scene_rect_changed(QRectF(mapToScene(0,0),mapToScene(width(),height())));
}

void Knot_View::rubberband_select(QList<Node *> nodes, bool modifier)
{
    bool select = true;

    // if there is no modifier the selection needs to be replaced
    if ( !modifier )
    {
        scene()->clearSelection();
    }
    // modifier, nodes may be selected or deselected
    else
    {
        // if all nodes are already selected, deselect them
        select = false;
        foreach(Node* itm, nodes)
        {
            // found one that is not selected, therefore need to select (ie not deselect)
            if ( !itm->isSelected() )
            {
                select = true;
                break;
            }
        }
    }

    // select or deselect
    foreach(Node* itm, nodes)
    {
        itm->setSelected(select);
        foreach(Edge* e, itm->edges())
        {
            e->setSelected( select && e->other(itm)->isSelected() );
        }
    }

    update_selection(false);
}

void Knot_View::check_plugins()
{
    if ( !Resource_Manager::cusp_shapes().contains(m_graph.default_node_style().cusp_shape) )
    {
        m_graph.default_node_style_reference().cusp_shape =
                Resource_Manager::default_cusp_shape();
    }
    foreach(Node* n, m_graph.nodes())
    {
        if ( n->style().enabled_style & Node_Style::CUSP_SHAPE &&
             !Resource_Manager::cusp_shapes().contains(n->style().cusp_shape) )
        {
            n->style().cusp_shape = nullptr;
            n->style().enabled_style ^= Node_Style::CUSP_SHAPE;
        }
    }

    foreach(Edge* e, m_graph.edges())
    {
        if ( !Resource_Manager::edge_types().contains(e->style().edge_type) )
        {
            e->style().edge_type = Resource_Manager::default_edge_type();
        }
    }
}

Node *Knot_View::node_at(QPointF p) const
{
    return dynamic_cast<Node*>(scene()->itemAt(p,QTransform()));
}


Edge *Knot_View::edge_at(QPointF p) const
{
    return dynamic_cast<Edge*>(scene()->itemAt(p,QTransform()));
}

Graph_Item *Knot_View::item_at(QPointF p) const
{
    return dynamic_cast<Graph_Item*>(scene()->itemAt(p,QTransform()));
}




bool Knot_View::insert(const Graph &graph, QString macro_name)
{
    if ( graph.nodes().isEmpty() || mouse_mode & MOVE_NODES )
        return false;

    scene()->clearSelection();
    macro_stack.push( new Knot_Insert_Macro(false,macro_name,this));

    foreach(Node* n, graph.nodes())
    {
        push_command(new Create_Node(n,this));
        n->setSelected(true);
    }

    foreach(Edge* e, graph.edges())
    {
        push_command(new Create_Edge(e,this));
    }



    set_mouse_mode(mouse_mode|MOVE_NODES|EXTERNAL);

    node_mover.set_nodes(graph.nodes());
    node_mover.initialize_movement( graph.nodes().front()->pos());
    emit selection_changed(node_mover.nodes(),selected_edges());

    QPointF p =  mapToScene(mapFromGlobal(QCursor::pos()));
    node_mover.set_pos(p);
    move_center = mapFromGlobal(QCursor::pos());
    //node_mover.deploy(this);

    end_macro();
    macro_stack.push( new Knot_Insert_Macro(true,macro_name,this));

    return true;

}

void Knot_View::set_fluid_refresh(bool enable)
{
    m_fluid_refresh = enable;
    if ( enable )
        update_knot();
}

void Knot_View::set_antialiasing(bool enable)
{
    setRenderHint(QPainter::Antialiasing,enable);
}

bool Knot_View::has_antialiasing() const
{
    return renderHints() & QPainter::Antialiasing;
}

void Knot_View::view_fit()
{
    if ( m_graph.nodes().empty() )
        reset_view();
    else
    {
        set_zoom(1);
        QRectF area = m_graph.boundingRect();
        QRectF self_area ( mapToScene(0,0), mapToScene(width(),height()) );

        double zoom_w = self_area.width() / area.width();
        double zoom_h = self_area.height() / area.height();
        double zoom_wh = qMin(zoom_w,zoom_h);

        area.setLeft(area.center().x()-self_area.width()/zoom_wh/2);
        area.setWidth(self_area.width()/zoom_wh);

        area.setTop(area.center().y()-self_area.height()/zoom_wh/2);
        area.setHeight(self_area.height()/zoom_wh);

        setSceneRect(area.united(sceneRect()));
        set_zoom(zoom_wh);
        centerOn(area.center());
        expand_scene_rect(10);
    }
}

void Knot_View::reset_view()
{
    resetTransform();
    setSceneRect(-width()/2,-height()/2,width(),height());
    centerOn(0,0);
    expand_scene_rect(10);
}


void Knot_View::mousePressEvent(QMouseEvent *event)
{
    QPoint mpos = event->pos();
    QPointF scene_pos = mapToScene(mpos);
    QPointF snapped_scene_pos = m_grid.nearest(scene_pos);

    if ( event->button() == Qt::MiddleButton )
    {
        // drag view
        setCursor(Qt::ClosedHandCursor);
    }
    else if ( mouse_mode & MOVE_BACK)
    {
        set_mouse_mode ( mouse_mode & ~MOVE_BACK );
    }
    else if ( !(mouse_mode & EXTERNAL) )
    {
        if ( !active_tool->press(Mouse_Event(scene_pos,snapped_scene_pos,event)) )
        {

            Transform_Handle *th = dynamic_cast<Transform_Handle*>(
                                    scene()->itemAt(scene_pos,QTransform()));
            if ( th )
            {
                // drag handle
                node_mover.set_nodes(selected_nodes());

                double anchor_angle = event->modifiers() & Qt::ShiftModifier;
                if ( transform_mode() == Transform_Handle::SCALE )
                    anchor_angle  = !anchor_angle;

                node_mover.set_dragged_handle(th,anchor_angle);
                set_mouse_mode ( mouse_mode | DRAG_HANDLE );
            }
            else
            {
                // drag rubberband selection
                set_mouse_mode ( mouse_mode | RUBBERBAND );
                rubberband.setPos(mapToScene(mpos));
                rubberband.setRect(0,0,0,0);
                scene()->addItem(&rubberband);
            }
        }


    }
    move_center = mpos;
}

void Knot_View::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mpos = event->pos();
    QPointF scene_pos = mapToScene(mpos);
    QPointF snapped_scene_pos = m_grid.nearest(scene_pos);
    QPointF emitted_pos = scene_pos;

    if ( event->buttons() & Qt::MiddleButton  )
    {
        // drag view
        QPointF delta = mpos-move_center;
        delta /= get_zoom_factor(); // take scaling into account
        translate_view(delta);
    }
    else if ( mouse_mode & MOVE_BACK )
    {
        Node* n = node_at(scene_pos);
        Edge* e = edge_at(scene_pos);
        if ( mouse_mode & MOVE_GRID )
        {
            if ( n )
                m_grid.set_origin(n->pos());
            else if ( e )
                m_grid.set_origin(e->snap(scene_pos));
            else
                m_grid.set_origin(scene_pos);
        }
        else if ( mouse_mode & MOVE_BG_IMG )
        {
            if ( n )
                bg_img.set_position(n->pos());
            else if ( e )
                bg_img.set_position(e->snap(scene_pos));
            else
                bg_img.set_position(scene_pos);
        }

    }
    else if ( mouse_mode & RUBBERBAND )
    {
        // update rubberband
        rubberband.setRect(QRectF(QPointF(0,0),mapToScene(mpos)-rubberband.pos())
                            .normalized());
    }
    else if ( mouse_mode & MOVE_NODES )
    {
        // move selected nodes

        node_mover.set_pos(snapped_scene_pos);
        if ( m_fluid_refresh )
            update_knot();

        emitted_pos = snapped_scene_pos;

    }
    else if ( mouse_mode & DRAG_HANDLE )
    {

        node_mover.drag_handle(scene_pos,
                               event->modifiers() & Qt::ControlModifier,
                               m_grid.size());
        if ( m_fluid_refresh )
            update_knot();
    }
    else
        active_tool->move(Mouse_Event(scene_pos,snapped_scene_pos,event),emitted_pos);


    // highlight item under cursor
    foreach ( QGraphicsItem* gi, scene()->items() )
    {
        Graph_Item* ci = dynamic_cast<Graph_Item*>(gi);
        if ( ci )
            ci->set_highlighted(false);
    }
    Graph_Item* ci = item_at(scene_pos);
    if ( ci )
        ci->set_highlighted(true);


    move_center = mpos;
    scene()->invalidate();
    emit mose_position_changed(emitted_pos);
}


void Knot_View::mouseReleaseEvent(QMouseEvent *event)
{

    QPoint mpos = event->pos();
    QPointF scene_pos = mapToScene(mpos);
    QPointF snapped_scene_pos = m_grid.nearest(scene_pos);

    if ( mouse_mode & RUBBERBAND )
    {

        // select from rubberband;
        scene()->removeItem(&rubberband);

        rubberband_select(nodes_in_rubberband(),
                     event->modifiers() & (Qt::ControlModifier|Qt::ShiftModifier) );

        mouse_mode &= ~RUBBERBAND;
    }
    else if ( mouse_mode & MOVE_NODES && event->button() != Qt::MiddleButton )
    {
        mouse_mode &=~ MOVE_NODES;
        node_mover.deploy(this,tr("Move Nodes"));
        update_knot();


        if ( mouse_mode & EXTERNAL )
        {
            mouse_mode &=~ EXTERNAL;
            end_macro();
        }
    }
    else if ( mouse_mode & DRAG_HANDLE )
    {
        node_mover.deploy(this,
                          node_mover.mode() == Transform_Handle::ROTATE ?
                              tr("Rotate Nodes") : tr("Scale Nodes") );
        set_mouse_mode ( mouse_mode & ~DRAG_HANDLE );
    }
    else
    {
        active_tool->release(Mouse_Event(scene_pos,snapped_scene_pos,event));
    }

    if ( event->button() == Qt::RightButton )
    {
        Node* n = node_at(scene_pos);
        Edge* e = edge_at(scene_pos);
        if ( n )
            context_menu_node->popup(n,mapToGlobal(mpos));
        else if ( e )
            context_menu_edge->popup(e,mapToGlobal(mpos));
    }

    update_mouse_cursor();
}

void Knot_View::mouseDoubleClickEvent(QMouseEvent *event)
{

    QPoint mpos = event->pos();
    QPointF scene_pos = mapToScene(mpos);
    QPointF snapped_scene_pos = m_grid.nearest(scene_pos);

    active_tool->double_click(Mouse_Event(scene_pos,snapped_scene_pos,event));
}

void Knot_View::wheelEvent(QWheelEvent *event)
{
    if ( event->modifiers() & Qt::ControlModifier )
    {
        if ( event->delta() < 0 )
            zoom_view(0.8);
        else
            zoom_view(1.25);
    }
    else if ( mouse_mode & MOVE_NODES )
    {
        if ( event->modifiers() & Qt::ShiftModifier )
            node_mover.rotate(15*(event->delta()>0?1:-1));
        else
            node_mover.fixed_scale( event->delta() < 0, m_grid.size() );
        update_knot();
    }
    else
    {
        QPointF p = mapToScene(event->pos());
        Edge* e = edge_at(p);
        if ( e )
        {
            Edge_Type* st = event->delta() < 0 ?
                        Resource_Manager::next_edge_type(e->style().edge_type) :
                        Resource_Manager::prev_edge_type(e->style().edge_type) ;
            push_command(new Change_Edge_Type(e,e->style().edge_type,st,this));
        }
        else // scroll
        {
            QScrollBar* sb = verticalScrollBar();
            int d = event->delta() > 0 ? -1 : +1;
            if ( event->modifiers() & Qt::ShiftModifier )
                sb = horizontalScrollBar();
            sb->setValue(sb->value()+d*sb->singleStep());
        }
    }
}



QList<Node *> Knot_View::nodes_in_rubberband() const
{
    QList<QGraphicsItem*> items = scene()->items(QRectF(
                            rubberband.rect().translated(rubberband.pos())));
    QList<Node*> nodes;
    foreach ( QGraphicsItem* item, items)
    {
        Node* n = dynamic_cast<Node*>(item);
        if ( n )
            nodes.push_back(n);
    }
    return nodes;
}

void Knot_View::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect,backgroundBrush());
    bg_img.render(painter);
    m_grid.render(painter,rect);
}



void Knot_View::set_mouse_mode(Mouse_Mode mode)
{
    mouse_mode = mode;
    update_mouse_cursor();
}


void Knot_View::update_mouse_cursor()
{
    if ( mouse_mode & MOVE_BACK )
        setCursor(Qt::SizeAllCursor);
    else if ( mouse_mode & EXTERNAL )
        setCursor(Qt::DragCopyCursor);
    else if ( mouse_mode & DRAG_HANDLE )
        setCursor(node_mover.current_handle_cursor());
    else if ( mouse_mode & RUBBERBAND )
        setCursor(Qt::ArrowCursor);
    else
        setCursor(active_tool->cursor());
}



bool Knot_View::edit_graph_mode_enabled() const
{
    return active_tool == &tool_select;
}

bool Knot_View::edge_loop_mode_enabled() const
{
    return active_tool == &tool_edge_chain;
}

bool Knot_View::toggle_edges_mode_enabled() const
{
    return active_tool == &tool_toggle_edge;
}
