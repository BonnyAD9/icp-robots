#pragma once

#include <QLayout>
#include <QPushButton>
#include <QPointer>

#include "menu.hpp"
#include "room.hpp"
#include "sim_controls.hpp"
#include "redit_menu.hpp"

namespace icp {

class Window : public QWidget {
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void handleMenuBtnClick();

private:
    QPointer<QPushButton> menu_button;
    QPointer<Menu> menu;
    QPointer<QGraphicsView> room_view;
    QPointer<Room> room;
    QPointer<SimControls> sim_controls;
    QPointer<ReditMenu> redit_menu;
};

} // namespace icp
