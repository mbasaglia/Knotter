#include "transform_handle.hpp"
#include <QPainter>

using namespace TH;

QSvgRenderer Transform_Handle::scale_rest;
QSvgRenderer Transform_Handle::scale_active;
double Transform_Handle::size = 24;

Transform_Handle::Transform_Handle(TH::Position position ) :
    position(position)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

QRectF Transform_Handle::boundingRect() const
{
    return QRectF(QPointF(-size/2,-size/2),QSizeF(size,size));
}

void Transform_Handle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QSvgRenderer& rend = highlight ? scale_active : scale_rest;

    if ( position == TOP || position == BOTTOM )
        painter->rotate(90);
    else if ( position == (TOP|RIGHT) || position == (BOTTOM|LEFT) )
        painter->rotate(-45);
    else if ( position == (TOP|LEFT) || position == (BOTTOM|RIGHT) )
        painter->rotate(45);

    rend.render(painter,boundingRect());

}
