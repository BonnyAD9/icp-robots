#include "room.hpp"

#include <memory>
#include <cmath>

#include <QPointer>
#include <QTimerEvent>

namespace icp {

using namespace std;

/**
 * @brief Small value used for floating point error tolerance
 */
constexpr qreal FP_ERROR = 0.00001;

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
    return dist.x() * dist.x() + dist.y() * dist.y() < radius * radius - FP_ERROR;
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

bool is_inside(QRectF inner, QRectF outer) {
    return inner.left() > outer.left()
        && inner.right() < outer.right()
        && inner.top() > outer.top()
        && inner.bottom() < outer.bottom();
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
    auto x = corner * Corner::Right ? cx - clx - p.x() : cx + clx - p.x();

    auto cly = cl_cross(r, cx, p.x());
    auto y = corner * Corner::Bottom ? cy - cly - p.y() : cy + cly - p.y();

    return QPointF(x, y);
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

    for (auto o : obstacles) {
        if (o->is_grabbed()) {
            continue;
        }
        for (auto r : robots) {
            if (!r->is_grabbed()) {
                obstacle_collision(r, o);
            }
        }
    }

    auto end = robots.end();
    for (auto r1 = robots.begin(); r1 != end; ++r1) {
        if ((*r1)->is_grabbed()) {
            continue;
        }
        for (auto r2 = r1 + 1; r2 != end; ++r2) {
            if (!(*r2)->is_grabbed()) {
                robot_collision(*r1, *r2);
            }
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
    } else if (in_range(cy, r.top(), r.bottom())) {
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
    auto m = QPointF(0, 0);
    if (in_circle(radius, center, r.topLeft())) {
        corner_collision(rob, r.topLeft(), Corner::TopLeft);
        return;
    }
    if (in_circle(radius, center, r.topRight())) {
        corner_collision(rob, r.topRight(), Corner::TopRight);
        return;
    }
    if (in_circle(radius, center, r.bottomRight())) {
        corner_collision(rob, r.bottomRight(), Corner::BottomRight);
        return;
    }
    if (in_circle(radius, center, r.bottomLeft())) {
        corner_collision(rob, r.bottomLeft(), Corner::BottomLeft);
        return;
    }
    return;
}

void Room::robot_collision(Robot *r1, Robot *r2) {
    auto c1 = r1->hitbox();
    auto c2 = r2->hitbox();
    auto dir = c2.topLeft() - c1.topLeft();
    auto cw = (c1.width() + c2.width()) / 2;
    auto dir_len = sqrt(dir.x() * dir.x() + dir.y() * dir.y());
    auto over = cw - dir_len;

    if (over <= FP_ERROR) {
        // No collision
        return;
    }

    dir = dir * (over / 2 / dir_len);
    c1.moveTopLeft(c1.topLeft() - dir);
    c2.moveTopLeft(c2.topLeft() + dir);
    r1->set_hitbox(c1);
    r2->set_hitbox(c2);
}

void Room::corner_collision(Robot *rob, QPointF p, Corner corner) {
    constexpr qreal M = 1.5;

    auto box = rob->hitbox();
    auto c = (box.topLeft() + box.bottomRight()) / 2;
    auto r = box.width() / 2;
    auto m = rob->last_move();

    auto o = corner_overlap(c.x(), c.y(), r, p, corner);
    auto ao = QPointF(qAbs(o.x()), qAbs(o.y()));
    auto t = pow(ao.x() + ao.y(), M);
    auto oc = QPointF(o.x() * pow(ao.x(), M), o.y() * pow(ao.y(), M)) / t;
    oc = QPointF(o.x() - oc.x(), o.y() - oc.y());

    m = QPointF(qAbs(m.x()), qAbs(m.y()));
    if (m.x() < m.y()) {
        o = corner_overlap(c.x(), c.y() - oc.y(), r, p, corner);
        box.moveTop(box.top() - oc.y());
        rob->set_hitbox(box);
        not_collides_or(rob, QPointF(-o.x(), 0), QPointF(0, -o.y()));
        return;
    }
    if (m.y() < m.x()) {
        o = corner_overlap(c.x() - oc.x(), c.y(), r, p, corner);
        box.moveLeft(box.left() - oc.x());
        rob->set_hitbox(box);
        not_collides_or(rob, QPointF(0, -o.y()), QPointF(-o.x(), 0));
        return;
    }
    ao = QPointF(qAbs(o.x()), qAbs(o.y()));
    if (ao.x() < ao.y()) {
        o = corner_overlap(c.x(), c.y() - oc.y(), r, p, corner);
        box.moveTop(box.top() - oc.y());
        rob->set_hitbox(box);
        not_collides_or(rob, QPointF(-o.x(), 0), QPointF(0, -o.y()));
        return;
    }
    if (ao.y() < ao.x()) {
        o = corner_overlap(c.x() - oc.x(), c.y(), r, p, corner);
        box.moveLeft(box.left() - oc.x());
        rob->set_hitbox(box);
        not_collides_or(rob, QPointF(0, -o.y()), QPointF(-o.x(), 0));
        return;
    }
    auto mv = p - c;
    auto ml = sqrt(mv.x() * mv.x() + mv.y() * mv.y());
    mv = mv - mv * (r / ml);

    box.moveTopLeft(box.topLeft() + mv);
    rob->set_hitbox(box);
}

void Room::not_collides_or(Robot *rob, QPointF a, QPointF b) {
    auto box = rob->hitbox();
    box.moveTopLeft(box.topLeft() + a);
    rob->set_hitbox(box);

    if (hard_collision(rob)) {
        box.moveTopLeft(box.topLeft() - a + b);
        rob->set_hitbox(box);
    }
}

bool Room::hard_collision(Robot *rob) {
    if (!is_inside(rob->hitbox(), QRectF(0, 0, width(), height()))) {
        return true;
    }

    for (auto o : obstacles) {
        if (o->is_grabbed()) {
            continue;
        }
        if (circle_overlaps_rect(rob->hitbox(), o->hitbox())) {
            return true;
        }
    }
    return false;
}

} // namespace icp

