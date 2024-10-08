/**
 * @file
 * @authors Martin Slezák (xsleza26), Jakub Antonín Štigler (xstigl00)
 * @brief Robot with basic AI (header file)
 */

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
     * @brief Creates new robot with basic AI.
     * @param position Initial position of the robot.
     * @param step The speed and initial orientation of the robot.
     * @param elide_dist How far from obstacle the robot stops and starts to
     * rotate.
     * @param elide_rot How much the robot rotates when it detects obstacle.
     * @param rot_speed How fast the robot rotates (angle per second).
     * @param parent The parent object.
     */
    explicit AutoRobot(
        QPointF position,
        QPointF step = QPoint(0, 20),
        qreal elide_dist = 20,
        qreal elide_rot = M_PI / M_E,
        qreal rot_speed = M_PI / 4,
        QObject *parent = nullptr
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
        QPointF position,
        qreal angle,
        qreal speed,
        qreal elide_dist = 20,
        qreal elide_rot = M_PI / M_E,
        qreal rot_speed = M_PI / 4, // angle per secod
        QObject *parent = nullptr
    );

    /**
     * @brief Creates auto robot with the same parameters as the given robot.
     */
    explicit AutoRobot(Robot *r);

    /**
     * @brief Moves the robot.
     * @param delta How much time (in second) passed since the last tick.
     * @param distance Distance to the closest obstacle in front of the robot.
     */
    void move(qreal delta, qreal distance) override;

    /**
     * @brief Gets the speed of the robot. (pixels per second)
     */
    virtual qreal speed() override;

    /**
     * @brief Sets the speed of the robot. (pixels per second)
     */
    virtual void set_speed(qreal speed) override;

    /**
     * @brief Saves robot to the file
     * @param file file to save robot into
     */
    virtual void save(std::ofstream &file) override;

    /**
     * @brief Gets the elide distance of the robot. (pixels)
     */
    qreal edist() const;

    /**
     * @brief Sets the elide distance of the robot. (pixels)
     */
    void set_edist(qreal dist);

    /**
     * @brief Gets the rotation speed of the robot. (radians per second)
     */
    qreal rspeed() const;

    /**
     * @brief Sets the rotation speed of the robot. (radans per second)
     */
    void set_rspeed(qreal speed);

    /**
     * @brief Gets the rotation distance to avoid collision. (radians)
     */
    qreal rdist() const;

    /**
     * @brief Sets the rotation distance to avoid collision. (radians)
     */
    void set_rdist(qreal dist);

private:
    qreal sspeed;
    qreal rot_remain;

    qreal elide_dist;
    qreal elide_rot;
    qreal rot_speed;
};

} // namespace icp
