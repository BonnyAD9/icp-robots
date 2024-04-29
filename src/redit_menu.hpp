#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPointer>

#include "robot.hpp"

namespace icp {

class ReditMenu : public QWidget {
    Q_OBJECT

public:
    ReditMenu(QRect rect, QWidget *parent = nullptr);

    void relayout(QRect rect);

public slots:
    void select_robot(Robot *robot);

private slots:
    void handle_deselect(bool);

private:
    Robot *robot;

    QPointer<QPushButton> deselect;
};

} // namespace icp

