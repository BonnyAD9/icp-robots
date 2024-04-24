#pragma once

#include <QGraphicsEllipseItem>

namespace icp {

/**
 * @brief Represents a robot that can move and can be moved by the user.
 */
class Robot : public QGraphicsEllipseItem {
public:
    /**
     * @brief Creates a new robot.
     * @param position Top-left corner of the bounding square of the robot.
     * @param speed Speed of the robot in pixels per second.
     * @param parent The parent graphics item.
     */
    explicit Robot(
        QPoint position,
        QPointF speed = QPointF(0, 20), // pixels per second
        QGraphicsItem *parent = nullptr
    );

    /**
     * @brief Moves the robot. The robot must also support 'unmove' of the one
     * last move when delta is negative.
     * @param delta Elapsed time in seconds since last tick.
     */
    void move(qreal delta);
    /**
     * @brief Checks whether the robot is grabbed.
     */
    inline bool is_grabbed() const { return grabbed; }
    /**
     * @brief Gets the visual bounding box.
     * @return The bounding box of the robot.
     */
    QRectF hitbox();
    /**
     * @brief Sets the visual bounding box of the robot.
     * @param box The visual bounding box of the robot (size may be ignored).
     */
    void set_hitbox(QRectF hitbox);

protected:
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

    bool grabbed;
    QPointF speed;
};

}
