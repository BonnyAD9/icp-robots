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
