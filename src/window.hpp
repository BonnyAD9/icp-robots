#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPointer>

namespace icp {

class Window : public QWidget {
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);
private:
    QPointer<QPushButton> menu_button;
};

} // namespace icp
