#include <iostream>

#include <QApplication>

#include "window.hpp"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    icp::Window window;
    window.show();

    app.exec();
}
