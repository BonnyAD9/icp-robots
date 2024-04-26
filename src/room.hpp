#pragma once

#include <vector>
#include <memory>

#include <QGraphicsScene>

#include "obstacle.hpp"
#include "robot.hpp"

namespace icp {


enum class Corner : int {
    TopLeft = 0b00,
    TopRight = 0b01,
    BottomLeft = 0b10,
    BottomRight = 0b11,

    Right = 0b01,
    Bottom = 0b10,
};

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
    void corner_collision(Robot *rob, QPointF p, Corner corner);
    void not_collides_or(Robot *rob, QPointF a, QPointF b);
    bool hard_collision(Robot *r);

    std::vector<Obstacle *> obstacles;
    std::vector<Robot *> robots;
};

} // namespace icp
