#include "window.hpp"

#include <memory>

#include <QGraphicsView>
#include <QResizeEvent>

#include "obstacle.hpp"

namespace icp {

using namespace std;

Window::Window(QWidget *parent) : QWidget(parent) {
    setGeometry(0, 0, 800, 600);

    room = new Room();
    room->setSceneRect(0, 0, 800, 600);

    room_view = new QGraphicsView(room, this);
    room_view->setGeometry(0, 0, 800, 600);
    room_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    room_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    room_view->setFrameStyle(QFrame::NoFrame);
    room_view->setRenderHints(
        QPainter::Antialiasing | QPainter::SmoothPixmapTransform
    );

    menu_button = new QPushButton("menu", this);
    menu_button->setGeometry(5, 5, 54, 30);
    connect(
        menu_button, &QPushButton::clicked, this, &Window::handleMenuBtnClick
    );

    menu = new Menu(this);
    menu->setGeometry(0, 0, 100, 600);

    // test code
    room->add_obstacle(unique_ptr<Obstacle>(
        new Obstacle(QRectF(100, 200, 60, 60))
    ));
    room->add_obstacle(unique_ptr<Obstacle>(
        new Obstacle(QRectF(100, 300, 60, 60))
    ));

    room->add_robot(unique_ptr<Robot>(new Robot(QPoint(200, 100))));

    room->add_robot(unique_ptr<Robot>(new Robot(QPoint(200, 200), QPoint(0, 0))));
}

void Window::resizeEvent(QResizeEvent *event) {
    auto size = event->size();
    room_view->resize(size);
    room->setSceneRect(0, 0, size.width(), size.height());
    menu->resize(menu->width(), size.height());
}

void Window::handleMenuBtnClick() {
    menu->show();
}

} // namespace icp
