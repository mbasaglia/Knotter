/**
  
\file

\author Mattia Basaglia

\section License
This file is part of Knotter.

Copyright (C) 2012-2014  Mattia Basaglia

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

#ifndef TRANSFORM_HANDLE_HPP
#define TRANSFORM_HANDLE_HPP

#include "graph_item.hpp"
#include <QSvgRenderer>

class Transform_Handle : public Graph_Item
{

public:
    /// What kind of transformation this handle applies
    enum Mode { SCALE, ROTATE, SIDE_SCALE, SKEW };

    static Mode side_mode ( Mode m )
    {
        switch ( m )
        {
            case SCALE:  return SIDE_SCALE;
            case ROTATE: return SKEW;
            default:     return m;
        }
    }

protected:
    static QSvgRenderer scale_rest; ///< SVG Image to display while at rest
    static QSvgRenderer scale_active;///< SVG Image to display when highlight == true
    static QSvgRenderer rotate_rest; ///< SVG Image to display while at rest
    static QSvgRenderer rotate_active;///< SVG Image to display when highlight == true
    static QSvgRenderer side_scale_rest; ///< SVG Image to display while at rest
    static QSvgRenderer side_scale_active;///< SVG Image to display when highlight == true
    static QSvgRenderer skew_rest; ///< SVG Image to display while at rest
    static QSvgRenderer skew_active;///< SVG Image to display when highlight == true
    static double       m_image_size; ///< Size of the image
    static bool         images_initialized; ///< whether static images have been initialized



    Mode    m_mode;
    int     m_image_angle;///< Angle for which the image has to be rotated
    double  m_angle;      ///< Dynamic rotation

public:
    explicit Transform_Handle(Mode mode = SCALE, int image_angle = 0);

    QRectF boundingRect() const override;

    void set_mode ( Mode mode );
    Mode mode() const { return m_mode; }

    int image_angle() const { return m_image_angle; }
    void set_image_angle( int angle );

    static double image_size() { return image_size(); }

    void set_angle(double angle);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // TRANSFORM_HANDLE_HPP
