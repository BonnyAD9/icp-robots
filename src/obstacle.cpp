#include "obstacle.hpp"

#include <QBrush>

namespace icp {

Obstacle::Obstacle(QRectF hitbox, QGraphicsItem *parent)
    : QGraphicsRectItem(hitbox, parent)
{
    setBrush(QBrush(QColor(0xff, 0x55, 0x55)));
}

}
