#include "window.hpp"

#include <memory>

#include <QGraphicsView>
#include <QResizeEvent>
#include <QMessageBox>

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

    menu_button = new QPushButton("menu", this);
    menu_button->setGeometry(5, 45, 54, 30);
    connect(menu_button, &QPushButton::clicked, this, &Window::show_menu);

    menu = new Menu(QSize(800, 600 - 40 * 2), this);
    menu->setGeometry(0, 40, 800, 600 - 40 * 2);

    sim_controls = new SimControls(QRect(0, 600 - 40, width(), 40), this);
    connect(sim_controls, &SimControls::load_room, this, &Window::load);

    room_listeners();
}

//---------------------------------------------------------------------------//
//                                PROTECTED                                  //
//---------------------------------------------------------------------------//

void Window::resizeEvent(QResizeEvent *event) {
    auto size = event->size();
    room_view->resize(QSize(size.width(), size.height() - 40 * 2));
    room->setSceneRect(0, 0, size.width(), size.height() - 40 * 2);
    menu->relayout(QSize(size.width(), size.height() - 40 * 2));
    menu->resize(size.width(), size.height() - 40 * 2);
    sim_controls->relayout(QRect(0, size.height() - 40, size.width(), 40));
    redit_menu->relayout(QRect(0, 0, size.width(), 40));
}

//---------------------------------------------------------------------------//
//                               PRIVATE SLOTS                               //
//---------------------------------------------------------------------------//

void Window::show_menu() {
    menu->show();
}

void Window::load(std::string filename) {
    Room *new_room;
    auto loader = Loader(filename);
    try {
        new_room = loader.load(this);
    } catch (const exception &e) {
        QMessageBox::critical(nullptr, "Error loading room", e.what());
        return;
    }
    room = new_room;
    room->setSceneRect(0, 0, width(), height() - 40 * 2);
    room->run_simulation(sim_controls->playing());

    room_listeners();

    room_view->setScene(room);
}


//---------------------------------------------------------------------------//
//                                  PRIVATE                                  //
//---------------------------------------------------------------------------//

void Window::room_listeners() {
    connect(
        sim_controls,
        &SimControls::run_simulation,
        room,
        &Room::run_simulation
    );
    connect(sim_controls, &SimControls::save_room, room, &Room::save);

    connect(room, &Room::new_selection, redit_menu, &ReditMenu::select_obj);
    connect(redit_menu, &ReditMenu::remove_obj, room, &Room::remove_obj);
    connect(redit_menu, &ReditMenu::change_robot, room, &Room::change_robot);

    connect(menu, &Menu::add_obstacle, room, &Room::add_obstacle_slot);
    connect(menu, &Menu::add_robot, room, &Room::add_robot_slot);
}

} // namespace icp
