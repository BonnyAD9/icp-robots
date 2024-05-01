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

    path_input = new QLineEdit(this);

    save = new QPushButton("save", this);
    connect(save, &QPushButton::clicked, this, &SimControls::handle_save);

    load = new QPushButton("load", this);
    connect(load, &QPushButton::clicked, this, &SimControls::handle_load);

    play_pause = new QPushButton("pause", this);
    connect(
        play_pause,
        &QPushButton::clicked,
        this,
        &SimControls::handle_play_pause
    );

    layout->addWidget(path_input, 1);
    layout->addWidget(save);
    layout->addWidget(load);
    layout->addWidget(play_pause);

    relayout(rect);
}

void SimControls::relayout(QRect rect) {
    setGeometry(rect);
}

bool SimControls::playing() {
    return is_playing;
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
    emit save_room(path_input->text().toStdString());
}

void SimControls::handle_load() {
    emit load_room(path_input->text().toStdString());
}

} // namespace icp

