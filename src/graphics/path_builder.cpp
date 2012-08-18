#include "path_builder.hpp"
#include <QMap>

namespace path_item {

line::line ( QPointF begin, QPointF end )
    : begin ( begin ), end ( end ) {}


line* line::clone() const
{
    return new line ( begin, end );
}

void line::add_to(bool move, QPainterPath &ppth) const
{
    if ( move )
        ppth.moveTo(begin);

    ppth.lineTo(end);
}

line::~line()
{

}

cubiccurve::cubiccurve(QPointF begin, QPointF control1, QPointF control2, QPointF end)
    : line ( begin, end ), control1 ( control1 ), control2 ( control2 )
{}

void cubiccurve::add_to(bool move, QPainterPath &ppth) const
{
    if ( move )
        ppth.moveTo(begin);

    ppth.cubicTo(control1,control2,end);
}

quadcurve::quadcurve(QPointF begin, QPointF control, QPointF end)
    : line ( begin, end ), control ( control )
{}

void quadcurve::add_to(bool move, QPainterPath &ppth) const
{
    if ( move )
        ppth.moveTo(begin);

    ppth.quadTo(control,end);
}



}  // namespace path

path_builder::path_builder() {}

/// deep copy
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
}

path_builder::~path_builder()
{
    foreach(path_item::line* stroke, strokes)
        delete stroke;
    strokes.clear();
}

bool fuzzy_double_equals ( double p1, double p2 )
{
    return qFuzzyCompare(p1,p2);
    //return qAbs(p1-p2) <= 0.1*qMax(p1,p2);
}

inline bool fuzzy_point_equals ( QPointF p1, QPointF p2 )
{
    return fuzzy_double_equals(p1.x(),p2.x()) && fuzzy_double_equals(p1.y(),p2.y());
}

void path_builder::add_line(path_item::line *l)
{
    /*for ( iterator i = strokes.begin(); i != strokes.end(); ++i )
    {
        if ( fuzzy_point_equals((*i)->begin,l->end) )
        {
            strokes.insert(i,l);
            return;
        }
        else if ( fuzzy_point_equals((*i)->end,l->begin) )
        {
            strokes.insert(i+1,l);
            return;
        }
    }*/
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


struct sequence
{
    path_item::line* prev;
    path_item::line* next;
    sequence() : prev(NULL), next(NULL) {}
};

void path_builder::sort()
{
    if ( strokes.size() < 2 )
        return;

    QMap<path_item::line*,sequence> seq;

    /// Building data structure: n^2 log n
    for ( iterator i = strokes.begin(); i != strokes.end(); ++i ) // n
    {
        sequence &tsq = seq[*i]; // log n

        if ( tsq.next && tsq.prev )
            continue;

        for ( iterator j = i+1; j != strokes.end(); ++j ) // n
        {
            if ( !tsq.next && fuzzy_point_equals ( (*j)->begin, (*i)->end ) )
            {
                tsq.next = *j;
                seq[*j].prev = *i; // log n
            }

            if ( !tsq.prev && fuzzy_point_equals ( (*j)->end, (*i)->begin ) )
            {
                tsq.prev = *j;
                seq[*j].next = *i; // log n
            }
        }
    }

    QList<path_item::line*> newstrokes;

    /// Traversing data struture: n log n
    path_item::line* next_stroke = seq.begin().key();
    while ( !seq.empty() ) // n
    {
        sequence &prevnext = seq[next_stroke]; // log n
        if ( prevnext.prev && !newstrokes.contains(prevnext.prev) ) // log n
        {
            next_stroke = prevnext.prev;
        }
        else
        {
            newstrokes.push_back(next_stroke);
            seq.remove(next_stroke); // log n
            if ( prevnext.next )
                next_stroke = prevnext.next;
            else if ( seq.empty() )
                break;
            else
                next_stroke = seq.begin().key();
        }
    }
    strokes.clear();
    strokes = newstrokes; // n

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
        stroke->add_to(!fuzzy_point_equals(nextpoint,stroke->begin),ppth);
        nextpoint = stroke->end;
    }

    return ppth;
}
