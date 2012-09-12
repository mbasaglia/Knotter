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
#ifndef TRANSFORM_HANDLE_HPP
#define TRANSFORM_HANDLE_HPP

#include <QGraphicsItem>
#include "custom_item.hpp"
#include <QSvgRenderer>

/**
    \brief flags for Transform_Handle

    Using TH:: as it's way shorter than Transform_Handle::
*/
namespace TH {

    enum PositionPrimitive
    {
        TOP = 0x01,
        BOTTOM = 0x02,
        LEFT = 0x10,
        RIGHT = 0x20
    };

    enum Mode { ROTATE, SCALE };

    Q_DECLARE_FLAGS(Position, PositionPrimitive)

    Q_DECLARE_OPERATORS_FOR_FLAGS(Position)

} // namespace TH

/**
    \brief Little handle used to scale selections

    This class does nothing more than drawing a little arrow with the correct
    orientation, it's the user that has to manipulate it and give it meaning.
*/
class Transform_Handle : public CustomItem
{

    public:
        static QSvgRenderer scale_rest; ///< SVG Image to display while at rest
        static QSvgRenderer scale_active;///< SVG Image to display when highlight == true
        static QSvgRenderer rotate_rest; ///< SVG Image to display while at rest
        static QSvgRenderer rotate_active;///< SVG Image to display when highlight == true

        static double size; ///< Size of the image, could be better

        TH::Position position; ///< Position in the bounding box ( affect how the arrow is oriented )
        TH::Mode mode;
    public:

        explicit Transform_Handle(TH::Position position, TH::Mode mode = TH::SCALE );

        QRectF boundingRect() const;

        void paint( QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget);
        
};


#endif // TRANSFORM_HANDLE_HPP
