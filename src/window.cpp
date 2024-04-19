#include "window.hpp"

namespace icp {

Window::Window(QWidget *parent) : QWidget(parent) {
    setGeometry(0, 0, 800, 600);

    menu_button = new QPushButton("menu", this);
    menu_button->setGeometry(5, 5, 54, 30);
}

} // namespace icp
