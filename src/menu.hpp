#pragma once

#include <memory>

#include <QRectF>
#include <QPointer>
#include <QPushButton>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QWidget>

#include "obstacle.hpp"
#include "robot.hpp"
#include "room.hpp"

namespace icp {

/**
 * @brief Represents obstacle menu button
 */
class ObstacleButton : public QObject, public QGraphicsRectItem {
    Q_OBJECT
public:
    /**
     * @brief Creates new Obstacle menu button
     * @param hitbox Hitbox of the button
     * @param parent Parent item
     */
    explicit ObstacleButton(QRectF hitbox,QGraphicsItem *parent = nullptr);

signals:
    void add_obstacle(QRectF rec);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF pos;
};

/**
 * @brief Represents robot menu button
 */
class RobotButton : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    /**
     * @brief Creates new Robot menu button
     * @param hitbox Hitbox of the button
     * @param parent Parent item
     */
    explicit RobotButton(QPoint pos, QGraphicsItem *parent = nullptr);

signals:
    void add_robot(QPointF point);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF pos;
};

class Menu : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Creates new menu
     * @param parent Parent widget
     */
    explicit Menu(QWidget *parent = nullptr);

signals:
    void add_obstacle(Obstacle *obstacle);
    void add_robot(Robot *robot);

protected:
    void paintEvent(QPaintEvent *e) override;

private slots:
    void handle_close_btn();
    void handle_obstacle_btn(QRectF rec);
    void handle_robot_btn(QPointF point);

private:
    QPointer<QPushButton> close_btn;
    QPointer<QGraphicsScene> button_scene;
    QPointer<QGraphicsView> button_view;
    ObstacleButton *obstacle_btn;
    RobotButton *robot_btn;
};

} // namespace icp
