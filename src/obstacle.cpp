#include "obstacle.hpp"

#include <iostream>

#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>

namespace icp {

constexpr qreal BORDER_THICKNESS = 6;

constexpr Obstacle::State operator|(
    const Obstacle::State &l,
    const Obstacle::State &r
) {
    return static_cast<Obstacle::State>(
        static_cast<int>(l) | static_cast<int>(r)
    );
}

constexpr Obstacle::State operator|=(
    Obstacle::State &l,
    const Obstacle::State &r
) {
    return l = l | r;
}

constexpr Obstacle::State operator^=(
    Obstacle::State &l,
    const Obstacle::State &r
) {
    return l = static_cast<Obstacle::State>(
        static_cast<int>(l) ^ static_cast<int>(r)
    );
}

constexpr Obstacle::State operator&(
    const Obstacle::State &l,
    const Obstacle::State &r
) {
    return static_cast<Obstacle::State>(
        static_cast<int>(l) & static_cast<int>(r)
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
        BORDER_THICKNESS,
        Qt::SolidLine,
        Qt::SquareCap,
        Qt::PenJoinStyle::SvgMiterJoin
    ));
    setAcceptHoverEvents(true);
}

QRectF Obstacle::hitbox() const {
    constexpr qreal ADJ = BORDER_THICKNESS / 2;
    return rect().adjusted(-ADJ, -ADJ, ADJ, ADJ);
}

void Obstacle::set_hitbox(QRectF hitbox) {
    constexpr qreal ADJ = BORDER_THICKNESS / 2;
    setRect(rect().adjusted(ADJ, ADJ, -ADJ, -ADJ));
}

bool Obstacle::is_grabbed() const {
    return state == State::Dragging;
}

void Obstacle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        if (state == State::None) {
            setCursor(Qt::ClosedHandCursor);
            state = State::Dragging;
        }
        grabMouse();
    }
}

void Obstacle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        state = State::None;
        ungrabMouse();
    }
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

void Obstacle::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setResizeCursor(event);
}

void Obstacle::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    setResizeCursor(event);
}

void Obstacle::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    unsetCursor();
    QGraphicsRectItem::hoverLeaveEvent(event);
}

void Obstacle::setResizeCursor(QGraphicsSceneHoverEvent *event) {
    constexpr qreal R_BORDER = BORDER_THICKNESS / 2;

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

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch"
    switch (state) {
        case State::ResizeHorizontal
            | State::ResizeVertical
            | State::ResizeLeft
            | State::ResizeTop:
        case State::ResizeHorizontal | State::ResizeVertical:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case State::ResizeHorizontal
            | State::ResizeVertical
            | State::ResizeTop:
        case State::ResizeHorizontal
            | State::ResizeVertical
            | State::ResizeLeft:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case State::ResizeHorizontal:
        case State::ResizeHorizontal | State::ResizeLeft:
            setCursor(Qt::SizeHorCursor);
            break;
        case State::ResizeVertical:
        case State::ResizeVertical | State::ResizeTop:
            setCursor(Qt::SizeVerCursor);
            break;
        default:
            setCursor(Qt::OpenHandCursor);
    }
    #pragma GCC diagnostic pop

    QGraphicsRectItem::hoverEnterEvent(event);
}

}
