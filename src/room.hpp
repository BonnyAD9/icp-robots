#pragma once

#include <vector>
#include <memory>

#include <QGraphicsScene>

#include "obstacle.hpp"
#include "robot.hpp"

namespace icp {

/**
 * @brief Room that can contain robots and obstacles.
 */
class Room : public QGraphicsScene {
public:
    /**
     * @brief Creates a new room.
     * @param parent The Qt object.
     */
    Room(QObject *parent = nullptr);

    /**
     * @brief Adds obstacle to the room.
     * @param obstacle Obstacle to add to the room.
     */
    void add_obstacle(std::unique_ptr<Obstacle> obstacle);
    /**
     * @brief Adds robot to the room.
     * @param robot Robot to add to the room.
     */
    void add_robot(std::unique_ptr<Robot> robot);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    void tick(qreal delta);
    void move_robots(qreal delta);
    void border_collision(Robot *rob);
    void obstacle_collision(Robot *rob, Obstacle *obs);
    void robot_collision(Robot *r1, Robot *r2);

    std::vector<Obstacle *> obstacles;
    std::vector<Robot *> robots;
};

} // namespace icp
