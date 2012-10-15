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
#ifndef BACKGROUND_IMAGE_HPP
#define BACKGROUND_IMAGE_HPP

#include <QPixmap>

/**
    \brief Background image for KnotView
*/
class background_image
{
    protected:

        bool    active;

        QString file;
        QPixmap image;

        QRectF  target;
        QPointF center;
        double  zoom;

    public:

        background_image();

        void set_active ( bool = true );

        void set_zoom ( double zoom );
        bool get_zoom() const { return zoom; }

        void set_center ( QPointF p );
        QPointF get_center () const { return center; }

        bool load ( QString filename );

        const QPixmap& get_image() const { return image; }

        QString get_file_name() const { return file; }

        void render(QPainter* painter) const;

    protected:
        void update_rect();
};

#endif // BACKGROUND_IMAGE_HPP
