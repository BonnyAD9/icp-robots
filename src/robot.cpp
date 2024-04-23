#include "robot.hpp"

#include <QBrush>
#include <QPen>
#include <QGraphicsSceneHoverEvent>
#include <QCursor>

namespace icp {

constexpr qreal ROBOT_DIAMETER = 50;

Robot::Robot(QPoint position, QPointF speed, QGraphicsItem *parent) :
    QGraphicsEllipseItem(
        QRectF(position, QSizeF(ROBOT_DIAMETER, ROBOT_DIAMETER))
    ),
    grabbed(false),
    speed(speed)
{
    setBrush(QBrush(QColor(0x55, 0xcc, 0x55)));
    setPen(QPen(QColor(0xff, 0xff, 0xff), 6));
    setAcceptHoverEvents(true);
}

void Robot::move(qreal delta) {
    move_by(speed * delta);
}

void Robot::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    grabbed = true;
    grabMouse();
    hover_mouse();
}

void Robot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    grabbed = false;
    ungrabMouse();
    hover_mouse();
}

void Robot::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    move_by(event->scenePos() - event->lastScenePos());
}

void Robot::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    hover_mouse();
}

void Robot::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    hover_mouse();
}

void Robot::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    unsetCursor();
}

void Robot::hover_mouse() {
    if (grabbed) {
        setCursor(Qt::ClosedHandCursor);
    } else {
        setCursor(Qt::OpenHandCursor);
    }
}

void Robot::move_by(QPointF delta) {
    auto rec = rect();
    rec.moveTopLeft(rec.topLeft() + delta);
    setRect(rec);
}

}
