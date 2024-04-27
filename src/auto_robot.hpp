#pragma once
#define _USE_MATH_DEFINES

#include <cmath>

#include "robot.hpp"

namespace icp {

/**
 * @brief Robot with basic AI.
 */
class AutoRobot : public Robot {
public:
    /**
     * @brief Craetes new robot with basic AI.
     * @param position Initial position of the robot.
     * @param step The speed and initial orientation of the robot.
     * @param elide_dist How far from obstacle the robot stops and starts to
     * rotate.
     * @param elide_rot How much the robot rotates when it detects obstacle.
     * @param rot_speed How fast the robot rotates (angle per second).
     * @param parent The parent object.
     */
    explicit AutoRobot(
        QPoint position,
        QPointF step = QPoint(0, 20),
        qreal elide_dist = 20,
        qreal elide_rot = M_PI / M_E,
        qreal rot_speed = M_PI / 4,
        QGraphicsItem *parent = nullptr
    );

    /**
     * @brief Craetes new robot with basic AI.
     * @param position initial position of the robot.
     * @param angle The initial orientation of the robot.
     * @param speed The speed of the robot.
     * @param elide_dist How far from obstacle the robot stops and starts to
     * rotate.
     * @param elide_rot How much the robot rotates when it detects obstacle.
     * @param rot_speed How fast the robot rotates (angle per second).
     * @param parent The parent object.
     */
    explicit AutoRobot(
        QPoint position,
        qreal angle,
        qreal speed,
        qreal elide_dist = 20,
        qreal elide_rot = M_PI / 2,
        qreal rot_speed = M_PI / 4, // angle per secod
        QGraphicsItem *parent = nullptr
    );

    /**
     * @brief Moves the robot.
     * @param delta How much time (in second) passed since the last tick.
     * @param distance Distance to the closest obstacle in front of the robot.
     */
    void move(qreal delta, qreal distance) override;

private:
    qreal sspeed;
    qreal rot_remain;

    qreal elide_dist;
    qreal elide_rot;
    qreal rot_speed;
};

} // namespace icp
