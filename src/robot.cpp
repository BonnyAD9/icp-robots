/**
 * @file
 * @authors Martin Slezák (xsleza26), Jakub Antonín Štigler (xstigl00)
 * @brief Robot that can be placed into a room. Base class for all other
 * robots. (source file)
 */

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

Robot::Robot(QPointF position, QPointF step, QObject *parent) :
    Robot(
        position,
        atan2(step.y(), step.x()),
        sqrt(step.x() * step.x() + step.y() * step.y()),
        parent
) {}

Robot::Robot(
    QPointF position,
    qreal angle,
    qreal speed,
    QObject *parent
) :
    QGraphicsEllipseItem(
        QRectF(position, QSizeF(ROBOT_DIAMETER, ROBOT_DIAMETER))
    ),
    grabbed(false)
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

Robot::Robot(Robot *other) : Robot(
    other->rect().topLeft(),
    other->angle,
    other->mspeed,
    other->parent()
) {}

void Robot::move(qreal delta, qreal distance) {
    move_by(step() * delta);
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

void Robot::set_step(QPointF step) {
    set_angle(step);
    set_speed(sqrt(step.x() * step.x() + step.y() * step.y()));
}

void Robot::set_angle(qreal angle) {
    auto em = this->angle != angle;
    this->angle = angle;
    auto e = eye->rect();
    e.moveCenter(
        rect().center() + orientation_vec() * (ROBOT_DIAMETER / 3)
    );
    eye->setRect(e);
    if (em) {
        emit angle_change(this->angle);
    }
}

void Robot::set_angle(QPointF angle) {
    set_angle(atan2(angle.y(), angle.x()));
}

qreal Robot::get_mod_angle(unsigned mod) {
    auto ang = -orientation() / M_PI * 180;
    auto sign = ang > 0 ? 1 : -1;
    ang = qAbs(ang);
    auto rnum = (int)ang % mod;
    return sign * (rnum + (ang - (int)ang));
}

void Robot::set_speed(qreal speed) {
    mspeed = speed;
}

void Robot::save(ofstream &file) {
    auto ang = get_mod_angle(360);
    if (ang < -180) {
        ang += 360;
    }

    file << "robot: [" << hitbox().x() << ", " << hitbox().y() << "] { speed: "
        << mspeed << ", angle: " << ang << " }" << endl;

    
}

//---------------------------------------------------------------------------//
//                                PROTECTED                                  //
//---------------------------------------------------------------------------//

void Robot::selection_event(bool selected) {
    if (selected) {
        setPen(QPen(QColor(0xff, 0xff, 0x55), BORDER_THICKNESS));
    } else {
        setPen(QPen(QColor(0xff, 0xff, 0xff), BORDER_THICKNESS));
    }
}

void Robot::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    setZValue(1);
    set_selected();

    if (event->button() & Qt::LeftButton) {
        grabbed = true;
        grabMouse();
        hover_mouse();
    }
}

void Robot::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    setZValue(0);
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
