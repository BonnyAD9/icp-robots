#include "window.hpp"

namespace icp {

Window::Window(QWidget *parent) : QWidget(parent) {
    setGeometry(0, 0, 800, 600);
}

} // namespace icp
