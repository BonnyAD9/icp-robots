#include "redit_menu.hpp"

namespace icp {

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

ReditMenu::ReditMenu(QRect rect, QWidget *parent) : QWidget(parent) {
    relayout(rect);
}

void ReditMenu::relayout(QRect rect) {
    setGeometry(rect);
}

} // namespace icp
