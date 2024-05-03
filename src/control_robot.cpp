/**
 * @file
 * @authors Martin Slezák (xsleza26), Jakub Antonín Štigler (xstigl00)
 * @brief Robot that can be controled by the user. (source file)
 */

#include "control_robot.hpp"

#include <QBrush>

#include "auto_robot.hpp"

namespace icp {

using namespace std;

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

ControlRobot::ControlRobot(
    QPointF position,
    QPointF step,
    qreal rot_speed,
    QObject *parent
) : ControlRobot(
    position,
    atan2(step.y(), step.x()),
    sqrt(step.x() * step.x() + step.y() * step.y()),
    rot_speed,
    parent
) {}

ControlRobot::ControlRobot(
    QPointF position,
    qreal angle,
    qreal speed,
    qreal rot_speed,
    QObject *parent
) : Robot(position, angle, 0, parent),
    sspeed(speed),
    rot_speed(rot_speed),
    cur_speed(0),
    cur_rot_speed(0)
{
    setBrush(QBrush(QColor(0x55, 0xcc, 0x55)));
}

ControlRobot::ControlRobot(Robot *r) :
    Robot(r),
    sspeed(r->speed()),
    rot_speed(M_PI / M_E),
    cur_speed(0),
    cur_rot_speed(0)
{
    setBrush(QBrush(QColor(0x55, 0xcc, 0x55)));

    auto arob = dynamic_cast<AutoRobot *>(r);
    if (arob) {
        rot_speed = arob->rspeed();
    }

    auto crob = dynamic_cast<ControlRobot *>(r);
    if (crob) {
        rot_speed = crob->rspeed();
    }
}

void ControlRobot::move(qreal delta, qreal distance) {
    if (distance == 0) {
        sspeed = Robot::speed();
        Robot::set_speed(0);
    } else {
        Robot::set_speed(cur_speed);
    }

    if (cur_rot_speed != 0) {
        auto ang = cur_rot_speed * delta;
        Robot::set_angle(orientation() + ang);
    }

    Robot::move(delta, distance);
}

qreal ControlRobot::speed() {
    if (cur_speed == 0) {
        return sspeed;
    }
    return Robot::speed();
}

void ControlRobot::set_speed(qreal speed) {
    if (cur_speed == 0) {
        sspeed = speed;
    }
    Robot::set_speed(speed);
}

void ControlRobot::save(ofstream &file) {
    auto ang = get_mod_angle(360);
    if (ang < -180) {
        ang += 360;
    }

    file << "control_robot: [" << hitbox().x() << ", " << hitbox().y()
        << "] { speed: " << speed() << ", rotation_speed: " << rot_speed
        << ", angle: " << ang << " }" << endl;
}

qreal ControlRobot::rspeed() const {
    return rot_speed;
}

void ControlRobot::set_rspeed(qreal speed) {
    rot_speed = speed;
}

void ControlRobot::forward(bool start) {
    cur_speed = start ? sspeed : 0;
}

void ControlRobot::right(bool start) {
    if (start) {
        cur_rot_speed += rot_speed;
    } else {
        cur_rot_speed -= rot_speed;
    }
}

void ControlRobot::left(bool start) {
    if (start) {
        cur_rot_speed -= rot_speed;
    } else {
        cur_rot_speed += rot_speed;
    }
}

} // namespace icp
