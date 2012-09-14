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
#include "background_image.hpp"
#include <QPainter>

background_image::background_image()
    :  active(false), zoom(1)
{
}

void background_image::set_active(bool act)
{
    active = act;
}

void background_image::set_zoom(double zm)
{
    zoom = zm;
    update_rect();
}

void background_image::set_center(QPointF p)
{
    center = p;
    update_rect();
}

bool background_image::load(QString filename)
{
    if ( image.load(filename) )
    {
        file = filename;
        update_rect();
        return true;
    }
    else
    {
        active = false;
        file = "";
        return false;
    }
}

void background_image::render(QPainter *painter) const
{
    if ( active )
    {
        painter->drawPixmap(target,image,image.rect());
    }
}

void background_image::update_rect()
{
    target.setTopLeft(center-zoom*QPointF(image.size().width(),image.size().height())/2 );
    target.setSize( image.size()*zoom );
}
