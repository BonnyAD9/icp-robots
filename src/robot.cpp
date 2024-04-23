#include "robot.hpp"

#include <QBrush>

namespace icp {

constexpr qreal ROBOT_DIAMETER = 50;

Robot::Robot(QPoint position, QGraphicsItem *parent) :
    QGraphicsEllipseItem(
        QRectF(position, QSizeF(ROBOT_DIAMETER, ROBOT_DIAMETER))
    )
{
    setBrush(QBrush(QColor(0x55, 0xdd, 0x55)));
}

}
