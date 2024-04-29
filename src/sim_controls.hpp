#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPointer>

namespace icp {

class SimControls : public QWidget {
    Q_OBJECT

public:
    SimControls(QRect rect, QWidget *parent = nullptr);

    void relayout(QRect rect);

signals:
    void run_simulation(bool play);

private slots:
    void handle_play_pause(bool checked);

private:
    QPointer<QPushButton> play_pause;
    bool is_playing;
};

} // namespace icp
