#pragma once

#include <string>
#include <fstream>

#include <QWidget>

#include "auto_robot.hpp"
#include "control_robot.hpp"
#include "robot.hpp"
#include "room.hpp"
#include "obstacle.hpp"

namespace icp {

class Loader {
public:
    /**
     * @brief Creates new loader
     */
    Loader(std::string filename);

    /**
     * @brief Loads the Room from file
     */
    Room *load(QWidget *window);

private:
    Obstacle *load_obstacle();
    Robot *load_robot();
    AutoRobot *load_auto_robot();
    ControlRobot *load_control_robot();

    std::string read_ident();
    QSizeF read_size();
    QPointF read_pos();

    std::string filename;
    std::ifstream file;
    char cur;
};


} // namespace icp
