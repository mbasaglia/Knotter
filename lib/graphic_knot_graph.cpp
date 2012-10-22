/**

\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012  Mattia Basaglia

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
#include "graphic_knot_graph.hpp"
#include <QBrush>
#include <QPen>
#include "path_builder.hpp"
#include "xml_saver.hpp"
#include <QStyleOptionGraphicsItem>
#include <QSvgGenerator>
#include <QPainter>
#include <QBuffer>

KnotGraph::KnotGraph()
{
    setBrush(QBrush(Qt::black));
    setPen(QPen(Qt::gray));
    stroker.setWidth(5);
    stroker.setCapStyle(Qt::FlatCap);

    /// \todo configure miter limit
    stroker.setJoinStyle(Qt::MiterJoin);

    //setCacheMode(QGraphicsItem::DeviceCoordinateCache); mild
    //setCacheMode(QGraphicsItem::ItemCoordinateCache); heavy

}

KnotGraph::~KnotGraph()
{
}

KnotGraph &KnotGraph::operator= (const KnotGraph &o)
{
    basic_knot_graph::operator= (o);
    copy_style(o);

    return *this;
}

void KnotGraph::copy_style(const KnotGraph &o)
{
    stroker.setCapStyle(o.stroker.capStyle());
    stroker.setCurveThreshold(o.stroker.curveThreshold());
    stroker.setJoinStyle(o.stroker.joinStyle());
    stroker.setMiterLimit(o.stroker.miterLimit());
    stroker.setWidth(o.stroker.width());
    setBrush(o.brush());
    setPen(o.pen());
}

void KnotGraph::add(CustomItem *what)
{
    add_node ( dynamic_cast<Node*>(what) );
    add_edge ( dynamic_cast<Edge*>(what) );
}

QPainterPath KnotGraph::build()
{

    QList<QPainterPath> paths = multi_build();
    QPainterPath path;
    foreach ( const QPainterPath& p, paths )
        path.addPath(p);

    /// \bug Qt? if not simplified weird artifacts on cusps
    QPainterPath styled = stroker.createStroke(path).simplified();
    styled.setFillRule(Qt::WindingFill);
    setPath(styled);

    return path;
}

QList<QPainterPath> KnotGraph::multi_build()
{
    path_builder path_b;
    build_knotline(path_b);

    return path_b.build();
}

void KnotGraph::set_width(double w)
{
    stroker.setWidth(w);
}

double KnotGraph::get_width() const
{
    return stroker.width();
}

Qt::PenJoinStyle KnotGraph::get_join_style() const
{
    return stroker.joinStyle();
}

void KnotGraph::set_join_style(Qt::PenJoinStyle pjs)
{
    stroker.setJoinStyle(pjs);
}

void KnotGraph::load_xml(xml_loader &xml)
{

    if ( xml.enter("style") )
    {
        QPen stroke(brush(),get_width());
        stroke.setJoinStyle(get_join_style());
        stroke = xml.get_pen("stroke",stroke);
        setBrush(stroke.brush());
        set_width(stroke.widthF());
        set_join_style(stroke.joinStyle());

        setPen ( xml.get_pen("outline", pen() ) );

        set_style_info ( xml.get_cusp( "cusp" ) );

        xml.leave();
    }


    xml.get_graph(*this);
}

void KnotGraph::save_xml(xml_saver &xml) const
{
    xml.start_element("style");
        QPen stroke(brush(),get_width());
        stroke.setJoinStyle(get_join_style());
        xml.save_pen ( "stroke", stroke, false, true );
        xml.save_pen ( "outline", pen(), true, false );
        xml.save_cusp ( "cusp", get_style_info() );
    xml.end_element();

    xml.save_graph("graph",nodes);
}


void KnotGraph::save_xml(QIODevice *device) const
{

    xml_saver xml(device);
    xml.begin();

    save_xml(xml);

    xml.end();
}

bool KnotGraph::load_xml(QIODevice *device)
{
    clear();

    xml_loader xml;

    if ( !xml.load(device) )
        return false;

    load_xml(xml);

    return true;
}

void KnotGraph::paint_knot(QPaintDevice *device, bool minimal )
{
    QPainter painter;
    painter.begin(device);

    paint_knot ( &painter, minimal );

    painter.end();
}

void KnotGraph::paint_knot(QPainter *painter, bool minimal)
{
    QPointF off = boundingRect().topLeft();
    painter->translate(-off.x(),-off.y()); // remove offset

    QList<QPainterPath> paths = multi_build();

    if ( minimal )
    {
        QPen stroke(brush(),stroker.width());
        stroke.setJoinStyle(stroker.joinStyle());
        stroke.setCapStyle(Qt::FlatCap);
        painter->setPen(stroke);
        foreach ( const QPainterPath& path, paths )
            painter->drawPath(path);
    }
    else
    {
        painter->setPen(pen());
        painter->setBrush(brush());

        foreach ( const QPainterPath& path, paths )
        {
            QPainterPath styled = stroker.createStroke(path).simplified();
            styled.setFillRule(Qt::WindingFill);
            painter->drawPath(styled);
        }
    }
}

void KnotGraph::export_svg(QIODevice &file, bool minimal)
{
    QSvgGenerator gen;
    gen.setOutputDevice(&file);

    paint_knot ( &gen, minimal );
}

void KnotGraph::export_raster(QIODevice &file, bool minimal,
    QColor background, bool antialias, QSize img_size, int quality )
{
    QSizeF actual_size = boundingRect().size();
    double scale_x = img_size.width() / actual_size.width();
    double scale_y = img_size.height() / actual_size.height();

    if ( antialias )
    {
        /// Letting QPainter handle antialiasing is not enough... :^(
        QImage original (boundingRect().size().toSize()*2, QImage::Format_ARGB32);
        original.fill(background.rgba());


        QPainter painter;
        painter.begin(&original);

        scale_x *= 2;
        scale_y *= 2;

        painter.scale(scale_x,scale_y);

        paint_knot ( &painter, minimal );

        painter.end();


        QImage supersampled(img_size, QImage::Format_ARGB32 );
        for ( int y = 0; y < supersampled.height(); y++ )
            for ( int x = 0; x < supersampled.width(); x++ )
            {
                QColor p1 = QColor::fromRgba(original.pixel(2*x,2*y));
                QColor p2 = QColor::fromRgba(original.pixel(2*x+1,2*y));
                QColor p3 = QColor::fromRgba(original.pixel(2*x,2*y+1));
                QColor p4 = QColor::fromRgba(original.pixel(2*x+1,2*y+1));
                QColor color ( (p1.red()+p2.red()+p3.red()+p4.red())/4,
                                (p1.green()+p2.green()+p3.green()+p4.green())/4,
                                (p1.blue()+p2.blue()+p3.blue()+p4.blue())/4,
                                (p1.alpha()+p2.alpha()+p3.alpha()+p4.alpha())/4
                              );
                supersampled.setPixel(x,y,color.rgba());
            }
        supersampled.save(&file,0,quality);
    }
    else
    {
        QPixmap pix (img_size);
        pix.fill(background);

        QPainter painter;
        painter.begin(&pix);

        painter.scale(scale_x,scale_y);

        paint_knot ( &painter, minimal );

        painter.end();

        pix.save(&file,0,quality);
    }
}

void KnotGraph::to_mime(QMimeData *data)
{

        QByteArray knot_xml;
        QBuffer xml_stream(&knot_xml);
        xml_stream.open(QIODevice::WriteOnly|QIODevice::Text);
        save_xml(&xml_stream);

        data->setData("application/x-knotter",knot_xml);
        data->setData("text/xml",knot_xml);

        QByteArray knot_svg;
        QBuffer svg_stream(&knot_svg);
        svg_stream.open(QIODevice::WriteOnly|QIODevice::Text);
        export_svg(svg_stream,true);
        data->setData("image/svg",knot_svg);
}




