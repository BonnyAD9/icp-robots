#include "window.hpp"

#include <memory>

#include <QGraphicsView>
#include <QResizeEvent>

#include "obstacle.hpp"
#include "auto_robot.hpp"

namespace icp {

using namespace std;

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

Window::Window(QWidget *parent) : QWidget(parent) {
    setGeometry(0, 0, 900, 600);

    room = new Room();
    room->setSceneRect(0, 40, width(), 600 - 40 * 2);

    room_view = new QGraphicsView(room, this);
    room_view->setGeometry(0, 40, width(), 600 - 40 * 2);
    room_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    room_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    room_view->setFrameStyle(QFrame::NoFrame);
    room_view->setRenderHints(
        QPainter::Antialiasing | QPainter::SmoothPixmapTransform
    );

    redit_menu = new ReditMenu(QRect(0, 0, width(), 40), this);
    connect(room, &Room::new_selection, redit_menu, &ReditMenu::select_obj);
    connect(redit_menu, &ReditMenu::remove_obj, room, &Room::remove_obj);
    connect(redit_menu, &ReditMenu::change_robot, room, &Room::change_robot);

    menu_button = new QPushButton("menu", this);
    menu_button->setGeometry(5, 45, 54, 30);
    connect(
        menu_button, &QPushButton::clicked, this, &Window::handleMenuBtnClick
    );

    menu = new Menu(this);
    menu->setGeometry(0, 40, 800, 600 - 40 * 2);

    sim_controls = new SimControls(QRect(0, 600 - 40, width(), 40), this);
    connect(
        sim_controls,
        &SimControls::run_simulation,
        room,
        &Room::run_simulation
    );
    connect(sim_controls, &SimControls::save_room, room, &Room::save);

    connect(
        menu,
        SIGNAL(add_obstacle(Obstacle *)),
        room,
        SLOT(add_obstacle_slot(Obstacle *))
    );

    connect(
        menu,
        SIGNAL(add_robot(Robot *)),
        room,
        SLOT(add_robot_slot(Robot *))
    );

    // test code
    room->add_obstacle(unique_ptr<Obstacle>(
        new Obstacle(QRectF(100, 200, 60, 60))
    ));
    room->add_obstacle(unique_ptr<Obstacle>(
        new Obstacle(QRectF(400, 50, 60, 450))
    ));

    room->add_robot(unique_ptr<Robot>(new AutoRobot(QPoint(200, 100))));

    room->add_robot(unique_ptr<Robot>(new Robot(QPoint(201, 200), 0, 0)));
}

//---------------------------------------------------------------------------//
//                                PROTECTED                                  //
//---------------------------------------------------------------------------//

void Window::resizeEvent(QResizeEvent *event) {
    auto size = event->size();
    room_view->resize(QSize(size.width(), size.height() - 40 * 2));
    room->setSceneRect(0, 0, size.width(), size.height() - 40 * 2);
    menu->resize(menu->width(), size.height());
    sim_controls->relayout(QRect(0, size.height() - 40, size.width(), 40));
    redit_menu->relayout(QRect(0, 0, size.width(), 40));
}

//---------------------------------------------------------------------------//
//                               PRIVATE SLOTS                               //
//---------------------------------------------------------------------------//

void Window::handleMenuBtnClick() {
    menu->show();
}

} // namespace icp
