#pragma once

#include <QGraphicsEllipseItem>

namespace icp {

class Robot : public QGraphicsEllipseItem {
public:
    explicit Robot(QPoint center, QGraphicsItem *parent = nullptr);
};

}
