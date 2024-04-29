#pragma once

#include <QWidget>

#include "robot.hpp"

namespace icp {

class ReditMenu : public QWidget {
public:
    ReditMenu(QRect rect, QWidget *parent = nullptr);

    void relayout(QRect rect);

private:
    Robot *robot;
};

} // namespace icp

