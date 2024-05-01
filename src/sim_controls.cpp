#include "sim_controls.hpp"

namespace icp {

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

SimControls::SimControls(QRect rect, QWidget *parent)
    : QWidget(parent),
    is_playing(true)
{
    layout = new QHBoxLayout(this);

    path = new QLineEdit(this);
    save = new QPushButton("save", this);
    connect(save, &QPushButton::clicked, this, &SimControls::handle_save);

    load = new QPushButton("load", this);

    play_pause = new QPushButton("pause", this);
    connect(
        play_pause,
        &QPushButton::clicked,
        this,
        &SimControls::handle_play_pause
    );

    layout->addWidget(path);
    layout->addWidget(save);
    layout->addWidget(load);
    layout->addStretch(1);
    layout->addWidget(play_pause);

    relayout(rect);
}

void SimControls::relayout(QRect rect) {
    setGeometry(rect);
}

//---------------------------------------------------------------------------//
//                              PRIVATE SLOTS                                //
//---------------------------------------------------------------------------//

void SimControls::handle_play_pause(bool checked) {
    is_playing = !is_playing;
    play_pause->setText(is_playing ? "pause" : "play");
    emit run_simulation(is_playing);
}

void SimControls::handle_save() {
    emit save_room("test.txt");
}

} // namespace icp

