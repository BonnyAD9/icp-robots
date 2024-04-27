#include "room.hpp"

#include <memory>
#include <cmath>
#include <algorithm>

#include <QPointer>
#include <QTimerEvent>

namespace icp {

using namespace std;

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

bool in_circle(qreal radius, QPointF center, QPointF point) {
    auto dist = center - point;
    return dist.x() * dist.x() + dist.y() * dist.y() < radius * radius;
}

QPointF line_intersection(QPointF p1, QPointF d1, QPointF p2, QPointF d2) {
    auto x = p1.x() + d1.x() * (p1.x() - p2.x()) / (d2.x() - d1.x());
    auto y = p1.y() + d1.y() * (p1.y() - p2.y()) / (d2.y() - d1.y());
    return QPointF(x, y);
}

qreal segment_distance(QPointF p, QPointF d, QPointF a, QPointF b) {
    auto is = line_intersection(p, d, a, a - b);
    if (isnan(is.x())
        || isnan(is.y())
        || (!in_range(is.x(), a.x(), b.x()) && !in_range(is.y(), a.y(), b.y()))
    ) {
        return Q_INFINITY;
    }
    auto v = is - p;
    if (QPointF::dotProduct(d, v) < 0) {
        return Q_INFINITY;
    }

    return sqrt(v.x() * v.x() + v.y() * v.y());
}

qreal rect_distance(QPointF p, QPointF d, QRectF r) {
    return min({
        segment_distance(p, d, r.topLeft(), r.topRight()),
        segment_distance(p, d, r.topRight(), r.bottomRight()),
        segment_distance(p, d, r.bottomLeft(), r.bottomRight()),
        segment_distance(p, d, r.topLeft(), r.bottomLeft())
    });
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
            r->move(delta, obstacle_distance(r));
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
        corner_collision(rob, r.topLeft());
        return;
    }
    if (in_circle(radius, center, r.topRight())) {
        corner_collision(rob, r.topRight());
        return;
    }
    if (in_circle(radius, center, r.bottomRight())) {
        corner_collision(rob, r.bottomRight());
        return;
    }
    if (in_circle(radius, center, r.bottomLeft())) {
        corner_collision(rob, r.bottomLeft());
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

    if (over <= 0) {
        // No collision
        return;
    }

    dir = dir * (over / (2 * dir_len));
    c1.moveTopLeft(c1.topLeft() - dir);
    c2.moveTopLeft(c2.topLeft() + dir);
    r1->set_hitbox(c1);
    r2->set_hitbox(c2);
}

void Room::corner_collision(Robot *rob, QPointF p) {
    auto box = rob->hitbox();
    auto c = (box.topLeft() + box.bottomRight()) / 2;
    auto r = box.width() / 2;

    auto mv = p - c;
    auto ml = sqrt(mv.x() * mv.x() + mv.y() * mv.y());
    mv = mv - mv * (r / ml);

    box.moveTopLeft(box.topLeft() + mv);
    rob->set_hitbox(box);
}

qreal Room::obstacle_distance(Robot *rob) {
    auto r = rob->hitbox();
    auto c = (r.topLeft() + r.bottomRight()) / 2;
    auto d = rob->orientation_vec();

    qreal res = rect_distance(c, d, QRectF(0, 0, width(), height()));

    for (auto o : obstacles) {
        if (!o->is_grabbed()) {
            res = min(res, rect_distance(c, d, o->hitbox()));
        }
    }

    return clamp(res - r.width() / 2, 0., Q_INFINITY);
}

} // namespace icp

