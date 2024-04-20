#include "obstacle.hpp"

#include <iostream>

#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

namespace icp {

Obstacle::State operator|=(Obstacle::State &l, const Obstacle::State &r) {
    return l = static_cast<Obstacle::State>(
        static_cast<int>(l) | static_cast<int>(r)
    );
}

Obstacle::State operator^=(Obstacle::State &l, const Obstacle::State &r) {
    return l = static_cast<Obstacle::State>(
        static_cast<int>(l) ^ static_cast<int>(r)
    );
}

Obstacle::State operator&(const Obstacle::State &l, const Obstacle::State &r) {
    return static_cast<Obstacle::State>(
        static_cast<int>(l) & static_cast<int>(r)
    );
}

Obstacle::State operator|(const Obstacle::State &l, const Obstacle::State &r) {
    return static_cast<Obstacle::State>(
        static_cast<int>(l) | static_cast<int>(r)
    );
}

bool operator*(const Obstacle::State &l, const Obstacle::State &r) {
    return (l & r) == r;
}

Obstacle::State operator~(const Obstacle::State &s) {
    return static_cast<Obstacle::State>(~static_cast<int>(s));
}

Obstacle::Obstacle(QRectF hitbox, QGraphicsItem *parent)
    : QGraphicsRectItem(hitbox, parent)
{
    setBrush(QBrush(QColor(0xff, 0x55, 0x55)));
    setPen(QPen(
        QColor(0xff, 0xff, 0xff),
        5,
        Qt::SolidLine,
        Qt::SquareCap,
        Qt::PenJoinStyle::SvgMiterJoin
    ));
    setFlag(ItemIsMovable);
}

void Obstacle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    constexpr qreal R_BORDER = 2.5;

    state = State::None;

    auto relPos = event->scenePos() - rect().topLeft();

    if (relPos.x() <= R_BORDER) {
        state |= State::ResizeHorizontal | State::ResizeLeft;
    } else if (rect().width() - relPos.x() <= R_BORDER) {
        state |= State::ResizeHorizontal;
    }

    if (relPos.y() <= R_BORDER) {
        state |= State::ResizeVertical | State::ResizeTop;
    } else if (rect().height() - relPos.y() <= R_BORDER) {
        state |= State::ResizeVertical;
    }

    if (state == State::None) {
        state = State::Dragging;
    }
    grabMouse();
}

void Obstacle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    state = State::None;
    ungrabMouse();
}

void Obstacle::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto delta = event->scenePos() - event->lastScenePos();
    QRectF rec = rect();

    if (state == State::Dragging) {
        rec.moveTopLeft(rec.topLeft() + delta);
    }

    if (state * State::ResizeHorizontal) {
        if (state * State::ResizeLeft) {
            rec.setLeft(rec.left() + delta.x());
        } else {
            rec.setRight(rec.right() + delta.x());
        }
        if (rec.width() < 0) {
            state ^= State::ResizeLeft;
        }
    }

    if (state * State::ResizeVertical) {
        if (state * State::ResizeTop) {
            rec.setTop(rec.top() + delta.y());
        } else {
            rec.setBottom(rec.bottom() + delta.y());
        }
        if (rec.height() < 0) {
            state ^= State::ResizeTop;
        }
    }

    setRect(rec.normalized());
}

}
