#pragma once

#include <memory>

#include <QRectF>
#include <QPointer>
#include <QPushButton>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QWidget>

#include "obstacle.hpp"
#include "robot.hpp"
#include "room.hpp"

namespace icp {

class ObstacleButton : public QGraphicsRectItem {
public:
    explicit ObstacleButton(
        QRectF hitbox,
        QPointer<Room> room,
        QWidget *menu,
        QGraphicsItem *parent = nullptr
    );

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointer<Room> room;
    QWidget *menu;
};

class Menu : public QWidget {
    Q_OBJECT
public:
    explicit Menu(QPointer<Room> room, QWidget *parent = nullptr);

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
    QPointer<QGraphicsScene> button_scene;
    QPointer<QGraphicsView> button_view;
    ObstacleButton *obstacle_btn;
    QPointer<QPushButton> robot_btn;
};

} // namespace icp
