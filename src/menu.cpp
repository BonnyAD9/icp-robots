#include "menu.hpp"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

namespace icp {

using namespace std;

ObstacleButton::ObstacleButton(
    QRectF hitbox,
    QPointer<Room> room,
    QWidget *menu,
    QGraphicsItem *parent
)
    : QGraphicsRectItem(hitbox, parent)
{
    setBrush(QBrush(QColor(0xff, 0x55, 0x55)));
    setPen(QPen(
        QColor(0xff, 0xff, 0xff),
        6,
        Qt::SolidLine,
        Qt::SquareCap,
        Qt::PenJoinStyle::SvgMiterJoin
    ));
    this->room = room;
    this->menu = menu;
}

void ObstacleButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    auto pos = event->scenePos();

    menu->setVisible(false);
    room->add_obstacle(unique_ptr<Obstacle>(
        new Obstacle(QRectF(pos.x(), pos.y(), 60, 60))),
        true
    );
    // emit add_obstacle(new Obstacle(boundingRect()));
}

Menu::Menu(QPointer<Room> room, QWidget *parent) : QWidget(parent) {
    setVisible(false);

    close_btn = new QPushButton("close", this);
    close_btn->setGeometry(5, 5, 54, 30);
    connect(close_btn, &QPushButton::clicked, this, &Menu::handle_close_btn);

    // obstacle_btn = new QPushButton("obstacle", this);
    // obstacle_btn->setGeometry(5, 40, 54, 30);
    // connect(
    //     obstacle_btn,
    //     SIGNAL(clicked()),
    //     this,
    //     SLOT(handle_obstacle_btn())
    // );
    button_scene = new QGraphicsScene(this);

    obstacle_btn = new ObstacleButton(QRectF(0, 0, 60, 60), room, this);
    button_scene->addItem(obstacle_btn);

    button_view = new QGraphicsView(button_scene, this);
    button_view->setGeometry(5, 40, 90, 300);

    // robot_btn = new QPushButton("robot", this);
    // robot_btn->setGeometry(5, 75, 54, 30);
    // connect(robot_btn, SIGNAL(clicked()), this, SLOT(handle_robot_btn()));
}

void Menu::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(rect(), QColor("#404040"));
}

void Menu::handle_close_btn() {
    setVisible(false);
}

void Menu::handle_obstacle_btn() {
    setVisible(false);
    emit add_obstacle(new Obstacle(QRectF(100, 200, 60, 60)));
}

void Menu::handle_robot_btn() {
    setVisible(false);
    emit add_robot(new Robot(QPoint(200, 100)));
}

} // namespace icp
