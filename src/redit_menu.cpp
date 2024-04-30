#include "redit_menu.hpp"

#include <limits>

#include "auto_robot.hpp"

namespace icp {

using namespace std;

constexpr int R_DUMMY = 0;
constexpr int R_AUTO = 1;

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

ReditMenu::ReditMenu(QRect rect, QWidget *parent) :
    QWidget(parent),
    robot(nullptr)
{
    robot_select_label = new QLabel("type:", this);
    robot_select = new QComboBox(this);
    speed_label = new QLabel("speed:", this);
    speed = new QLineEdit(this);
    angle_label = new QLabel("angle:", this);
    angle = new QLineEdit(this);
    remove = new QPushButton("remove", this);
    deselect = new QPushButton("deselect", this);

    robot_select_label->hide();
    robot_select->hide();
    speed_label->hide();
    speed->hide();
    angle_label->hide();
    angle->hide();
    deselect->hide();
    remove->hide();

    robot_select->addItems({ "Dummy", "Auto" });
    speed->setValidator(
        new QDoubleValidator(0, numeric_limits<double>::max(), 2)
    );
    angle->setValidator(new QDoubleValidator(-360, 360, 2));

    connect(
        robot_select.data(),
        SIGNAL(currentIndexChanged(int)),
        this,
        SLOT(handle_type_change(int))
    );
    connect(
        speed,
        &QLineEdit::editingFinished,
        this,
        &ReditMenu::speed_editing_finished
    );
    connect(
        angle,
        &QLineEdit::editingFinished,
        this,
        &ReditMenu::angle_editing_finished
    );
    connect(
        deselect,
        &QPushButton::clicked,
        this,
        &ReditMenu::handle_deselect
    );
    connect(
        remove,
        &QPushButton::clicked,
        this,
        &ReditMenu::handle_remove
    );

    relayout(rect);
}

void ReditMenu::relayout(QRect rect) {
    setGeometry(rect);
    robot_select_label->setGeometry(QRect(5, 5, 40, 30));
    robot_select->setGeometry(QRect(40, 5, 80, 30));
    speed_label->setGeometry(QRect(125, 5, 45, 30));
    speed->setGeometry(QRect(170, 5, 60, 30));
    angle_label->setGeometry(QRect(235, 5, 45, 30));
    angle->setGeometry(QRect(280, 5, 60, 30));
    remove->setGeometry(QRect(rect.width() - 65, 5, 60, 30));
    deselect->setGeometry(QRect(rect.width() - 130, 5, 60, 30));
}

//---------------------------------------------------------------------------//
//                               PUBLIC SLOTS                                //
//---------------------------------------------------------------------------//

void ReditMenu::select_robot(Robot *robot) {
    if (robot) {
        robot_select_label->show();
        robot_select->show();
        speed_label->show();
        speed->show();
        angle_label->show();
        angle->show();
        deselect->show();
        remove->show();
    } else {
        robot_select_label->hide();
        robot_select->hide();
        speed_label->hide();
        speed->hide();
        angle_label->hide();
        angle->hide();
        deselect->hide();
        remove->hide();
    }

    this->robot = robot;

    robot_select->setCurrentIndex(get_robot_type());
    speed->setText(QString::number(robot->speed()));
    angle->setText(QString::number(-robot->orientation() / M_PI * 180));
}

//---------------------------------------------------------------------------//
//                              PRIVATE SLOTS                                //
//---------------------------------------------------------------------------//

void ReditMenu::handle_deselect(bool) {
    if (robot) {
        robot->set_selected(false);
    }
}

void ReditMenu::handle_remove(bool) {
    emit remove_robot(robot);
}

void ReditMenu::handle_type_change(int idx) {
    if (idx == get_robot_type()) {
        return;
    }

    switch (idx) {
        case R_DUMMY:
            emit change_robot(robot, new Robot(robot));
            break;
        case R_AUTO:
            emit change_robot(robot, new AutoRobot(robot));
            break;
    }
}

void ReditMenu::speed_editing_finished() {
    if (robot) {
        robot->set_speed(speed->text().toDouble());
    }
}

void ReditMenu::angle_editing_finished() {
    if (robot) {
        robot->set_angle(-angle->text().toDouble() / 180 * M_PI);
    }
}

//---------------------------------------------------------------------------//
//                                 PRIVATE                                   //
//---------------------------------------------------------------------------//

int ReditMenu::get_robot_type() {
    auto auto_rob = dynamic_cast<AutoRobot *>(robot);
    if (auto_rob) {
        return R_AUTO;
    }
    return R_DUMMY;
}

} // namespace icp
