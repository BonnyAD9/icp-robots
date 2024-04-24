#pragma once

#include <memory>

#include <QPointer>
#include <QPushButton>
#include <QGraphicsRectItem>
#include <QWidget>

#include "obstacle.hpp"
#include "robot.hpp"

namespace icp {

class Menu : public QWidget {
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void add_obstacle(Obstacle *obstacle);
    void add_robot(Robot *robot);

private slots:
    void handle_close_btn();
    void handle_obstacle_btn();
    void handle_robot_btn();

private:
    QPointer<QPushButton> close_btn;
    QPointer<QPushButton> obstacle_btn;
    QPointer<QPushButton> robot_btn;
};

} // namespace icp
