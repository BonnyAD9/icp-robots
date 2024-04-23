#pragma once

#include <QPointer>
#include <QPushButton>
#include <QGraphicsRectItem>
#include <QWidget>

namespace icp {

class Menu : public QWidget {
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void handleCloseBtn();

private:
    QPointer<QPushButton> close_button;
};

} // namespace icp
