/**
 * @file
 * @authors Martin Slezák (xsleza26), Jakub Antonín Štigler (xstigl00)
 * @brief The main window of the aplication (header file)
 */

#pragma once

#include <QLayout>
#include <QPushButton>
#include <QPointer>

#include "menu.hpp"
#include "room.hpp"
#include "sim_controls.hpp"
#include "redit_menu.hpp"
#include "loader.hpp"

namespace icp {

/**
 * @brief Class representing window
 */
class Window : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Creates new window
     * @param parent Parent widget
     */
    explicit Window(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void show_menu();

    void load(std::string filename);

private:
    void room_listeners();

    void room_rem_listeners();

    QPointer<QPushButton> menu_button;
    QPointer<Menu> menu;
    QPointer<QGraphicsView> room_view;
    QPointer<Room> room;
    QPointer<SimControls> sim_controls;
    QPointer<ReditMenu> redit_menu;
};

} // namespace icp
