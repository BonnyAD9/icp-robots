/**
 * @file
 * @authors Martin Slezák (xsleza26), Jakub Antonín Štigler (xstigl00)
 * @brief Robot with basic AI (source file)
 */

#include "auto_robot.hpp"

#include <QBrush>

#include "control_robot.hpp"

namespace icp {

using namespace std;

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

AutoRobot::AutoRobot(
    QPointF position,
    QPointF step,
    qreal elide_dist,
    qreal elide_rot,
    qreal rot_speed,
    QObject *parent
) : AutoRobot(
    position,
    atan2(step.y(), step.x()),
    sqrt(step.x() * step.x() + step.y() * step.y()),
    elide_dist,
    elide_rot,
    rot_speed,
    parent
) {}

AutoRobot::AutoRobot(
    QPointF position,
    qreal angle,
    qreal speed,
    qreal elide_dist,
    qreal elide_rot,
    qreal rot_speed,
    QObject *parent
) : Robot(position, angle, speed, parent),
    sspeed(speed),
    rot_remain(0),
    elide_dist(elide_dist),
    elide_rot(elide_rot),
    rot_speed(rot_speed)
{
    setBrush(QBrush(QColor(0x55, 0x55, 0xcc)));
}

AutoRobot::AutoRobot(Robot *r) :
    Robot(r),
    sspeed(r->speed()),
    rot_remain(0),
    elide_dist(20),
    elide_rot(M_PI / M_E),
    rot_speed(M_PI / 4)
{
    setBrush(QBrush(QColor(0x55, 0x55, 0xcc)));

    auto arob = dynamic_cast<AutoRobot *>(r);
    if (arob) {
        elide_dist = arob->elide_dist;
        elide_rot = arob->elide_rot;
        rot_speed = arob->rot_speed;
    }

    auto crob = dynamic_cast<ControlRobot *>(r);
    if (crob) {
        rot_speed = crob->rspeed();
    }
}

void AutoRobot::move(qreal delta, qreal distance) {
    if (rot_remain == 0 && distance <= elide_dist) {
        rot_remain = elide_rot;
        sspeed = Robot::speed();
        Robot::set_speed(0);
    }

    if (rot_remain != 0) {
        auto ang = rot_speed * delta;
        ang = rot_remain < 0 ? -ang : ang;
        if (qAbs(rot_remain) < qAbs(ang)) {
            ang = rot_remain;
            rot_remain = 0;
        } else {
            rot_remain -= ang;
        }

        set_angle(orientation() + ang);

        if (rot_remain == 0) {
            Robot::set_speed(sspeed);
        }
    }

    Robot::move(delta, distance);
}

qreal AutoRobot::speed() {
    if (rot_remain != 0) {
        return sspeed;
    } else {
        return Robot::speed();
    }
}

void AutoRobot::set_speed(qreal speed) {
    if (rot_remain != 0) {
        sspeed = speed;
    } else {
        Robot::set_speed(speed);
    }
}

void AutoRobot::save(ofstream &file) {
    auto ang = get_mod_angle(360);
    if (ang < -180) {
        ang += 360;
    }

    file << "auto_robot: [" << hitbox().x() << ", " << hitbox().y()
        << "] { speed: " << speed() << ", rotation_speed: "
        << rot_speed / M_PI * 180 << ", elide_distance: " << elide_dist
        << ", elide_rotation: " << elide_rot / M_PI * 180
        << ", angle: " << ang << " }" << endl;
}

qreal AutoRobot::edist() const {
    return elide_dist;
}

void AutoRobot::set_edist(qreal dist) {
    elide_dist = dist;
}

qreal AutoRobot::rspeed() const {
    return rot_speed;
}

void AutoRobot::set_rspeed(qreal rspeed) {
    rot_speed = rspeed;
}

qreal AutoRobot::rdist() const {
    return elide_rot;
}

void AutoRobot::set_rdist(qreal dist) {
    elide_rot = dist;
}

} // namespace icp

