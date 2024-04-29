#include "sim_controls.hpp"

namespace icp {

SimControls::SimControls(QRect rect, QWidget *parent)
    : QWidget(parent)
{
    play_pause = new QPushButton("pause", this);
    relayout(rect);
}

void SimControls::relayout(QRect rect) {
    play_pause->setGeometry(rect.width() - 65, 5, 60, 30);
    setGeometry(rect);
}

} // namespace icp

