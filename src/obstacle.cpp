#include "obstacle.hpp"

#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

namespace icp {

Obstacle::Obstacle(QRectF hitbox, QGraphicsItem *parent)
    : QGraphicsRectItem(hitbox, parent), dragging(false)
{
    setBrush(QBrush(QColor(0xff, 0x55, 0x55)));
    setPen(QPen(QColor(0xff, 0xff, 0xff)));
}

void Obstacle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    grabMouse();
    dragging = true;
}

void Obstacle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    ungrabMouse();
    dragging = false;
}

void Obstacle::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto delta = event->scenePos() - event->lastScenePos();
    moveBy(delta.x(), delta.y());
}

}
