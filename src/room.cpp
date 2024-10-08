/**
 * @file
 * @authors Martin Slezák (xsleza26), Jakub Antonín Štigler (xstigl00)
 * @brief Room with robots and obstacles that can move in here. (source file)
 */

#include "room.hpp"

#include <memory>
#include <cmath>
#include <algorithm>
#include <iostream>

#include <QPointer>
#include <QTimerEvent>
#include <QKeyEvent>
#include <QMessageBox>

#include "auto_robot.hpp"
#include "control_robot.hpp"

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

/**
 * @brief Checks if value is in range.
 * @param val Value to check.
 * @param start Smaller value in the range.
 * @param end Larger value in the range.
 * @return true if value is in the range, otherwise false.
 */
bool in_range(qreal val, qreal start, qreal end) {
    return val > start && val < end;
}

/**
 * @brief Checks if point is in circle.
 * @param radius Radius of the circle.
 * @param center Center of the circle.
 * @param point Point to check.
 * @return true if point is in circle, otherwise false.
 */
bool in_circle(qreal radius, QPointF center, QPointF point) {
    auto dist = center - point;
    return dist.x() * dist.x() + dist.y() * dist.y() < radius * radius;
}

/**
 * @brief Calculates the discriminant of a 2x2 matrix.
 * @param a First row.
 * @param b Second row.
 */
qreal cross(QPointF a, QPointF b) {
    return a.x() * b.y() - a.y() * b.x();
}

/**
 * @brief Calculates a point where two lines intersect.
 * @param p1 Point trough which the first line passes.
 * @param d1 Direction in which the first line goes.
 * @param p2 Point trough which the second line passes.
 * @param d2 Direction in which the second line goes.
 * @return The intersection point.
 */
QPointF line_intersection(QPointF p1, QPointF d1, QPointF p2, QPointF d2) {
    auto u = cross(p2 - p1, d1) / cross(d1, d2);
    return p2 + u * d2;
}

/**
 * @brief Calculates the distance of a segment from a given point in the given
 * direction.
 * @param p Point from which the segment distance is calculated.
 * @param d Direction of the 'ray'.
 * @param a First point of the segment (x or y must be smaller than b).
 * @param b Second point of the segment (x or y must be larger than b).
 * @return Distance of the segment from the point. INFINITY when the 'ray'
 * doesn't touch the segment.
 */
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

/**
 * @brief Calculates the distance of a rectangle from a point in the given
 * direction.
 * @param p Point from which to calculate the distance.
 * @param d Direction from the point ('ray').
 * @param r Rectangle to calculate the distance from.
 * @return Distance from the rectangle. INFINITY if the 'ray' doesn't touch the
 * rectangle.
 */
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

Room::Room(QObject *parent) :
    QGraphicsScene(parent),
    obstacles(),
    timer(0),
    selected(nullptr)
{
    setBackgroundBrush(QBrush(QColor(0x22, 0x22, 0x22)));
    timer = startTimer(TICK_LEN, Qt::PreciseTimer);
}

void Room::add_obstacle(unique_ptr<Obstacle> obstacle) {
    Obstacle *obst = obstacle.release();
    addItem(obst);
    obstacles.push_back(obst);
    connect(
        obst,
        &Obstacle::select,
        this,
        &Room::select_obj
    );
}

void Room::add_robot(unique_ptr<Robot> robot) {
    Robot *rob = robot.release();
    addItem(rob);
    robots.push_back(rob);
    connect(
        rob,
        &Robot::select,
        this,
        &Room::select_obj
    );
}

//---------------------------------------------------------------------------//
//                               PUBLIC SLOTS                                //
//---------------------------------------------------------------------------//
void Room::run_simulation(bool play) {
    if (play) {
        if (!timer) {
            timer = startTimer(TICK_LEN, Qt::PreciseTimer);
        }
    } else {
        killTimer(timer);
        timer = 0;
    }
}

