#include "menu.hpp"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

namespace icp {

using namespace std;

ObstacleButton::ObstacleButton(
    QRectF hitbox,
    QGraphicsItem *parent
) : QGraphicsRectItem(hitbox, parent),
    pos(hitbox.topLeft())
{
    setBrush(QBrush(QColor(0xff, 0x55, 0x55)));
    setPen(QPen(
        QColor(0xff, 0xff, 0xff),
        6,
        Qt::SolidLine,
        Qt::SquareCap,
        Qt::PenJoinStyle::SvgMiterJoin
    ));
}

void ObstacleButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        setCursor(Qt::ClosedHandCursor);
        grabMouse();
    }
}

void ObstacleButton::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto delta = event->scenePos() - event->lastScenePos();
    auto rec = rect();
    rec.moveTopLeft(rec.topLeft() + delta);
    setRect(rec.normalized());
}

void ObstacleButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        ungrabMouse();
        auto rec = rect();
        emit add_obstacle(rec);
        rec.moveTopLeft(pos);
        setRect(rec);
    }
}

/**
 * @brief Diameter of the robot.
 */
constexpr qreal ROBOT_D = 50;

RobotButton::RobotButton(
    QPoint pos,
    QGraphicsItem *parent
) : QGraphicsEllipseItem(QRectF(pos, QSizeF(ROBOT_D, ROBOT_D))),
    pos(pos)
{
    setBrush(QBrush(QColor(0x55, 0xcc, 0x55)));
    setPen(QPen(QColor(0xff, 0xff, 0xff), 6));
    setFlag(ItemIsMovable);
}

void RobotButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        setCursor(Qt::ClosedHandCursor);
        grabMouse();
    }
}

void RobotButton::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    auto delta = event->scenePos() - event->lastScenePos();
    auto rec = rect();
    rec.moveTopLeft(rec.topLeft() + delta);
    setRect(rec.normalized());
}

void RobotButton::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        ungrabMouse();
        auto rec = rect();
        emit add_robot(rec.topLeft());
        rec.moveTopLeft(pos);
        setRect(rec);
    }
}

Menu::Menu(QWidget *parent) : QWidget(parent) {
    setVisible(false);
    auto pal = QPalette();
    pal.setColor(QPalette::Window, QColor(0xff, 0xff, 0xff));
    pal.setColor(QPalette::Base, QColor(0, 0, 0, 0));

    button_scene = new QGraphicsScene(this);
    button_scene->setSceneRect(0, 0, 800, 600 - 40 * 2);

    button_scene->addItem(new ObstacleButton(QRectF(20, 190, 60, 60)));
    obstacle_btn = new ObstacleButton(QRectF(20, 190, 60, 60));
    connect(
        obstacle_btn, SIGNAL(add_obstacle(QRectF)),
        this, SLOT(handle_obstacle_btn(QRectF))
    );
    button_scene->addItem(obstacle_btn);

    button_scene->addItem(new RobotButton(QPoint(25, 270)));
    robot_btn = new RobotButton(QPoint(25, 270));
    connect(
        robot_btn, SIGNAL(add_robot(QPointF)),
        this, SLOT(handle_robot_btn(QPointF))
    );
    button_scene->addItem(robot_btn);

    button_view = new QGraphicsView(button_scene, this);
    button_view->setGeometry(0, 0, 800, 600 - 40 * 2);
    button_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    button_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    button_view->setFrameStyle(QFrame::NoFrame);
    button_view->setRenderHints(
        QPainter::Antialiasing | QPainter::SmoothPixmapTransform
    );
    button_view->setPalette(pal);

    close_btn = new QPushButton("close", this);
    close_btn->setGeometry(5, 5, 54, 30);
    connect(close_btn, &QPushButton::clicked, this, &Menu::handle_close_btn);
}

void Menu::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    painter.fillRect(QRect(0, 0, 100, 600 - 40 * 2), QColor(0x40, 0x40, 0x40));
}

void Menu::handle_close_btn() {
    setVisible(false);
}

void Menu::handle_obstacle_btn(QRectF rec) {
    setVisible(false);
    emit add_obstacle(new Obstacle(rec));
}

void Menu::handle_robot_btn(QPointF point) {
    setVisible(false);
    emit add_robot(new Robot(point));
}

} // namespace icp
