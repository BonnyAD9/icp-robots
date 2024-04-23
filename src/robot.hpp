#pragma once

#include <QGraphicsEllipseItem>

namespace icp {

class Robot : public QGraphicsEllipseItem {
public:
    explicit Robot(
        QPoint center,
        QPointF speed = QPointF(0, 1),
        QGraphicsItem *parent = nullptr
    );

    void move(qreal delta);

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

    bool grabbed;
    QPointF speed;
};

}
