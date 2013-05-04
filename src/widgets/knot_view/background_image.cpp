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

#include "background_image.hpp"
#include <QPainter>

Background_Image::Background_Image()
    : enabled(false), scale(1)
{
}

void Background_Image::render(QPainter *painter) const
{
    if ( enabled && ! image.isNull() )
    {
        painter->drawPixmap(pos.x(),pos.y(),
                            image.width()*scale, image.height()*scale,
                            image );
    }
}


void Background_Image::enable(bool enabled)
{
    this->enabled = enabled;
    emit changed();
}

void Background_Image::load_file(QString name)
{
    image.load(name);
    file = name;
    emit changed();
}

void Background_Image::set_position(QPointF position)
{
    pos = position;
    emit changed();
    emit moved(pos);
}


void Background_Image::set_scale_percent(double pc)
{
    scale = pc/100;
    emit changed();
}
