#pragma once

#include <QLayout>
#include <QPushButton>
#include <QPointer>

#include "room.hpp"

namespace icp {

class Window : public QWidget {
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QPointer<QPushButton> menu_button;
    QPointer<QGraphicsView> room_view;
    QPointer<Room> room;
};

} // namespace icp
