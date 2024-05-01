#pragma once

#include <QWidget>
#include <QPushButton>
#include <QPointer>
#include <QLineEdit>
#include <QHBoxLayout>

namespace icp {

/**
 * @brief Menu for controling the simulation.
 */
class SimControls : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Creates new simulation controls.
     * @param rect Position and size of the controls.
     * @param parent The parent widget.
     */
    SimControls(QRect rect, QWidget *parent = nullptr);

    /**
     * @brief Updates the layout of the widget.
     * @param rect New available space.
     */
    void relayout(QRect rect);

signals:
    /**
     * @brief Play/Pause button was pressed.
     * @param play `true` if the simulation should run, otherwise `false`.
     */
    void run_simulation(bool play);

    /**
     * @brief Save button was pressed
     * @param filename file to save room into
     */
    void save_room(std::string filename);

    /**
     * @brief Load button was pressed
     * @param filename file to load room from
     */
    void load_room(std::string filename);

private slots:
    void handle_play_pause(bool checked);

    void handle_save();
    void handle_load();

private:
    QPointer<QHBoxLayout> layout;
    QPointer<QLineEdit> path_input;
    QPointer<QPushButton> play_pause;
    QPointer<QPushButton> save;
    QPointer<QPushButton> load;

    bool is_playing;
};

} // namespace icp
