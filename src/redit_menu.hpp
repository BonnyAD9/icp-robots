/**
 * @file
 * @authors Martin Slezák (xsleza26), Jakub Antonín Štigler (xstigl00)
 * @brief Menu for editing robot parameters (or delete obstacle). (header file)
 */

#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPointer>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>

#include "robot.hpp"

namespace icp {

/**
 * @brief Menu for editing robots
 */
class ReditMenu : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Creates new robot edit menu
     * @param rect size of the menu
     * @param parent Parent widget
     */
    ReditMenu(QRect rect, QWidget *parent = nullptr);

    /**
     * @brief Re-layouts the menu
     * @param rect new size of the menu
     */
    void relayout(QRect rect);

signals:
    /**
     * @brief Signal to remove robot from room
     * @param robot robot to be removed
     */
    void remove_obj(SceneObj *robot);

    /**
     * @brief Signal for changing robot type
     * @param old old robot to be replaced
     * @param replace new robot with new type
     */
    void change_robot(Robot *old, Robot *replace);

public slots:
    /**
     * @brief Handles selecting robot
     * @param robot robot to be selected
     */
    void select_obj(SceneObj *robot);

private slots:
    void handle_deselect(bool);
    void handle_remove(bool);
    void handle_type_change(int idx);
    void speed_editing_finished();
    void angle_editing_finished();
    void distance_editing_finished();
    void rspeed_editing_finished();
    void rdist_editing_finished();
    void robot_angle_change(qreal angle);

private:
    int get_robot_type();

    SceneObj *obj;

    QPointer<QLabel> robot_select_label;
    QPointer<QComboBox> robot_select;
    QPointer<QLabel> speed_label;
    QPointer<QLineEdit> speed;
    QPointer<QLabel> angle_label;
    QPointer<QLineEdit> angle;
    QPointer<QLabel> distance_label;
    QPointer<QLineEdit> distance;
    QPointer<QLabel> rspeed_label;
    QPointer<QLineEdit> rspeed;
    QPointer<QLabel> rdist_label;
    QPointer<QLineEdit> rdist;
    QPointer<QPushButton> remove;
    QPointer<QPushButton> deselect;
};

} // namespace icp

