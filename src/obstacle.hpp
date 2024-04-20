#pragma once

#include <QRectF>
#include <QGraphicsRectItem>

namespace icp {

class Obstacle : public QGraphicsRectItem {
public:
    explicit Obstacle(QRectF hitbox, QGraphicsItem *parent = nullptr);
};

} // namespace icp
