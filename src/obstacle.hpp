/**
 * @file
 * @authors Martin Slezák (xsleza26), Jakub Antonín Štigler (xstigl00)
 * @brief Obstacle that can be placed into a room. (header file)
 */

#pragma once

#include <QRectF>
#include <QGraphicsRectItem>
#include <QCursor>
#include <fstream>

#include "scene_obj.hpp"

namespace icp {

/**
 * @brief Class representing obstacle
 */
class Obstacle : public SceneObj, public QGraphicsRectItem {
public:
    /**
     * @brief Creates new obstacle
     * @param hitbox Hitbox of the obstacle
     * @param parent Parent object
     */
    explicit Obstacle(QRectF hitbox, QGraphicsItem *parent = nullptr);

    /**
     * @brief Enum representing state of the obstacle
     */
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
    void selection_event(bool selected) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    void setResizeCursor(QGraphicsSceneHoverEvent *event);

    State state;
};

} // namespace icp
