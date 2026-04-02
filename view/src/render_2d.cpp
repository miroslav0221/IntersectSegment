#include "view/Render.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <vector>

namespace view {

namespace {

float niceStep(float range) {
    const float rough = range / 8.0f;
    const float p = std::pow(10.0f, std::floor(std::log10(rough)));
    const float n = rough / p;
    if (n < 1.5f) return p;
    if (n < 3.5f) return 2.0f * p;
    if (n < 7.5f) return 5.0f * p;
    return 10.0f * p;
}

}

void render2D(ImDrawList* dl, State& s, float vpX, float vpY, float vpW, float vpH) {
    struct Point {
        float x;
        float y;
    };

    std::vector<Point> all = {{s.ax, s.ay}, {s.bx, s.by}, {s.cx, s.cy}, {s.dx, s.dy}};
    if (s.computed && !s.isError) {
        all.push_back({s.p1x, s.p1y});
        all.push_back({s.p2x, s.p2y});
    }

    float xMin = 1e9f, xMax = -1e9f, yMin = 1e9f, yMax = -1e9f;
    for (const auto& p : all) {
        xMin = std::min(xMin, p.x);
        xMax = std::max(xMax, p.x);
        yMin = std::min(yMin, p.y);
        yMax = std::max(yMax, p.y);
    }

    if (xMax - xMin < 0.01f) {
        xMin -= 1.0f;
        xMax += 1.0f;
    }
    if (yMax - yMin < 0.01f) {
        yMin -= 1.0f;
        yMax += 1.0f;
    }

    const float px = (xMax - xMin) * 0.25f;
    const float py = (yMax - yMin) * 0.25f;
    xMin -= px;
    xMax += px;
    yMin -= py;
    yMax += py;

    const float sc = std::min(vpW / (xMax - xMin), vpH / (yMax - yMin));
    const float ox = (vpW - (xMax - xMin) * sc) / 2.0f;
    const float oy = (vpH - (yMax - yMin) * sc) / 2.0f;

    const auto tx = [&](float x) { return vpX + ox + (x - xMin) * sc; };
    const auto ty = [&](float y) { return vpY + vpH - oy - (y - yMin) * sc; };

    dl->PushClipRect({vpX, vpY}, {vpX + vpW, vpY + vpH}, true);

    const float step = niceStep(std::max(xMax - xMin, yMax - yMin));
    for (float gx = std::floor(xMin / step) * step; gx <= xMax; gx += step) {
        dl->AddLine({tx(gx), vpY}, {tx(gx), vpY + vpH}, ui::COL_GRID);
    }
    for (float gy = std::floor(yMin / step) * step; gy <= yMax; gy += step) {
        dl->AddLine({vpX, ty(gy)}, {vpX + vpW, ty(gy)}, ui::COL_GRID);
    }

    if (xMin <= 0.0f && xMax >= 0.0f) dl->AddLine({tx(0.0f), vpY}, {tx(0.0f), vpY + vpH}, ui::COL_AXIS, 1.5f);
    if (yMin <= 0.0f && yMax >= 0.0f) dl->AddLine({vpX, ty(0.0f)}, {vpX + vpW, ty(0.0f)}, ui::COL_AXIS, 1.5f);

    const int prec = step < 1.0f ? std::max(1, -static_cast<int>(std::floor(std::log10(step)))) : 0;
    char buf[32];
    for (float gx = std::floor(xMin / step) * step; gx <= xMax; gx += step) {
        std::snprintf(buf, sizeof(buf), "%.*f", prec, gx);
        dl->AddText({tx(gx) + 3.0f, vpY + vpH - 16.0f}, ui::COL_LABEL, buf);
    }
    for (float gy = std::floor(yMin / step) * step; gy <= yMax; gy += step) {
        std::snprintf(buf, sizeof(buf), "%.*f", prec, gy);
        dl->AddText({vpX + 4.0f, ty(gy) - 14.0f}, ui::COL_LABEL, buf);
    }

    dl->AddLine({tx(s.ax), ty(s.ay)}, {tx(s.bx), ty(s.by)}, ui::COL_AB, 2.5f);
    dl->AddLine({tx(s.cx), ty(s.cy)}, {tx(s.dx), ty(s.dy)}, ui::COL_CD, 2.5f);

    if (s.computed && !s.isError) {
        if (s.resultType == "point") {
            dl->AddCircleFilled({tx(s.p1x), ty(s.p1y)}, 8.0f, ui::COL_HIT);
            dl->AddCircle({tx(s.p1x), ty(s.p1y)}, 8.0f, ui::COL_BG, 0, 1.5f);
        } else if (s.resultType == "overlap") {
            dl->AddLine({tx(s.p1x), ty(s.p1y)}, {tx(s.p2x), ty(s.p2y)}, ui::COL_HIT, 5.0f);
            dl->AddCircleFilled({tx(s.p1x), ty(s.p1y)}, 6.0f, ui::COL_HIT);
            dl->AddCircleFilled({tx(s.p2x), ty(s.p2y)}, 6.0f, ui::COL_HIT);
        }
    }

    dl->AddCircleFilled({tx(s.ax), ty(s.ay)}, 5.0f, ui::COL_AB);
    dl->AddCircle({tx(s.ax), ty(s.ay)}, 5.0f, ui::COL_BG, 0, 1.5f);
    dl->AddCircleFilled({tx(s.bx), ty(s.by)}, 5.0f, ui::COL_AB);
    dl->AddCircle({tx(s.bx), ty(s.by)}, 5.0f, ui::COL_BG, 0, 1.5f);
    dl->AddCircleFilled({tx(s.cx), ty(s.cy)}, 5.0f, ui::COL_CD);
    dl->AddCircle({tx(s.cx), ty(s.cy)}, 5.0f, ui::COL_BG, 0, 1.5f);
    dl->AddCircleFilled({tx(s.dx), ty(s.dy)}, 5.0f, ui::COL_CD);
    dl->AddCircle({tx(s.dx), ty(s.dy)}, 5.0f, ui::COL_BG, 0, 1.5f);

    dl->AddText(nullptr, 14.0f, {tx(s.ax) + 10.0f, ty(s.ay) - 18.0f}, ui::COL_AB, "A");
    dl->AddText(nullptr, 14.0f, {tx(s.bx) + 10.0f, ty(s.by) - 18.0f}, ui::COL_AB, "B");
    dl->AddText(nullptr, 14.0f, {tx(s.cx) + 10.0f, ty(s.cy) - 18.0f}, ui::COL_CD, "C");
    dl->AddText(nullptr, 14.0f, {tx(s.dx) + 10.0f, ty(s.dy) - 18.0f}, ui::COL_CD, "D");

    dl->PopClipRect();
}

}
