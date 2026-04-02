#include "view/Render.h"

#include <algorithm>
#include <cmath>

namespace view {

namespace {

struct Scr {
    float x;
    float y;
    bool ok;
};

Scr project(float px, float py, float pz, const State& s, float vpX, float vpY, float vpW, float vpH) {
    const float ct = std::cos(s.camTheta);
    const float st = std::sin(s.camTheta);
    const float cp = std::cos(s.camPhi);
    const float sp = std::sin(s.camPhi);
    const float d = s.camDist;

    const float ex = s.camTx + d * cp * st;
    const float ey = s.camTy + d * sp;
    const float ez = s.camTz + d * cp * ct;

    const float fl = (d > 1e-4f) ? d : 1.0f;
    const float fx = (s.camTx - ex) / fl;
    const float fy = (s.camTy - ey) / fl;
    const float fz = (s.camTz - ez) / fl;

    float rx = -fz;
    float rz = fx;
    float rl = std::sqrt(rx * rx + rz * rz);
    if (rl < 1e-3f) {
        rx = 1.0f;
        rz = 0.0f;
        rl = 1.0f;
    }
    rx /= rl;
    rz /= rl;

    const float ux = -rz * fy;
    const float uy = rz * fx - rx * fz;
    const float uz = rx * fy;

    const float ddx = px - ex;
    const float ddy = py - ey;
    const float ddz = pz - ez;
    const float vx = ddx * rx + ddz * rz;
    const float vy = ddx * ux + ddy * uy + ddz * uz;
    const float vz = ddx * fx + ddy * fy + ddz * fz;

    if (vz < 0.1f) return {0.0f, 0.0f, false};

    constexpr float pi = 3.14159265f;
    const float fov = 25.0f * pi / 180.0f;
    const float f = 1.0f / std::tan(fov);
    const float sc = std::min(vpW, vpH) / 2.0f;

    return {vpX + vpW / 2.0f + (vx / vz) * f * sc, vpY + vpH / 2.0f - (vy / vz) * f * sc, true};
}

}

void render3D(ImDrawList* dl, State& s, float vpX, float vpY, float vpW, float vpH) {
    dl->PushClipRect({vpX, vpY}, {vpX + vpW, vpY + vpH}, true);

    const auto p3 = [&](float x, float y, float z) -> Scr { return project(x, y, z, s, vpX, vpY, vpW, vpH); };
    const auto ln = [&](float x1, float y1, float z1, float x2, float y2, float z2, ImU32 c, float w) {
        const auto a = p3(x1, y1, z1);
        const auto b = p3(x2, y2, z2);
        if (a.ok && b.ok) dl->AddLine({a.x, a.y}, {b.x, b.y}, c, w);
    };
    const auto dot = [&](float x, float y, float z, ImU32 c, float r) {
        const auto p = p3(x, y, z);
        if (p.ok) {
            dl->AddCircleFilled({p.x, p.y}, r, c);
            dl->AddCircle({p.x, p.y}, r, ui::COL_BG, 0, 1.5f);
        }
    };
    const auto txt = [&](float x, float y, float z, const char* t, ImU32 c) {
        const auto p = p3(x, y, z);
        if (p.ok) dl->AddText(nullptr, 14.0f, {p.x + 10.0f, p.y - 18.0f}, c, t);
    };

    constexpr int gridN = 10;
    for (int i = -gridN; i <= gridN; ++i) {
        ln(static_cast<float>(i), 0.0f, static_cast<float>(-gridN), static_cast<float>(i), 0.0f, static_cast<float>(gridN), ui::COL_GRID, 0.5f);
        ln(static_cast<float>(-gridN), 0.0f, static_cast<float>(i), static_cast<float>(gridN), 0.0f, static_cast<float>(i), ui::COL_GRID, 0.5f);
    }

    const float al = std::max(6.0f, s.camDist * 0.5f);
    ln(-al, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, ui::COL_DIM_AXIS, 1.0f);
    ln(0.0f, -al, 0.0f, 0.0f, 0.0f, 0.0f, ui::COL_DIM_AXIS, 1.0f);
    ln(0.0f, 0.0f, -al, 0.0f, 0.0f, 0.0f, ui::COL_DIM_AXIS, 1.0f);
    ln(0.0f, 0.0f, 0.0f, al, 0.0f, 0.0f, ui::COL_AXIS_X, 1.5f);
    ln(0.0f, 0.0f, 0.0f, 0.0f, al, 0.0f, ui::COL_AXIS_Y, 1.5f);
    ln(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, al, ui::COL_AXIS_Z, 1.5f);
    txt(al + 0.4f, 0.0f, 0.0f, "X", ui::COL_AXIS_X);
    txt(0.0f, al + 0.4f, 0.0f, "Y", ui::COL_AXIS_Y);
    txt(0.0f, 0.0f, al + 0.4f, "Z", ui::COL_AXIS_Z);

    ln(s.ax, s.ay, s.az, s.bx, s.by, s.bz, ui::COL_AB, 3.0f);
    ln(s.cx, s.cy, s.cz, s.dx, s.dy, s.dz, ui::COL_CD, 3.0f);

    if (s.computed && !s.isError) {
        if (s.resultType == "point") {
            dot(s.p1x, s.p1y, s.p1z, ui::COL_HIT, 9.0f);
        } else if (s.resultType == "overlap") {
            ln(s.p1x, s.p1y, s.p1z, s.p2x, s.p2y, s.p2z, ui::COL_HIT, 5.0f);
            dot(s.p1x, s.p1y, s.p1z, ui::COL_HIT, 7.0f);
            dot(s.p2x, s.p2y, s.p2z, ui::COL_HIT, 7.0f);
        }
    }

    dot(s.ax, s.ay, s.az, ui::COL_AB, 5.0f);
    dot(s.bx, s.by, s.bz, ui::COL_AB, 5.0f);
    dot(s.cx, s.cy, s.cz, ui::COL_CD, 5.0f);
    dot(s.dx, s.dy, s.dz, ui::COL_CD, 5.0f);
    txt(s.ax, s.ay, s.az, "A", ui::COL_AB);
    txt(s.bx, s.by, s.bz, "B", ui::COL_AB);
    txt(s.cx, s.cy, s.cz, "C", ui::COL_CD);
    txt(s.dx, s.dy, s.dz, "D", ui::COL_CD);

    dl->PopClipRect();
}

}
