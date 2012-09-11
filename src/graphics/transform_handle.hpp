#ifndef TRANSFORM_HANDLE_HPP
#define TRANSFORM_HANDLE_HPP

#include <QGraphicsItem>
#include "custom_item.hpp"
#include <QSvgRenderer>

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

class Transform_Handle : public CustomItem
{

    public:
        static QSvgRenderer scale_rest;
        static QSvgRenderer scale_active;

        static double size;

        TH::Position position;

    public:

        explicit Transform_Handle(TH::Position position);

        QRectF boundingRect() const;

        void paint( QPainter *painter,
                    const QStyleOptionGraphicsItem *option,
                    QWidget *widget);
        
};


#endif // TRANSFORM_HANDLE_HPP
