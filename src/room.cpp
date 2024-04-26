#include "room.hpp"

#include <memory>
#include <cmath>

#include <QPointer>
#include <QTimerEvent>

namespace icp {

using namespace std;

enum class Corner : int {
    TopLeft = 0b00,
    TopRight = 0b01,
    BottomLeft = 0b10,
    BottomRight = 0b11,

    Right = 0b01,
    Bottom = 0b10,
};

constexpr Corner operator&(
    const Corner &l,
    const Corner &r
) {
    return static_cast<Corner>(
        static_cast<int>(l) & static_cast<int>(r)
    );
}

constexpr bool operator*(const Corner &l, const Corner &r) {
    return (l & r) == r;
}

/**
 * @brief How often should the robots update their position.
 */
constexpr chrono::milliseconds TICK_LEN = chrono::milliseconds(10);
/**
 * @brief How often should the robots update their position. (in seconds)
 */
constexpr qreal TICK_DELTA = TICK_LEN.count()
    * decltype(TICK_LEN)::period::num
    / static_cast<qreal>(decltype(TICK_LEN)::period::den);

bool in_range(qreal val, qreal start, qreal end) {
    return val > start && val < end;
}

bool overlaps(qreal a, qreal b, qreal x, qreal y) {
    return in_range(a, x, y)
        || in_range(b, x, y)
        || (a == x && a > b);
}

bool overlaps(QRectF a, QRectF b) {
    return overlaps(a.left(), a.right(), b.left(), b.right())
        && overlaps(a.top(), a.bottom(), b.top(), b.bottom());
}

bool in_circle(qreal radius, QPointF center, QPointF point) {
    auto dist = center - point;
    return dist.x() * dist.x() + dist.y() * dist.y() < radius;
}

bool circle_overlaps_rect(QRectF c, QRectF r) {
    // quick check on bounding boxes
    if (!overlaps(c, r)) {
        return false;
    }

    // check edge overlap
    auto center = (c.topLeft() + c.bottomRight()) / 2;
    auto edge_overlap = in_range(center.x(), r.left(), r.right())
        || in_range(center.y(), r.top(), r.bottom());
    if (edge_overlap) {
        return true;
    }

    // check corner overlap
    auto radius = c.width() / 2;
    return in_circle(radius, center, r.topLeft())
        || in_circle(radius, center, r.topRight())
        || in_circle(radius, center, r.bottomRight())
        || in_circle(radius, center, r.bottomLeft());
}

/**
 * @brief Calculates the difference from the center of crossection of circle
 * and aligned line
 *
 * Example:
 * circle:
 *   center: [cx, cy]
 *   radius: r
 * line: y = n
 *
 * auto dif = cl_cross(r, cx, n);
 * auto c1 = (x, cy - dif);
 * auto c2 = (x, cy + dif);
 *
 * @param r radius of the circle
 * @param c the other coordinate of the circle center (same to `n`)
 * @param n the known coordinate of the line.
 * @return the other coordinate to n. The value is always positive.
 */
qreal cl_cross(qreal r, qreal c, qreal n) {
    auto dif = n - c;
    return sqrt(r * r - dif * dif);
}

/**
 * @brief
 *
 * @param corner tl: 0, tr: 1, bl: 2, br: 3
 * @return QPointF
 */
QPointF corner_overlap(qreal cx, qreal cy, qreal r, QPointF p, Corner corner) {
    auto clx = cl_cross(r, cy, p.y());
    auto x = corner * Corner::Right ? p.x() - cx - clx : cx + clx - p.x();

    auto cly = cl_cross(r, cx, p.x());
    auto y = corner * Corner::Bottom ? p.y() - cy - cly : cy + cly - p.y();

    return QPointF(x, y);
}

QPointF corner_collision(QPointF c, qreal r, QPointF p, Corner corner) {
    auto o = corner_overlap(c.x(), c.y(), r, p, corner);
    if (o.x() < o.y()) {
        c.moveLeft(c.left() - o.x());
        rob->set_hitbox(c);
        return p - QPointF();
    }
    if (o.y() < o.x()) {
        c.moveTop(c.top() - o.y());
        rob->set_hitbox(c);
        return;
    }
    auto mov_vec = r.topLeft() - center;
    auto mov_len = sqrt(mov_vec.x() * mov_vec.x() + mov_vec.y() * mov_vec.y());
    mov_vec = mov_vec - mov_vec * (radius / mov_len);
    c.moveTopLeft(c.topLeft() + mov_vec);
}

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

Room::Room(QObject *parent) : QGraphicsScene(parent), obstacles() {
    setBackgroundBrush(QBrush(QColor(0x22, 0x22, 0x22)));
    startTimer(TICK_LEN, Qt::PreciseTimer);
}

void Room::add_obstacle(unique_ptr<Obstacle> obstacle) {
    Obstacle *obst = obstacle.release();
    addItem(obst);
    obstacles.push_back(obst);
}

void Room::add_robot(unique_ptr<Robot> robot) {
    Robot *rob = robot.release();
    addItem(rob);
    robots.push_back(rob);
}

//---------------------------------------------------------------------------//
//                                PROTECTED                                  //
//---------------------------------------------------------------------------//

void Room::timerEvent(QTimerEvent *event) {
    tick(TICK_DELTA);
}

//---------------------------------------------------------------------------//
//                                 PRIVATE                                   //
//---------------------------------------------------------------------------//

void Room::tick(qreal delta) {
    move_robots(delta);

    for (auto r : robots) {
        if (!r->is_grabbed()) {
            border_collision(r);
        }
    }
}

void Room::move_robots(qreal delta) {
    for (auto r : robots) {
        if (!r->is_grabbed()) {
            r->move(delta);
        }
    }
}

void Room::border_collision(Robot *rob) {
    auto box = rob->hitbox();

    if (box.left() < 0) {
        box.moveLeft(0);
    } else if (box.right() > width()) {
        box.moveRight(width());
    }

    if (box.top() < 0) {
        box.moveTop(0);
    } else if (box.bottom() > height()) {
        box.moveBottom(height());
    }

    rob->set_hitbox(box);
}

void Room::obstacle_collision(Robot *rob, Obstacle *obs) {
    // circle (robot)
    auto c = rob->hitbox();
    // rectangle (obstacle)
    auto r = obs->hitbox();

    // check edge overlap
    auto center = (c.topLeft() + c.bottomRight()) / 2;
    auto cx = center.x();
    auto cy = center.y();
    // horizontal edge
    if (in_range(cx, r.left(), r.right())) {
        // top edge of obstacle
        if (in_range(c.bottom(), r.top(), r.bottom())) {
            c.moveBottom(r.top());
            rob->set_hitbox(c);
            return;
        }
        // bottom edge of obstacle
        if (in_range(c.top(), r.top(), r.bottom())) {
            c.moveTop(r.bottom());
            rob->set_hitbox(c);
            return;
        }
        // no overlap
        return;
    }
    // vertical edge
    if (in_range(cy, r.top(), r.bottom())) {
        // left edge of obstacle
        if (in_range(c.right(), r.left(), r.right())) {
            c.moveRight(r.left());
            rob->set_hitbox(c);
            return;
        }
        // right edge of obstacle
        if (in_range(c.left(), r.left(), r.right())) {
            c.moveLeft(r.right());
            rob->set_hitbox(c);
            return;
        }
        // no overlap
        return;
    }

    // check corner overlap
    auto radius = c.width() / 2;
    // top left corner of obstacle
    if (in_circle(radius, center, r.topLeft())) {
        auto o = corner_overlap(cx, cy, radius, r.topLeft(), Corner::TopLeft);
        if (o.x() < o.y()) {
            c.moveLeft(c.left() - o.x());
            rob->set_hitbox(c);
            return;
        }
        if (o.y() < o.x()) {
            c.moveTop(c.top() - o.y());
            rob->set_hitbox(c);
            return;
        }
        auto mov_vec = r.topLeft() - center;
        auto mov_len = sqrt(mov_vec.x() * mov_vec.x() + mov_vec.y() * mov_vec.y());
        mov_vec = mov_vec - mov_vec * (radius / mov_len);
        c.moveTopLeft(c.topLeft() + mov_vec);
    }
}

} // namespace icp

