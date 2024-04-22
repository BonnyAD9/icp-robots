#include "menu.hpp"

#include <QPainter>

namespace icp {

using namespace std;

Menu::Menu(QWidget *parent) : QWidget(parent) {
    setVisible(false);

    close_button = new QPushButton("close", this);
    close_button->setGeometry(5, 5, 54, 30);
    connect(close_button, &QPushButton::clicked, this, &Menu::handleCloseBtn);
}

void Menu::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), QColor("#404040"));
}

void Menu::handleCloseBtn() {
    setVisible(false);
}

} // namespace icp
