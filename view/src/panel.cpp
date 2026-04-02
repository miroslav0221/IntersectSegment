#include "view/Panel.h"

#include "view/Compute.h"

namespace view {

void panel(State& s, float height) {
    ImGui::SetNextWindowPos({0.0f, 0.0f});
    ImGui::SetNextWindowSize({ui::PANEL_W, height});
    ImGui::Begin(
        "##panel",
        nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

    ImGui::TextColored({0.94f, 0.96f, 0.99f, 1.0f}, u8"Пересечение отрезков");
    ImGui::Spacing();

    if (ImGui::RadioButton("2D", !s.mode3d)) {
        s.mode3d = false;
        s.computed = false;
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("3D", s.mode3d)) {
        s.mode3d = true;
        s.computed = false;
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    const float fieldWidth = s.mode3d ? 70.0f : 110.0f;

    const auto pointRow = [&](const char* label,
                              ImVec4 color,
                              const char* idX,
                              const char* idY,
                              const char* idZ,
                              float& vx,
                              float& vy,
                              float& vz) {
        ImGui::TextColored(color, "%s", label);
        ImGui::PushItemWidth(fieldWidth);
        ImGui::InputFloat(idX, &vx, 0, 0, "%.2f");
        ImGui::SameLine();
        ImGui::InputFloat(idY, &vy, 0, 0, "%.2f");
        if (s.mode3d) {
            ImGui::SameLine();
            ImGui::InputFloat(idZ, &vz, 0, 0, "%.2f");
        }
        ImGui::PopItemWidth();
        ImGui::Spacing();
    };

    const ImVec4 colAB = {0.345f, 0.651f, 1.0f, 1.0f};
    const ImVec4 colCD = {0.824f, 0.6f, 0.133f, 1.0f};

    pointRow(u8"ТОЧКА A", colAB, "##ax", "##ay", "##az", s.ax, s.ay, s.az);
    pointRow(u8"ТОЧКА B", colAB, "##bx", "##by", "##bz", s.bx, s.by, s.bz);
    pointRow(u8"ТОЧКА C", colCD, "##cx", "##cy", "##cz", s.cx, s.cy, s.cz);
    pointRow(u8"ТОЧКА D", colCD, "##dx", "##dy", "##dz", s.dx, s.dy, s.dz);

    ImGui::PushStyleColor(ImGuiCol_Button, {0.137f, 0.525f, 0.212f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {0.180f, 0.627f, 0.263f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, {0.137f, 0.525f, 0.212f, 1.0f});
    if (ImGui::Button(u8"Запуск", {-1.0f, 36.0f})) {
        compute(s);
        if (s.mode3d) fitCamera(s);
    }
    ImGui::PopStyleColor(3);
    ImGui::Spacing();

    if (s.computed) {
        const ImVec4 col = s.isError ? ImVec4(0.973f, 0.322f, 0.286f, 1.0f) : ImVec4(0.247f, 0.725f, 0.314f, 1.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, col);
        ImGui::TextWrapped("%s", s.resultMsg.c_str());
        ImGui::PopStyleColor();
    } else {
        ImGui::TextColored({0.545f, 0.580f, 0.624f, 1.0f}, u8"Нажмите «Запуск»");
    }

    ImGui::PopFont();
    ImGui::End();
}

}
