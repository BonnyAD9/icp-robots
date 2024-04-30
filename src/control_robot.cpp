#include "control_robot.hpp"

#include <QBrush>

namespace icp {

using namespace std;

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

ControlRobot::ControlRobot(
    QPoint position,
    QPointF step,
    qreal rot_speed,
    QGraphicsItem *parent
) : ControlRobot(
    position,
    atan2(step.y(), step.x()),
    sqrt(step.x() * step.x() + step.y() * step.y()),
    rot_speed,
    parent
) {}

ControlRobot::ControlRobot(
    QPoint position,
    qreal angle,
    qreal speed,
    qreal rot_speed,
    QGraphicsItem *parent
) : Robot(position, angle, 0, parent),
    sspeed(speed),
    rot_speed(rot_speed),
    cur_speed(0),
    cur_rot_speed(0)
{
    setBrush(QBrush(QColor(0x55, 0xcc, 0x55)));
}

void ControlRobot::move(qreal delta, qreal distance) {
    if (distance == 0) {
        sspeed = speed();
        set_speed(0);
    } else {
        set_speed(cur_speed);
    }

    if (cur_rot_speed != 0) {
        auto ang = cur_rot_speed * delta;
        set_angle(orientation() + ang);
    }

    Robot::move(delta, distance);
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
