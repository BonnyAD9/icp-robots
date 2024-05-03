#pragma once

#define _USE_MATH_DEFINES

#include <cmath>
#include <fstream>

#include <QGraphicsEllipseItem>

#include "scene_obj.hpp"

namespace icp {

/**
 * @brief Represents a robot that can move and can be moved by the user.
 */
class Robot : public SceneObj, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    /**
     * @brief Creates a new robot.
     * @param position Top-left corner of the bounding square of the robot.
     * @param speed Speed of the robot in pixels per second.
     * @param parent The parent graphics item.
     */
    explicit Robot(
        QPointF position,
        QPointF step = QPointF(0, 20),
        QObject *parent = nullptr
    );

    /**
     * @brief Creates a new robot.
     * @param position Initial position of the robot.
     * @param angle Rotation of the robot.
     * @param speed Movement speed of the robot.
     * @param parent Parent object.
     */
    explicit Robot(
        QPointF position,
        qreal angle,
        qreal speed,
        QObject *parent = nullptr
    );

    explicit Robot(Robot *other);

    /**
     * @brief Moves the robot. The robot must also support 'unmove' of the one
     * last move when delta is negative.
     * @param delta Elapsed time in seconds since last tick.
     * @param distance Distance to the closest obstacle in the direction of the
     * robot.
     */
    virtual void move(qreal delta, qreal distance);

    /**
     * @brief Checks whether the robot is grabbed.
     */
    inline bool is_grabbed() const { return grabbed; }

    /**
     * @brief Gets the visual bounding box.
     * @return The bounding box of the robot.
     */
    QRectF hitbox() const;

    /**
     * @brief Sets the visual bounding box of the robot.
     * @param box The visual bounding box of the robot (size may be ignored).
     */
    void set_hitbox(QRectF hitbox);

    /**
     * @brief Gets the movement speed and orientation.
     */
    QPointF step();

    /**
     * @brief Gets the angle the robot is facing.
     */
    qreal orientation();

    /**
     * @brief Gets the unit vector of the orientation
     */
    QPointF orientation_vec();

    /**
     * @brief Gets the movement speed.
     */
    virtual qreal speed();

    /**
     * @brief Sets the angle and speed so that it matches the given step.
     */
    void set_step(QPointF step);

    /**
     * @brief Sets the orientation of the robot.
     * @param angle Angle in radans.
     */
    void set_angle(qreal angle);

    /**
     * @brief Sets the orientation of the robot.
     * @param angle Takes the mod of the vector.
     */
    void set_angle(QPointF angle);

    /**
     * @brief Gets modded angle of the robot in degrees
     * @param mod mod value
     */
    qreal get_mod_angle(unsigned mod);

    /**
     * @brief Sets the robot movement speed.
     * @param speed Speed of the robot in pixels per second.
     */
    virtual void set_speed(qreal speed);

    /**
     * @brief Saves robot to the file
     * @param file file to save robot into
     */
    virtual void save(std::ofstream &file);

signals:
    /**
     * @brief Emited when the angle of the robot changes.
     * @param angle The new angle.
     */
    void angle_change(qreal angle);

protected:
    void selection_event(bool selected) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    void hover_mouse();
    void move_by(QPointF delta);
    void move_to(QPointF pos);

    qreal angle;
    qreal mspeed;

    bool grabbed;
    QGraphicsEllipseItem *eye;
};

}
