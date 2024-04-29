#pragma once

#include <QRectF>
#include <QGraphicsRectItem>
#include <QCursor>

namespace icp {

class Obstacle : public QGraphicsRectItem {
public:
    explicit Obstacle(QRectF hitbox, QGraphicsItem *parent = nullptr);

    void start_drag();

    enum class State : int {
        None = 0x0,
        Dragging = 0x1,
        ResizeHorizontal = 0x2,
        ResizeVertical = 0x4,
        ResizeLeft = 0x8,
        ResizeTop = 0x10,
    };

    /**
     * @brief Gets the hitbox of the rectangle.
     * @return Hitbox of the rectangle.
     */
    QRectF hitbox() const;
    /**
     * @brief Sets the hitbox of the rectangle.
     * @param hitbox the new hitbox.
     */
    void set_hitbox(QRectF hitbox);
    /**
     * @brief Checks whether the obstacle is grabbed by the user.
     * @return `true` if it is grabbed, otherwise `false`.
     */
    bool is_grabbed() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    State state;

    void setResizeCursor(QGraphicsSceneHoverEvent *event);
};

} // namespace icp
