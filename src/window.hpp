#pragma once

#include <QWidget>

namespace icp {

class Window : public QWidget {
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);
};

} // namespace icp
