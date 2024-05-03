/**
 * @mainpage icp-robots
 *
 * @include ../README.txt
 */

/**
 * @file
 * @author Jakub Antonín Štigler (xstigl00)
 * @brief Application entry point.
 */


#include <iostream>

#include <QApplication>

#include "window.hpp"

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    icp::Window window;
    window.show();

    app.exec();
}
