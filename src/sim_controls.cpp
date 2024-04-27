#include "sim_controls.hpp"

namespace icp {

SimControls::SimControls(QRectF rect, QWidget *parent)
    : QWidget(parent)
{
    play_pause = new QPushButton("pause", this);
    relayout(rect);
}

void SimControls::relayout(QRectF rect) {
    play_pause->setGeometry(rect.width() - 65, rect.y() + 5, 60, 30);
}

} // namespace icp

