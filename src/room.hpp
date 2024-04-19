#pragma once

#include <vector>
#include <memory>

#include <QGraphicsScene>

#include "obstacle.hpp"

namespace icp {

class Room : public QGraphicsScene {
public:
    Room(QObject *parent = nullptr);

    void add_obstacle(std::unique_ptr<Obstacle> obstacle);

private:
    std::vector<Obstacle *> obstacles;
};

} // namespace icp
