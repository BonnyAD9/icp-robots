#include "room.hpp"

#include <memory>

#include <QPointer>

namespace icp {

using namespace std;

Room::Room(QObject *parent) : QGraphicsScene(parent), obstacles() {
    setBackgroundBrush(QBrush(QColor(0x22, 0x22, 0x22)));
}

void Room::add_obstacle(unique_ptr<Obstacle> obstacle) {
    Obstacle *obst = obstacle.release();
    addItem(obst);
    obstacles.push_back(obst);
}

} // namespace icp

