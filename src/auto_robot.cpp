#include "auto_robot.hpp"

#include <QBrush>

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
    elide_dist(20),
    elide_rot(M_PI / M_E),
    rot_speed(M_PI / 4)
{
    setBrush(QBrush(QColor(0x55, 0x55, 0xcc)));
    auto rob = dynamic_cast<AutoRobot *>(r);
    if (rob) {
        elide_dist = rob->elide_dist;
        elide_rot = rob->elide_rot;
        rot_speed = rob->rot_speed;
    }
}

void AutoRobot::move(qreal delta, qreal distance) {
    if (rot_remain == 0 && distance <= elide_dist) {
        rot_remain = elide_rot;
        sspeed = speed();
        set_speed(0);
    }

    if (rot_remain != 0) {
        auto ang = rot_speed * delta;
        if (qAbs(rot_remain) < qAbs(ang)) {
            ang = qAbs(rot_remain);
            rot_remain = 0;
        } else {
            if (rot_remain > 0) {
                rot_remain -= ang;
            } else {
                rot_remain += ang;
            }
        }

        set_angle(orientation() + ang);

        if (rot_remain == 0) {
            set_speed(sspeed);
        }
    }

    Robot::move(delta, distance);
}

} // namespace icp

