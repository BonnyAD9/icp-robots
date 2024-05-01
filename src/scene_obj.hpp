#pragma once

#include <QObject>

namespace icp {

/**
 * @brief Class representing object that can be placed into the room with
 * robots and can be selected.
 */
class SceneObj : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Select/unselect the object.
     * @param val when `true` object is selected, otherwise object is
     * unselected.
     */
    void set_selected(bool val = true);

signals:
    /**
     * @brief Emited when the object selection is changed.
     * @param sender `this` when the object is selected, `nullptr` when the
     * object is unselected.
     */
    void select(SceneObj *sender);

protected:
    virtual void selection_event(bool selected);
};

} // namespace icp

