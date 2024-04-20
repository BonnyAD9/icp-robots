#include "obstacle.hpp"

#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

namespace icp {

Obstacle::Obstacle(QRectF hitbox, QGraphicsItem *parent)
    : QGraphicsRectItem(hitbox, parent)
{
    setBrush(QBrush(QColor(0xff, 0x55, 0x55)));
    setPen(QPen(QColor(0xff, 0xff, 0xff)));
    setFlag(ItemIsMovable);
}

}
