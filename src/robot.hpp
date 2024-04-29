#pragma once

#define _USE_MATH_DEFINES

#include <cmath>

#include <QGraphicsEllipseItem>

namespace icp {

/**
 * @brief Represents a robot that can move and can be moved by the user.
 */
class Robot : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT

public:
    /**
     * @brief Creates a new robot.
     * @param position Top-left corner of the bounding square of the robot.
     * @param speed Speed of the robot in pixels per second.
     * @param parent The parent graphics item.
     */
    explicit Robot(
        QPoint position,
        QPointF step = QPointF(0, 20),
        QGraphicsItem *parent = nullptr
    );

    /**
     * @brief Creates a new robot.
     * @param position Initial position of the robot.
     * @param angle Rotation of the robot.
     * @param speed Movement speed of the robot.
     * @param parent Parent object.
     */
    explicit Robot(
        QPoint position,
        qreal angle,
        qreal speed,
        QGraphicsItem *parent = nullptr
    );

    /**
     * @brief Moves the robot. The robot must also support 'unmove' of the one
     * last move when delta is negative.
     * @param delta Elapsed time in seconds since last tick.
     * @param distance Distance to the closest obstacle in the direction of the
     * robot.
     */
    virtual void move(qreal delta, qreal distance);
    /**
     * @brief Gets the position change from the last move.
     */
    QPointF last_move();

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
    qreal speed();

    void set_selected(bool val = true);

    void unselect();

signals:
    void select(Robot *sender);

protected:
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
     * @brief Sets the robot movement speed.
     * @param speed Speed of the robot in pixels per second.
     */
    void set_speed(qreal speed);

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
    QPointF last_move_vec;
    QGraphicsEllipseItem *eye;
};

}
