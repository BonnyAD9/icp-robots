#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPointer>

namespace icp {

class SimControls : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Creates new simulation controls.
     * @param rect Position and size of the controls.
     * @param parent The parent widget.
     */
    SimControls(QRect rect, QWidget *parent = nullptr);

    /**
     * @brief Updates the layout of the widget.
     * @param rect New available space.
     */
    void relayout(QRect rect);

signals:
    /**
     * @brief Play/Pause button was pressed.
     *
     * @param play
     */
    void run_simulation(bool play);

private slots:
    void handle_play_pause(bool checked);

private:
    QPointer<QPushButton> play_pause;
    bool is_playing;
};

} // namespace icp
