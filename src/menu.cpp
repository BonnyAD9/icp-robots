#include "menu.hpp"

#include <QPainter>

namespace icp {

using namespace std;

Menu::Menu(QWidget *parent) : QWidget(parent) {
    setVisible(false);

    close_btn = new QPushButton("close", this);
    close_btn->setGeometry(5, 5, 54, 30);
    connect(close_btn, &QPushButton::clicked, this, &Menu::handle_close_btn);

    obstacle_btn = new QPushButton("obstacle", this);
    obstacle_btn->setGeometry(5, 40, 54, 30);
    connect(
        obstacle_btn,
        SIGNAL(clicked()),
        this,
        SLOT(handle_obstacle_btn())
    );

    robot_btn = new QPushButton("robot", this);
    robot_btn->setGeometry(5, 75, 54, 30);
    connect(robot_btn, SIGNAL(clicked()), this, SLOT(handle_robot_btn()));
}

void Menu::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), QColor("#404040"));
}

void Menu::handle_close_btn() {
    setVisible(false);
}

void Menu::handle_obstacle_btn() {
    emit add_obstacle(new Obstacle(QRectF(100, 200, 60, 60)));
}

void Menu::handle_robot_btn() {
    emit add_robot(new Robot(QPoint(200, 100)));
}

} // namespace icp
