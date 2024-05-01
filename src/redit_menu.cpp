#include "redit_menu.hpp"

#include <limits>

#include "auto_robot.hpp"
#include "control_robot.hpp"

namespace icp {

using namespace std;

/**
 * @brief `AutoRobot`
 */
constexpr int R_AUTO = 0;
/**
 * @brief `ControlRobot`
 */
constexpr int R_CONTROL = 1;
/**
 * @brief `Robot` but not `AutoRobot` or `ControlRobot`
 */
constexpr int R_DUMMY = 2;

/**
 * @brief Modulo on a floating point number.
 */
qreal dmod(qreal num, unsigned mod) {
    auto sign = num > 0 ? 1 : -1;
    num = qAbs(num);
    auto rnum = (int)num % mod;
    return sign * (rnum + (num - (int)num));
}

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

ReditMenu::ReditMenu(QRect rect, QWidget *parent) :
    QWidget(parent),
    obj(nullptr)
{
    robot_select_label = new QLabel("type:", this);
    robot_select = new QComboBox(this);
    speed_label = new QLabel("speed:", this);
    speed = new QLineEdit(this);
    angle_label = new QLabel("angle:", this);
    angle = new QLineEdit(this);
    distance_label = new QLabel("d. dist.:", this);
    distance = new QLineEdit(this);
    rspeed_label = new QLabel("r. speed:", this);
    rspeed = new QLineEdit(this);
    rdist_label = new QLabel("r. dist.:", this);
    rdist = new QLineEdit(this);
    remove = new QPushButton("remove", this);
    deselect = new QPushButton("deselect", this);

    robot_select_label->hide();
    robot_select->hide();
    speed_label->hide();
    speed->hide();
    angle_label->hide();
    angle->hide();
    distance_label->hide();
    distance->hide();
    rspeed_label->hide();
    rspeed->hide();
    rdist_label->hide();
    rdist->hide();
    deselect->hide();
    remove->hide();

    robot_select->addItems({ "Auto", "Control", "Dummy" });
    speed->setValidator(
        new QDoubleValidator(0, numeric_limits<double>::max(), 2)
    );
    angle->setValidator(new QDoubleValidator(-360, 360, 2));
    distance->setValidator(
        new QDoubleValidator(0, numeric_limits<double>::max(), 2)
    );
    rspeed->setValidator(
        new QDoubleValidator(0, numeric_limits<double>::max(), 2)
    );
    rdist->setValidator(
        new QDoubleValidator(-360, 360, 2)
    );

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
        distance,
        &QLineEdit::editingFinished,
        this,
        &ReditMenu::distance_editing_finished
    );
    connect(
        rspeed,
        &QLineEdit::editingFinished,
        this,
        &ReditMenu::rspeed_editing_finished
    );
    connect(
        rdist,
        &QLineEdit::editingFinished,
        this,
        &ReditMenu::rdist_editing_finished
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
    rspeed_label->setGeometry(QRect(345, 5, 60, 30));
    rspeed->setGeometry(QRect(405, 5, 60, 30));
    distance_label->setGeometry(QRect(470, 5, 55, 30));
    distance->setGeometry(QRect(525, 5, 60, 30));
    rdist_label->setGeometry(QRect(590, 5, 50, 30));
    rdist->setGeometry(QRect(640, 5, 60, 30));
    remove->setGeometry(QRect(rect.width() - 65, 5, 60, 30));
    deselect->setGeometry(QRect(rect.width() - 130, 5, 60, 30));
}

//---------------------------------------------------------------------------//
//                               PUBLIC SLOTS                                //
//---------------------------------------------------------------------------//

void ReditMenu::select_obj(SceneObj *obj) {
    if (this->obj == obj) {
        return;
    }

    auto trob = dynamic_cast<Robot *>(this->obj);

    if (trob) {
        disconnect(
            trob,
            &Robot::angle_change,
            this,
            &ReditMenu::robot_angle_change
        );
    }

    robot_select_label->hide();
    robot_select->hide();
    speed_label->hide();
    speed->hide();
    angle_label->hide();
    angle->hide();
    distance_label->hide();
    distance->hide();
    rspeed_label->hide();
    rspeed->hide();
    rdist_label->hide();
    rdist->hide();
    deselect->hide();
    remove->hide();

    this->obj = obj;
    if (!obj) {
        return;
    }

    deselect->show();
    remove->show();

    auto rob = dynamic_cast<Robot *>(obj);
    if (!rob) {
        return;
    }

    robot_select_label->show();
    robot_select->show();
    speed_label->show();
    speed->show();
    angle_label->show();
    angle->show();

    connect(
        rob,
        &Robot::angle_change,
        this,
        &ReditMenu::robot_angle_change
    );

    robot_select->setCurrentIndex(get_robot_type());
    speed->setText(QString::number(rob->speed(), 'f', 2));
    auto num = dmod(-rob->orientation() / M_PI * 180, 360);
    if (num < -180) {
        num += 360;
    }
    angle->setText(QString::number(num, 'f', 2));

    AutoRobot *arob = dynamic_cast<AutoRobot *>(rob);
    if (arob) {
        distance_label->show();
        distance->show();
        rspeed_label->show();
        rspeed->show();
        rdist_label->show();
        rdist->show();

        distance->setText(QString::number(arob->edist(), 'f', 2));
        rspeed->setText(QString::number(arob->rspeed() / M_PI * 180, 'f', 2));
        rdist->setText(QString::number(-arob->rdist() / M_PI * 180, 'f', 2));
    }
    ControlRobot *crob = dynamic_cast<ControlRobot *>(rob);
    if (crob) {
        rspeed_label->show();
        rspeed->show();

        rspeed->setText(QString::number(crob->rspeed() / M_PI * 180));
    }
}

//---------------------------------------------------------------------------//
//                              PRIVATE SLOTS                                //
//---------------------------------------------------------------------------//

void ReditMenu::handle_deselect(bool) {
    if (obj) {
        obj->set_selected(false);
    }
}

void ReditMenu::handle_remove(bool) {
    emit remove_obj(obj);
}

void ReditMenu::handle_type_change(int idx) {
    auto rob = dynamic_cast<Robot *>(obj);
    if (idx == get_robot_type()) {
        return;
    }

    switch (idx) {
        case R_AUTO:
            emit change_robot(rob, new AutoRobot(rob));
            break;
        case R_CONTROL:
            emit change_robot(rob, new ControlRobot(rob));
            break;
        case R_DUMMY:
            emit change_robot(rob, new Robot(rob));
            break;
    }
}

void ReditMenu::speed_editing_finished() {
    auto rob = dynamic_cast<Robot *>(obj);
    if (rob) {
        rob->set_speed(speed->text().toDouble());
    }
}

void ReditMenu::angle_editing_finished() {
    auto rob = dynamic_cast<Robot *>(obj);
    if (rob) {
        rob->set_angle(-angle->text().toDouble() / 180 * M_PI);
    }
}

void ReditMenu::distance_editing_finished() {
    auto arob = dynamic_cast<AutoRobot *>(obj);
    if (arob) {
        arob->set_edist(distance->text().toDouble());
    }
}

void ReditMenu::rspeed_editing_finished() {
    auto arob = dynamic_cast<AutoRobot *>(obj);
    if (arob) {
        arob->set_rspeed(rspeed->text().toDouble() / 180 * M_PI);
    }
}

void ReditMenu::rdist_editing_finished() {
    auto arob = dynamic_cast<AutoRobot *>(obj);
    if (arob) {
        arob->set_rdist(-rdist->text().toDouble() / 180 * M_PI);
    }
}

void ReditMenu::robot_angle_change(qreal angle) {
    if (this->angle->hasFocus()) {
        return;
    }

    angle = dmod(-angle / M_PI * 180, 360);
    if (angle < -180) {
        angle += 360;
    }
    this->angle->setText(QString::number(angle, 'f', 2));
}

//---------------------------------------------------------------------------//
//                                 PRIVATE                                   //
//---------------------------------------------------------------------------//

int ReditMenu::get_robot_type() {
    auto auto_rob = dynamic_cast<AutoRobot *>(obj);
    if (auto_rob) {
        return R_AUTO;
    }
    auto control_rob = dynamic_cast<ControlRobot *>(obj);
    if (control_rob) {
        return R_CONTROL;
    }
    return R_DUMMY;
}

} // namespace icp
