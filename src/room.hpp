#pragma once

#include <vector>
#include <memory>
#include <fstream>

#include <QGraphicsScene>

#include "obstacle.hpp"
#include "robot.hpp"
#include "control_robot.hpp"
#include "auto_robot.hpp"

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
     * @brief Loads the room from the file
     * @param filename file to load room from
     * @param parent parent object
     */
    Room(std::string filename, QObject *parent = nullptr);

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
     * @brief Signal for new object selection
     * @param o new selected object
     */
    void new_selection(SceneObj *o);

    /**
     * @brief Signal for window resize
     * @param size new size of the window
     */
    void resize(QPointF size);

public slots:
    /**
     * @brief Play/pause the simulation.
     * @param play
     */
    void run_simulation(bool play);

    /**
     * @brief Remove object.
     * @param o object to be removed
     */
    void remove_obj(SceneObj *o);

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

    /**
     * @brief Saves room
     * @param filename file to save the room into
     */
    void save(std::string filename);

protected:
    void timerEvent(QTimerEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void select_obj(SceneObj *o);

private:
    void load(std::string filename);

    Obstacle *load_obstacle(std::ifstream &file);
    Robot *load_robot(std::ifstream &file);
    AutoRobot *load_auto_robot(std::ifstream &file);
    ControlRobot *load_control_robot(std::ifstream &file);

    std::string read_ident(std::ifstream &file);
    QPointF read_size(std::ifstream &file);
    QPointF read_pos(std::ifstream &file);
    qreal read_num(std::ifstream &file);

    void tick(qreal delta);
    void move_robots(qreal delta);
    void border_collision(Robot *rob);
    void obstacle_collision(Robot *rob, Obstacle *obs);
    void robot_collision(Robot *r1, Robot *r2);
    void corner_collision(Robot *rob, QPointF p);
    qreal obstacle_distance(Robot *rob);

    std::vector<Obstacle *> obstacles;
    std::vector<Robot *> robots;

    SceneObj *selected;

    int timer;
};

} // namespace icp
