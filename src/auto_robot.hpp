#pragma once
#define _USE_MATH_DEFINES

#include <cmath>

#include "robot.hpp"

namespace icp {

class AutoRobot : public Robot {
public:
    explicit AutoRobot(
        QPoint position,
        QPointF step = QPoint(0, 20),
        qreal elide_dist = 20,
        qreal elide_rot = M_PI / 2.5,
        qreal rot_speed = M_PI / 4, // angle per secod
        QGraphicsItem *parent = nullptr
    );

    explicit AutoRobot(
        QPoint position,
        qreal angle,
        qreal speed,
        qreal elide_dist = 20,
        qreal elide_rot = M_PI / 2,
        qreal rot_speed = M_PI / 4, // angle per secod
        QGraphicsItem *parent = nullptr
    );

    void move(qreal delta, qreal distance) override;

private:
    qreal sspeed;
    qreal rot_remain;

    qreal elide_dist;
    qreal elide_rot;
    qreal rot_speed;
};

} // namespace icp
