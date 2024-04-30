#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPointer>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>

#include "robot.hpp"

namespace icp {

class ReditMenu : public QWidget {
    Q_OBJECT

public:
    ReditMenu(QRect rect, QWidget *parent = nullptr);

    void relayout(QRect rect);

signals:
    void remove_robot(Robot *robot);
    void change_robot(Robot *old, Robot *replace);

public slots:
    void select_robot(Robot *robot);

private slots:
    void handle_deselect(bool);
    void handle_remove(bool);
    void handle_type_change(int idx);
    void speed_editing_finished();
    void angle_editing_finished();
    void distance_editing_finished();
    void rspeed_editing_finished();
    void rdist_editing_finished();

private:
    int get_robot_type();

    Robot *robot;

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