void Room::remove_obj(SceneObj *o) {
    auto obj = unique_ptr<SceneObj>(o);
    if (o == selected) {
        select_obj(NULL);
    }

    auto rob = dynamic_cast<Robot *>(o);
    if (rob) {
        auto p = find(robots.begin(), robots.end(), rob);
        if (p == robots.end()) {
            return;
        }

        removeItem(rob);

        swap(*p, *robots.rbegin());
        robots.pop_back();
    }

    auto obs = dynamic_cast<Obstacle *>(o);
    if (obs) {
        auto p = find(obstacles.begin(), obstacles.end(), obs);
        if (p == obstacles.end()) {
            return;
        }

        removeItem(obs);

        swap(*p, *obstacles.rbegin());
        obstacles.pop_back();
    }
}

void Room::change_robot(Robot *old, Robot *replace) {
    auto rep = unique_ptr<Robot>(replace);
    auto sel = selected;

    remove_obj(old);
    add_robot(std::move(rep));

    if (old == sel) {
        replace->set_selected(true);
    }
}

void Room::add_obstacle_slot(Obstacle *obstacle) {
    add_obstacle(unique_ptr<Obstacle>(obstacle));
}

void Room::add_robot_slot(Robot *robot) {
    add_robot(unique_ptr<Robot>(robot));
}

void Room::save(string filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        QMessageBox::critical(
            nullptr,
            "Error saving room", "File cannot be accessed"
        );
        return;
    }

    file << "room: " << width() << "x" << height() << endl;
    for (auto obst : obstacles) {
        file << "obstacle: " << obst->hitbox().width() << "x"
            << obst->hitbox().height() << " [" << obst->hitbox().x() << ", "
            << obst->hitbox().y() << "]" << endl;
    }

    for (auto rob : robots) {
        rob->save(file);
    }
    file.close();

    QMessageBox::information(
        nullptr,
        "Success",
        "The room was successfully saved"
    );
}

//---------------------------------------------------------------------------//
//                                PROTECTED                                  //
//---------------------------------------------------------------------------//

void Room::timerEvent(QTimerEvent *event) {
    tick(TICK_DELTA);
}

void Room::keyPressEvent(QKeyEvent *event) {
    if (selected && event->key() == Qt::Key_Delete) {
        remove_obj(selected);
        return;
    }

    auto robot = dynamic_cast<ControlRobot *>(selected);
    if (!robot || timer == 0)
        return;

    switch (event->key()) {
        case Qt::Key_Left:
            robot->left();
            break;
        case Qt::Key_Right:
            robot->right();
            break;
        case Qt::Key_Up:
            robot->forward();
            break;
    }
}

void Room::keyReleaseEvent(QKeyEvent *event) {
    auto robot = dynamic_cast<ControlRobot *>(selected);
    if (!robot || timer == 0)
        return;

    switch (event->key()) {
        case Qt::Key_Left:
            robot->left(false);
            break;
        case Qt::Key_Right:
            robot->right(false);
            break;
        case Qt::Key_Up:
            robot->forward(false);
            break;
    }
}

//---------------------------------------------------------------------------//
//                              PRIVATE SLOTS                                //
//---------------------------------------------------------------------------//

void Room::select_obj(SceneObj *o) {
    if (selected && selected != o) {
        QSignalBlocker block(selected);
        selected->set_selected(false);
    }

    selected = o;
    emit new_selection(selected);
}

//---------------------------------------------------------------------------//
//                                 PRIVATE                                   //
//---------------------------------------------------------------------------//

void Room::tick(qreal delta) {
    move_robots(delta);

    // collisions of robots with the border of the room
    for (auto r : robots) {
        if (!r->is_grabbed()) {
            border_collision(r);
        }
    }

    // collisions of robots with obstacles
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

    // collisions of robots with each other
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

    dir = dir_len == 0 ? QPointF(0, 0) : dir * (over / (2 * dir_len));
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

