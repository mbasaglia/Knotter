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
#include "transform_handle.hpp"
#include <QPainter>
#include <QCursor>

using namespace TH;

QSvgRenderer Transform_Handle::scale_rest;
QSvgRenderer Transform_Handle::scale_active;
QSvgRenderer Transform_Handle::rotate_rest;
QSvgRenderer Transform_Handle::rotate_active;
double Transform_Handle::size = 24;

Transform_Handle::Transform_Handle(TH::Position position, Mode mode ) :
    position(position), mode ( mode )
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
    update_cursor();
}

QRectF Transform_Handle::boundingRect() const
{
    return QRectF(QPointF(-size/2,-size/2),QSizeF(size,size));
}

void Transform_Handle::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if ( mode == SCALE )
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
    else
    {
        QSvgRenderer& rend = highlight ? rotate_active : rotate_rest;

        if ( position == (TOP|RIGHT)  )
            painter->rotate(90);
        else if ( position == (BOTTOM|RIGHT) )
            painter->rotate(180);
        else if ( position == (BOTTOM|LEFT)  )
            painter->rotate(-90);

        rend.render(painter,boundingRect());
    }
}

void Transform_Handle::update_cursor()
{
    if ( mode == ROTATE )
        setCursor(Qt::ArrowCursor); /// \todo custom cursor?
    else if ( position == TOP || position == BOTTOM )
        setCursor(Qt::SizeVerCursor);
    else if ( position == LEFT || position == RIGHT )
        setCursor(Qt::SizeHorCursor);
    else if ( position == (TOP|RIGHT) || position == (BOTTOM|LEFT) )
        setCursor(Qt::SizeBDiagCursor);
    else if ( position == (TOP|LEFT) || position == (BOTTOM|RIGHT) )
        setCursor(Qt::SizeFDiagCursor);

}

void Transform_Handle::set_position(TH::Position pos)
{
    position = pos;
    update_cursor();
}

void Transform_Handle::set_mode(Mode mod)
{
    mode = mod;
    update_cursor();
}
