#include "include/xml_saver.hpp"

xml_saver::xml_saver(QIODevice *output)
    : xml ( output )
{
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(4);
}

void xml_saver::begin()
{
    xml.writeStartDocument("1.0");

    xml.writeStartElement("knot");
    xml.writeAttribute("version",QString::number(version));
}

void xml_saver::end()
{
    xml.writeEndDocument();
}

void xml_saver::start_element(QString name)
{
    xml.writeStartElement ( name );
}

void xml_saver::end_element()
{
    xml.writeEndElement();
}

void xml_saver::save_pen(QString name, QPen pen, bool style, bool join_style )
{
    start_element(name);

    xml.writeAttribute("width",QString::number(pen.width()));
    xml.writeAttribute("color",pen.color().name());
    xml.writeAttribute("alpha",QString::number(pen.color().alpha()));

    if ( style )
    {
        QString penstyle;
        switch ( pen.style() )
        {
            default:
            case Qt::NoPen:         penstyle = "nothing";       break;
            case Qt::SolidLine:     penstyle = "solid";         break;
            case Qt::DotLine:       penstyle = "dot";           break;
            case Qt::DashLine:      penstyle = "dash";          break;
            case Qt::DashDotLine:   penstyle = "dash_dot";      break;
            case Qt::DashDotDotLine:penstyle = "dash_dot_dot";  break;
        }
        xml.writeAttribute("style",penstyle);
    }

    if ( join_style )
    {
        QString point;
        switch ( pen.joinStyle() )
        {
            default:
            case Qt::BevelJoin: point = "bevel"; break;
            case Qt::MiterJoin: point = "miter"; break;
            case Qt::RoundJoin: point = "round"; break;
        }
        xml.writeAttribute("point",point);
    }

    end_element();
}

void xml_saver::save_cusp(QString name, styleinfo si)
{
    start_element(name);

    xml.writeTextElement("style",knot_curve_styler::name(si.curve_style));
    xml.writeTextElement("min-angle",QString::number(si.cusp_angle));
    xml.writeTextElement("distance",QString::number(si.cusp_distance));

    xml.writeTextElement("gap",QString::number(si.crossing_distance));
    xml.writeTextElement("handle-length",QString::number(si.handle_length));

    end_element();
}

void xml_saver::save_graph(QString name, node_list nodes)
{
    start_element(name);

    foreach(Node* n, nodes)
    {
        save_node(n);
    }

    end_element();
}

void xml_saver::save_node(Node *n)
{
    int id = register_node(n);


    xml.writeStartElement("node");
        xml.writeAttribute("id",QString("node_%1").arg(id));
        xml.writeAttribute("x",QString::number(n->pos().x()));
        xml.writeAttribute("y",QString::number(n->pos().y()));
        if ( n->has_custom_style() )
            save_cusp("custom-style",n->get_custom_style());

        foreach(Edge* e, n->links())
        {
            Node* o = e->other(n);
            int oid = register_node(o);
            xml.writeEmptyElement("edge");
            QString edge_type;
            switch ( e->type )
            {
                default:
                case Edge::CROSSING: edge_type = "regular"; break;
                case Edge::WALL:     edge_type = "wall"; break;
                case Edge::HOLE:     edge_type = "hole"; break;
                case Edge::INVERTED: edge_type = "inverted"; break;
            }
            xml.writeAttribute("type",edge_type);
            xml.writeAttribute("target",QString("node_%1").arg(oid));
        }
    xml.writeEndElement();

}

int xml_saver::register_node(Node *n )
{
    if ( node_ids.contains(n) )
        return node_ids[n];
    else
    {
        return node_ids[n] = node_ids.size();
    }
}


//============================================

bool xml_loader::load(QIODevice *input)
{

    QDomDocument xml;
    if ( !xml.setContent(input) ) // could retrieve error details
        return false; // xml error

    QDomElement knot = xml.firstChildElement("knot");
    if ( knot.isNull() )
        return false; // XML does not contain a knot
    version = knot.attribute("version").toInt();
    if ( version > xml_saver::version )
        return false; // unknown version

    if ( knot.firstChildElement("graph").isNull() )
        return false; // XML does not contain a graph description

    current_elements.push( knot );
    return true;
}

