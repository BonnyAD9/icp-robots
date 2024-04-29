#include "redit_menu.hpp"

namespace icp {

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

ReditMenu::ReditMenu(QRect rect, QWidget *parent) :
    QWidget(parent),
    robot(nullptr)
{
    remove = new QPushButton("remove", this);
    deselect = new QPushButton("deselect", this);

    deselect->hide();
    remove->hide();

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
    remove->setGeometry(QRect(rect.width() - 65, 5, 60, 30));
    deselect->setGeometry(QRect(rect.width() - 130, 5, 60, 30));
}

//---------------------------------------------------------------------------//
//                               PUBLIC SLOTS                                //
//---------------------------------------------------------------------------//

void ReditMenu::select_robot(Robot *robot) {
    if (robot) {
        deselect->show();
        remove->show();
    } else {
        deselect->hide();
        remove->hide();
    }
    this->robot = robot;
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

} // namespace icp
