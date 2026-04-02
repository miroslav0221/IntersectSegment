#pragma once

#include "imgui.h"

#include <string>

namespace view {

    namespace ui {
        inline constexpr ImU32 COL_BG = IM_COL32(13, 17, 23, 255);
        inline constexpr ImU32 COL_AB = IM_COL32(88, 166, 255, 255);
        inline constexpr ImU32 COL_CD = IM_COL32(210, 153, 34, 255);
        inline constexpr ImU32 COL_HIT = IM_COL32(248, 81, 73, 255);
        inline constexpr ImU32 COL_GRID = IM_COL32(139, 148, 158, 20);
        inline constexpr ImU32 COL_AXIS = IM_COL32(139, 148, 158, 46);
        inline constexpr ImU32 COL_AXIS_X = IM_COL32(255, 100, 100, 178);
        inline constexpr ImU32 COL_AXIS_Y = IM_COL32(100, 255, 100, 178);
        inline constexpr ImU32 COL_AXIS_Z = IM_COL32(100, 140, 255, 178);
        inline constexpr ImU32 COL_LABEL = IM_COL32(72, 79, 88, 255);
        inline constexpr ImU32 COL_DIM_AXIS = IM_COL32(139, 148, 158, 25);

        inline constexpr float PANEL_W = 300.0f;
    }

    struct State {
        bool mode3d = false;
        float ax = 0, ay = 0, az = 0;
        float bx = 2, by = 2, bz = 0;
        float cx = 0, cy = 2, cz = 0;
        float dx = 2, dy = 0, dz = 0;

        bool computed = false;
        bool isError = false;
        std::string resultMsg;
        std::string resultType;
        float p1x = 0, p1y = 0, p1z = 0;
        float p2x = 0, p2y = 0, p2z = 0;

        float camTheta = 2.4f, camPhi = 0.5f, camDist = 10.0f;
        float camTx = 0, camTy = 0, camTz = 0;
    };

}
