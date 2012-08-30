#include "path_builder.hpp"
#include <QMap>

path_builder::path_builder() {}

/*/// deep copy
path_builder::path_builder(const path_builder &o)
{
    foreach(path_item::line* stroke, o.strokes)
        strokes.push_back(stroke->clone());
}

path_builder &path_builder::operator= (const path_builder &o)
{
    this->~path_builder();

    foreach(path_item::line* stroke, o.strokes)
        strokes.push_back(stroke->clone());

    return *this;
}*/

path_builder::~path_builder()
{
    foreach(path_item::line* stroke, strokes)
        delete stroke;
    strokes.clear();
}

void path_builder::add_line(path_item::line *l)
{
    for ( iterator i = strokes.begin(); i != strokes.end(); ++i )
    {
        if ( path_item::adjacent(*i,l) )
        {
            path_item::line* c = path_item::merge(*i,l);
            strokes.erase(i);
            for ( iterator j = strokes.begin(); j != strokes.end(); )
            {
                if ( *j != c && path_item::adjacent(c,*j) )
                {
                    c = path_item::merge(c,*j);
                    strokes.erase(j);
                    j = strokes.begin();
                }
                else
                    ++j;
            }
            strokes.push_back(c);
            return;
        }

    }

    strokes.push_back(l);
}

void path_builder::add_line(QPointF begin, QPointF end)
{
    add_line ( new path_item::line ( begin, end ) );
}

void path_builder::add_cubic(QPointF begin, QPointF control1, QPointF control2, QPointF end)
{
    add_line ( new path_item::cubiccurve(begin,control1,control2,end) );
}

void path_builder::add_quad(QPointF begin, QPointF control, QPointF end)
{
    add_line ( new path_item::quadcurve ( begin, control, end ) );
}

QPainterPath path_builder::build()
{
    if ( strokes.empty() )
        return QPainterPath();

    QPainterPath ppth;
    QPointF nextpoint = strokes[0]->begin;
    ppth.moveTo(nextpoint);

    foreach ( path_item::line* stroke, strokes )
    {
        stroke->add_to(true,ppth);
        nextpoint = stroke->end;
    }

    return ppth;
}
