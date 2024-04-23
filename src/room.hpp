#pragma once

#include <vector>
#include <memory>

#include <QGraphicsScene>

#include "obstacle.hpp"
#include "robot.hpp"

namespace icp {

class Room : public QGraphicsScene {
public:
    Room(QObject *parent = nullptr);

    void add_obstacle(std::unique_ptr<Obstacle> obstacle);
    void add_robot(std::unique_ptr<Robot> robot);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    void tick(qreal delta);

    std::vector<Obstacle *> obstacles;
    std::vector<Robot *> robots;
};

} // namespace icp
