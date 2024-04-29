#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPointer>

namespace icp {

class SimControls : public QWidget {
public:
    SimControls(QRect rect, QWidget *parent = nullptr);

    void relayout(QRect rect);

private:
    QPointer<QPushButton> play_pause;
};

} // namespace icp
