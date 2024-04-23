#pragma once

#include <QGraphicsEllipseItem>

namespace icp {

class Robot : public QGraphicsEllipseItem {
public:
    explicit Robot(
        QPoint center,
        QPointF speed = QPointF(0, 20), // pixels per second
        QGraphicsItem *parent = nullptr
    );

    void move(qreal delta);
    inline bool is_grabbed() const { return grabbed; }

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
