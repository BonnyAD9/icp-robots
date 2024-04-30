#pragma once

#include "robot.hpp"

namespace icp {

/**
 * @brief Controlled robot by the user
 */
class ControlRobot : public Robot {
public:
    /**
     * @brief Creates new controlled robot
     * @param position Initial position of the robot
     * @param step The speed and initial orientation of the robot
     * @param elide_dist How far from obstacle robot should stop
     * @param parent Parent object
     */
    explicit ControlRobot(
        QPoint position,
        QPointF step = QPoint(0, 20),
        qreal rot_speed = M_PI / 4,
        QGraphicsItem *parent = nullptr
    );

    /**
     * @brief Creates new controlled robot
     * @param position Initial position of the robot
     * @param angle Initial orientation of the robot
     * @param speed Initial speed of the robot
     * @param elide_dist How far from obstacle robot should stop
     * @param parent Parent object
     */
    explicit ControlRobot(
        QPoint position,
        qreal angle,
        qreal speed,
        qreal rot_speed = M_PI / 4,
        QGraphicsItem *parent = nullptr
    );

    /**
     * @brief Moves the robot.
     * @param delta How much time (in second) passed since the last tick.
     * @param distance Distance to the closest obstacle in front of the robot.
     */
    void move(qreal delta, qreal distance) override;

    /**
     * @brief Starts moving robot forward
     */
    void forward(bool start = true);

    /**
     * @brief Starts rotating robot left
     */
    void left(bool start = true);

    /**
     * @brief Starts rotating robot right
     */
    void right(bool start = true);

private:
    qreal sspeed;
    qreal rot_speed;

    qreal cur_speed;
    qreal cur_rot_speed;
};

} // namespace icp