void xml_loader::get_graph(KnotView *view)
{
    QDomElement graph = current_elements.top().firstChildElement("graph");
    QDomElement node = graph.firstChildElement("node");
    while ( !node.isNull() )
    {
        QString id = node.attribute("id");
        Node* cur_node = register_node ( id );
        cur_node->setPos(node.attribute("x").toDouble(),node.attribute("y").toDouble());

        if ( !node.firstChildElement("custom-style").isNull() )
        {
            current_elements.push(node);
            cur_node->set_custom_style ( get_cusp ( "custom-style",
                                        view->get_default_style() ) );
            current_elements.pop();
        }

        view->add_node(cur_node);

        QDomElement edge = node.firstChildElement("edge");
        while ( !edge.isNull() )
        {
            QString type_name = edge.attribute("type");
            QString target = edge.attribute("target");
            Edge::type_type type = Edge::CROSSING;
            if ( type_name == "regular" )
                type = Edge::CROSSING;
            else if ( type_name == "wall" )
                type = Edge::WALL;
            else if ( type_name == "hole" )
                type = Edge::HOLE;
            else if ( type_name == "inverted" )
                type = Edge::INVERTED;
            Node* target_node = register_node(target);
            view->add_edge(cur_node,target_node);
            view->set_edge_type( cur_node->get_link(target_node), type );
            edge = edge.nextSiblingElement("edge");
        }
        node = node.nextSiblingElement("node");
    }
}

Node *xml_loader::register_node(QString id)
{
    if ( !node_ids.contains(id) )
    {
        return node_ids[id] = new Node(QPointF());
    }
    else
        return node_ids[id];
}

bool xml_loader::enter(QString name)
{
    QDomElement elem = current_elements.top().firstChildElement(name);
    if ( elem.isNull() )
        return false;
    current_elements.push ( elem );
    return true;
}

QPen xml_loader::get_pen(QString name, QPen pen)
{
    QDomElement pen_elem = current_elements.top().firstChildElement(name);

    QColor pen_col(pen_elem.attribute("color",pen.color().name()));
    pen_col.setAlpha(pen_elem.attribute("alpha",
                            QString::number(pen.color().alpha())
                        ).toInt());
    pen.setColor(pen_col);
    pen.setWidth(pen_elem.attribute("width",QString::number(pen.widthF())).toDouble() );


    QString pstyle_name = pen_elem.attribute("style","solid");
    if ( pstyle_name == "solid" )
        pen.setStyle ( Qt::SolidLine );
    else if ( pstyle_name == "dot" )
        pen.setStyle ( Qt::DotLine );
    else if ( pstyle_name == "dash" )
        pen.setStyle ( Qt::DashLine );
    else if ( pstyle_name == "dash_dot" )
        pen.setStyle ( Qt::DashDotLine );
    else if ( pstyle_name == "dash_dot_dot" )
        pen.setStyle ( Qt::DashDotDotLine );
    else
        pen.setStyle ( Qt::NoPen );

    QDomElement cusp_point = pen_elem.firstChildElement("point");
    if ( !cusp_point.isNull() )
    {
        QString point_name =  cusp_point.text();
        if ( point_name == "bevel" )
            pen.setJoinStyle(Qt::BevelJoin);
        else if ( point_name == "round" )
            pen.setJoinStyle ( Qt::RoundJoin );
        else
            pen.setJoinStyle ( Qt::MiterJoin );
    }

    return pen;
}

styleinfo xml_loader::get_cusp(QString name, styleinfo cusp_style_info)
{

    QDomElement cusp = current_elements.top().firstChildElement(name);

        QDomElement cusp_style = cusp.firstChildElement("style");
        if ( !cusp_style.isNull() )
            cusp_style_info.curve_style = knot_curve_styler::idof(cusp_style.text());

        QDomElement cusp_angle = cusp.firstChildElement("min-angle");
        if ( ! cusp_angle.isNull() )
            cusp_style_info.cusp_angle =  cusp_angle.text().toDouble();

        QDomElement cusp_dist = cusp.firstChildElement("distance");
        if ( !  cusp_dist.isNull() )
            cusp_style_info.cusp_distance = cusp_dist.text().toDouble();


        QDomElement egap = cusp.firstChildElement("gap");
        if ( !egap.isNull() )
            cusp_style_info.crossing_distance =  egap.text().toDouble();

        QDomElement handle_length = cusp.firstChildElement("handle-length");
        if ( ! handle_length.isNull() )
            cusp_style_info.handle_length =  handle_length.text().toDouble();

        return cusp_style_info;

}

void xml_loader::leave()
{
    current_elements.pop();
}


