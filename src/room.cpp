#include "room.hpp"

#include <memory>

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
    for (auto r : robots) {
        if (!r->is_grabbed()) {
            r->move(delta);
        }
    }
}

} // namespace icp

