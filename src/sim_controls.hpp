#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPointer>

namespace icp {

class SimControls : public QWidget {
public:
    SimControls(QRectF rect, QWidget *parent = nullptr);

    void relayout(QRectF rect);

private:
    QPointer<QPushButton> play_pause;
};

} // namespace icp
