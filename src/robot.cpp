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

Robot::Robot(QPoint position, QPointF step, QGraphicsItem *parent) :
    Robot(
        position,
        atan2(step.y(), step.x()),
        sqrt(step.x() * step.x() + step.y() * step.y()),
        parent
) {}

Robot::Robot(
    QPoint position,
    qreal angle,
    qreal speed,
    QGraphicsItem *parent
) :
    QGraphicsEllipseItem(
        QRectF(position, QSizeF(ROBOT_DIAMETER, ROBOT_DIAMETER))
    ),
    grabbed(false),
    last_move_vec(0, 0)
{
    setBrush(QBrush(QColor(0xcc, 0x55, 0xcc)));
    setPen(QPen(QColor(0xff, 0xff, 0xff), BORDER_THICKNESS));
    setAcceptHoverEvents(true);

    eye = new QGraphicsEllipseItem(
        QRectF(0, 0, BORDER_THICKNESS, BORDER_THICKNESS),
        this
    );
    eye->setBrush(QBrush(QColor(0xff, 0xff, 0xff)));
    eye->setPen(QPen(QColor(0, 0, 0, 0)));
    set_angle(angle);
    set_speed(speed);
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

void Robot::set_selected(bool val) {
    if (val) {
        setPen(QPen(QColor(0xff, 0xff, 0x55), BORDER_THICKNESS));
        emit select(this);
    } else {
        setPen(QPen(QColor(0xff, 0xff, 0xff), BORDER_THICKNESS));
        emit select(nullptr);
    }
}

//---------------------------------------------------------------------------//
//                                PROTECTED                                  //
//---------------------------------------------------------------------------//

void Robot::set_step(QPointF step) {
    set_angle(step);
    set_speed(sqrt(step.x() * step.x() + step.y() * step.y()));
}

void Robot::set_angle(qreal angle) {
    this->angle = angle;
    auto e = eye->rect();
    e.moveCenter(
        rect().center() + orientation_vec() * (ROBOT_DIAMETER / 3)
    );
    eye->setRect(e);
}

void Robot::set_angle(QPointF angle) {
    set_angle(atan2(angle.y(), angle.x()));
}

void Robot::set_speed(qreal speed) {
    mspeed = speed;
}

void Robot::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    set_selected();

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

    // ensure that the eye of the robot is updated
    set_angle(angle);
}

}
