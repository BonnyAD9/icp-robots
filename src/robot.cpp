#include "robot.hpp"

#include <cmath>

#include <QBrush>
#include <QPen>
#include <QGraphicsSceneHoverEvent>
#include <QCursor>

namespace icp {

using namespace std;

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
    angle(atan2(speed.y(), speed.x())),
    mspeed(sqrt(speed.x() * speed.x() + speed.y() * speed.y())),
    grabbed(false),
    last_move_vec(0, 0)
{
    setBrush(QBrush(QColor(0x55, 0xcc, 0x55)));
    setPen(QPen(QColor(0xff, 0xff, 0xff), BORDER_THICKNESS));
    setAcceptHoverEvents(true);
}

void Robot::move(qreal delta, qreal distance) {
    last_move_vec = step() * delta;
    move_by(last_move_vec);
}

QPointF Robot::last_move() {
    return last_move_vec;
}

QRectF Robot::hitbox() const {
    constexpr qreal ADJ = BORDER_THICKNESS / 2;
    return rect().adjusted(-ADJ, -ADJ, ADJ, ADJ);
}

void Robot::set_hitbox(QRectF hitbox) {
    constexpr qreal ADJ = BORDER_THICKNESS / 2;
    move_to(hitbox.topLeft() + QPointF(ADJ, ADJ));
}

QPointF Robot::step() {
    return orientation_vec() * mspeed;
}

qreal Robot::orientation() {
    return angle;
}

QPointF Robot::orientation_vec() {
    return QPointF(cos(angle), sin(angle));
}

qreal Robot::speed() {
    return mspeed;
}

//---------------------------------------------------------------------------//
//                                PROTECTED                                  //
//---------------------------------------------------------------------------//

void Robot::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        grabbed = true;
        grabMouse();
        hover_mouse();
    }
}

void Robot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        grabbed = false;
        ungrabMouse();
        hover_mouse();
    }
}

void Robot::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (is_grabbed()) {
        move_by(event->scenePos() - event->lastScenePos());
    }
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
