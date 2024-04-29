#include "redit_menu.hpp"

namespace icp {

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

ReditMenu::ReditMenu(QRect rect, QWidget *parent) :
    QWidget(parent),
    robot(nullptr)
{
    deselect = new QPushButton("deselect", this);
    deselect->hide();

    connect(
        deselect,
        &QPushButton::clicked,
        this,
        &ReditMenu::handle_deselect
    );

    relayout(rect);
}

void ReditMenu::relayout(QRect rect) {
    setGeometry(rect);
    deselect->setGeometry(QRect(rect.width() - 65, 5, 60, 30));
}

//---------------------------------------------------------------------------//
//                               PUBLIC SLOTS                                //
//---------------------------------------------------------------------------//

void ReditMenu::select_robot(Robot *robot) {
    if (robot) {
        deselect->show();
    } else {
        deselect->hide();
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

} // namespace icp
