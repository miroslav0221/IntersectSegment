#pragma once

#include "imgui.h"
#include "view/State.h"

namespace view {

    void render2D(ImDrawList *dl, State &s, float vpX, float vpY, float vpW, float vpH);

    void render3D(ImDrawList *dl, State &s, float vpX, float vpY, float vpW, float vpH);

}
