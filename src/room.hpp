#pragma once

#include <vector>
#include <memory>

#include <QGraphicsScene>

#include "obstacle.hpp"
#include "robot.hpp"
#include "control_robot.hpp"

namespace icp {

/**
 * @brief Room that can contain robots and obstacles.
 */
class Room : public QGraphicsScene {
    Q_OBJECT

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

signals:
    /**
     * @brief Signal for new robot selection
     * @param r new selected robot
     */
    void new_selection(Robot *r);

public slots:
    /**
     * @brief Play/pause the simulation.
     * @param play
     */
    void run_simulation(bool play);

    /**
     * @brief Remove robot
     * @param r robot to be removed
     */
    void remove_robot(Robot *r);

    /**
     * @brief Changes robot to new type
     * @param old old robot to be replaced
     * @param replace new robot with new type
     */
    void change_robot(Robot *old, Robot *replace);

    /**
     * @brief Adds new obstacle
     * @param obstacle obstacle to be added to room
     */
    void add_obstacle_slot(Obstacle *obstacle);

    /**
     * @brief Adds new robot
     * @param robot robot to be added to room
     */
    void add_robot_slot(Robot *robot);

protected:
    void timerEvent(QTimerEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void select_robot(Robot *r);

private:
    void tick(qreal delta);
    void move_robots(qreal delta);
    void border_collision(Robot *rob);
    void obstacle_collision(Robot *rob, Obstacle *obs);
    void robot_collision(Robot *r1, Robot *r2);
    void corner_collision(Robot *rob, QPointF p);
    qreal obstacle_distance(Robot *rob);

    std::vector<Obstacle *> obstacles;
    std::vector<Robot *> robots;

    Robot *selected;

    int timer;
};

} // namespace icp
