#include "robot.hpp"

#include <QBrush>
#include <QPen>
#include <QGraphicsSceneHoverEvent>
#include <QCursor>

namespace icp {

/**
 * @brief Diameter of the robot.
 */
constexpr qreal ROBOT_DIAMETER = 50;
/**
 * @brief Thickness of the border around the robot.
 */
constexpr qreal BORDER_THICKNESS = 6;

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

Robot::Robot(QPoint position, QPointF speed, QGraphicsItem *parent) :
    QGraphicsEllipseItem(
        QRectF(position, QSizeF(ROBOT_DIAMETER, ROBOT_DIAMETER))
    ),
    grabbed(false),
    speed(speed)
{
    setBrush(QBrush(QColor(0x55, 0xcc, 0x55)));
    setPen(QPen(QColor(0xff, 0xff, 0xff), BORDER_THICKNESS));
    setAcceptHoverEvents(true);
}

void Robot::move(qreal delta) {
    move_by(speed * delta);
}

QRectF Robot::hitbox() {
    constexpr qreal ADJ = BORDER_THICKNESS / 2;
    return rect().adjusted(-ADJ, -ADJ, ADJ, ADJ);
}

void Robot::set_hitbox(QRectF hitbox) {
    constexpr qreal ADJ = BORDER_THICKNESS / 2;
    move_to(hitbox.topLeft() + QPointF(ADJ, ADJ));
}

//---------------------------------------------------------------------------//
//                                PROTECTED                                  //
//---------------------------------------------------------------------------//

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

//---------------------------------------------------------------------------//
//                                 PRIVATE                                   //
//---------------------------------------------------------------------------//

void Robot::hover_mouse() {
    if (grabbed) {
        setCursor(Qt::ClosedHandCursor);
    } else {
        setCursor(Qt::OpenHandCursor);
    }
}

void Robot::move_by(QPointF delta) {
    move_to(rect().topLeft() + delta);
}

void Robot::move_to(QPointF point) {
    auto rec = rect();
    rec.moveTopLeft(point);
    setRect(rec);
}

}
