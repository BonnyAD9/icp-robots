/**
 * @file
 * @authors Martin Slezák (xsleza26), Jakub Antonín Štigler (xstigl00)
 * @brief Robot that can be controled by the user. (header file)
 */

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
        QPointF position,
        QPointF step = QPoint(0, 20),
        qreal rot_speed = M_PI / 4,
        QObject *parent = nullptr
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
        QPointF position,
        qreal angle,
        qreal speed,
        qreal rot_speed = M_PI / 4,
        QObject *parent = nullptr
    );

    /**
     * @brief Creates new controlled robot based on given robot
     * @param r robot to create controll robot based on
     */
    explicit ControlRobot(Robot *r);

    /**
     * @brief Moves the robot.
     * @param delta How much time (in second) passed since the last tick.
     * @param distance Distance to the closest obstacle in front of the robot.
     */
    void move(qreal delta, qreal distance) override;

    /**
     * @brief Gets the movement speed of the robot. (pixels per second)
     */
    virtual qreal speed() override;

    /**
     * @brief Sets the movement speed of the robot. (pixels per second)
     */
    virtual void set_speed(qreal speed) override;

    virtual void save(std::ofstream &file) override;

    /**
     * @brief Gets rotation speed of the robot
     */
    qreal rspeed() const;

    /**
     * @brief Sets rotation speed of the robot
     * @param speed new rotation speed
     */
    void set_rspeed(qreal speed);

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
