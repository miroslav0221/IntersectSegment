#include "view/Compute.h"

#include "сore/Intersect.h"
#include "сore/Segment3D.h"
#include "сore/Vector3D.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <exception>

namespace view {

void compute(State& s) {
    const float za = s.mode3d ? s.az : 0.0f;
    const float zb = s.mode3d ? s.bz : 0.0f;
    const float zc = s.mode3d ? s.cz : 0.0f;
    const float zd = s.mode3d ? s.dz : 0.0f;

    Vector3D A(s.ax, s.ay, za), B(s.bx, s.by, zb);
    Vector3D C(s.cx, s.cy, zc), D(s.dx, s.dy, zd);
    Segment3D AB(A, B), CD(C, D);

    try {
        auto [r1, r2] = intersect3d(AB, CD);
        s.computed = true;
        s.isError = false;
        s.p1x = static_cast<float>(r1.get_x());
        s.p1y = static_cast<float>(r1.get_y());
        s.p1z = static_cast<float>(r1.get_z());
        s.p2x = static_cast<float>(r2.get_x());
        s.p2y = static_cast<float>(r2.get_y());
        s.p2z = static_cast<float>(r2.get_z());

        const float d = (s.p2x - s.p1x) * (s.p2x - s.p1x) +
                        (s.p2y - s.p1y) * (s.p2y - s.p1y) +
                        (s.p2z - s.p1z) * (s.p2z - s.p1z);

        char buf[256];
        if (d < 1e-12f) {
            s.resultType = "point";
            if (s.mode3d) {
                std::snprintf(buf, sizeof(buf), u8"Точка пересечения\n(%.4f, %.4f, %.4f)", s.p1x, s.p1y, s.p1z);
            } else {
                std::snprintf(buf, sizeof(buf), u8"Точка пересечения\n(%.4f, %.4f)", s.p1x, s.p1y);
            }
        } else {
            s.resultType = "overlap";
            if (s.mode3d) {
                std::snprintf(
                    buf,
                    sizeof(buf),
                    u8"Отрезок пересечения\n(%.4f, %.4f, %.4f) \n(%.4f, %.4f, %.4f)",
                    s.p1x,
                    s.p1y,
                    s.p1z,
                    s.p2x,
                    s.p2y,
                    s.p2z);
            } else {
                std::snprintf(
                    buf,
                    sizeof(buf),
                    u8"Отрезок пересечения\n(%.4f, %.4f) \n(%.4f, %.4f)",
                    s.p1x,
                    s.p1y,
                    s.p2x,
                    s.p2y);
            }
        }
        s.resultMsg = buf;
    } catch (const std::exception& e) {
        s.computed = true;
        s.isError = true;
        s.resultType = "error";
        s.resultMsg = e.what();
    }
}

void fitCamera(State& s) {
    const float za = s.mode3d ? s.az : 0.0f;
    const float zb = s.mode3d ? s.bz : 0.0f;
    const float zc = s.mode3d ? s.cz : 0.0f;
    const float zd = s.mode3d ? s.dz : 0.0f;

    const float pts[][3] = {{s.ax, s.ay, za}, {s.bx, s.by, zb}, {s.cx, s.cy, zc}, {s.dx, s.dy, zd}};

    float cx = 0.0f;
    float cy = 0.0f;
    float cz = 0.0f;
    for (const auto& p : pts) {
        cx += p[0];
        cy += p[1];
        cz += p[2];
    }
    cx /= 4.0f;
    cy /= 4.0f;
    cz /= 4.0f;

    float maxR = 0.0f;
    for (const auto& p : pts) {
        const float r = std::sqrt((p[0] - cx) * (p[0] - cx) + (p[1] - cy) * (p[1] - cy) + (p[2] - cz) * (p[2] - cz));
        maxR = std::max(maxR, r);
    }
    s.camTx = cx;
    s.camTy = cy;
    s.camTz = cz;
    s.camDist = std::max(maxR * 3.5f, 5.0f);
}

}
