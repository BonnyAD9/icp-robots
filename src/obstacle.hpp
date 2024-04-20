#pragma once

#include <QRectF>
#include <QGraphicsRectItem>

namespace icp {

class Obstacle : public QGraphicsRectItem {
public:
    explicit Obstacle(QRectF hitbox, QGraphicsItem *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool dragging;
};

} // namespace icp
