#include "sim_controls.hpp"

namespace icp {

SimControls::SimControls(QRect rect, QWidget *parent)
    : QWidget(parent),
    is_playing(true)
{
    play_pause = new QPushButton("pause", this);
    connect(
        play_pause,
        SIGNAL(clicked(bool)),
        this,
        SLOT(handle_play_pause(bool))
    );

    relayout(rect);
}

void SimControls::relayout(QRect rect) {
    play_pause->setGeometry(rect.width() - 65, 5, 60, 30);
    setGeometry(rect);
}

void SimControls::handle_play_pause(bool checked) {
    is_playing = !is_playing;
    play_pause->setText(is_playing ? "pause" : "play");
    emit run_simulation(is_playing);
}

} // namespace icp

