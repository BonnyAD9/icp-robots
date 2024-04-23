#include "robot.hpp"

#include <QBrush>
#include <QPen>
#include <QGraphicsSceneHoverEvent>
#include <QCursor>

namespace icp {

constexpr qreal ROBOT_DIAMETER = 50;

Robot::Robot(QPoint position, QGraphicsItem *parent) :
    QGraphicsEllipseItem(
        QRectF(position, QSizeF(ROBOT_DIAMETER, ROBOT_DIAMETER))
    ),
    grabbed(false)
{
    setBrush(QBrush(QColor(0x55, 0xcc, 0x55)));
    setPen(QPen(QColor(0xff, 0xff, 0xff), 6));
    setAcceptHoverEvents(true);
}

void Robot::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    grabbed = true;
    grabMouse();
    hoverMouse();
}

void Robot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    grabbed = false;
    ungrabMouse();
    hoverMouse();
}

void Robot::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto delta = event->scenePos() - event->lastScenePos();

    auto rec = rect();
    rec.moveTopLeft(rec.topLeft() + delta);

    setRect(rec);
}

void Robot::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    hoverMouse();
}

void Robot::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    hoverMouse();
}

void Robot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    unsetCursor();
}

void Robot::hoverMouse() {
    if (grabbed) {
        setCursor(Qt::ClosedHandCursor);
    } else {
        setCursor(Qt::OpenHandCursor);
    }
}

}
