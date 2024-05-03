/**
 * @file
 * @author Jakub Antonín Štigler (xstigl00)
 * @brief Base class for all objects that can be placed into a room.
 * (source file)
 */

#include "scene_obj.hpp"

namespace icp {

//---------------------------------------------------------------------------//
//                                  PUBLIC                                   //
//---------------------------------------------------------------------------//

void SceneObj::set_selected(bool val) {
    selection_event(val);
    if (val) {
        emit select(this);
    } else {
        emit select(nullptr);
    }
}

//---------------------------------------------------------------------------//
//                                PROTECTED                                  //
//---------------------------------------------------------------------------//

void SceneObj::selection_event(bool selected) {}

} // namespace icp
