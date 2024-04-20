#pragma once

#include <QRectF>
#include <QGraphicsRectItem>

namespace icp {

class Obstacle : public QGraphicsRectItem {
public:
    explicit Obstacle(QRectF hitbox, QGraphicsItem *parent = nullptr);

    enum class State : int {
        None = 0x0,
        Dragging = 0x1,
        ResizeHorizontal = 0x2,
        ResizeVertical = 0x4,
        ResizeLeft = 0x8,
        ResizeTop = 0x10,
    };

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    State state;
};

} // namespace icp
