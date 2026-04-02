#include "view/App.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "view/Panel.h"
#include "view/Render.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include <GLFW/glfw3.h>

#include <algorithm>

namespace view {

int runViewApp() {
    if (!glfwInit()) return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);

    GLFWwindow* win = glfwCreateWindow(1200, 700, "IntersectSegment", nullptr, nullptr);
    if (!win) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    const ImWchar* ranges = io.Fonts->GetGlyphRangesCyrillic();
    const char* fontPaths[] = {
        "/System/Library/Fonts/Supplemental/Arial.ttf",
        "/System/Library/Fonts/SFNS.ttf",
        "/Library/Fonts/Arial.ttf",
        "/System/Library/Fonts/Supplemental/Helvetica.ttf",
        nullptr};
    bool fontOk = false;
    for (int i = 0; fontPaths[i]; ++i) {
        if (io.Fonts->AddFontFromFileTTF(fontPaths[i], 15.0f, nullptr, ranges)) {
            fontOk = true;
            break;
        }
    }
    if (!fontOk) io.Fonts->AddFontDefault();

    ImGui::StyleColorsDark();
    ImGuiStyle& st = ImGui::GetStyle();
    st.WindowRounding = 0;
    st.FrameRounding = 6;
    st.GrabRounding = 4;
    st.WindowBorderSize = 0;
    st.FrameBorderSize = 1;

    auto& c = st.Colors;
    c[ImGuiCol_WindowBg] = {0.086f, 0.106f, 0.133f, 1};
    c[ImGuiCol_FrameBg] = {0.051f, 0.067f, 0.090f, 1};
    c[ImGuiCol_FrameBgHovered] = {0.071f, 0.087f, 0.110f, 1};
    c[ImGuiCol_FrameBgActive] = {0.091f, 0.107f, 0.130f, 1};
    c[ImGuiCol_Border] = {0.188f, 0.212f, 0.239f, 1};
    c[ImGuiCol_Text] = {0.788f, 0.820f, 0.851f, 1};
    c[ImGuiCol_CheckMark] = {0.220f, 0.545f, 0.992f, 1};
    c[ImGuiCol_SliderGrab] = {0.220f, 0.545f, 0.992f, 1};

    ImGui_ImplGlfw_InitForOpenGL(win, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    State state;

    while (!glfwWindowShouldClose(win)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int ww = 0;
        int wh = 0;
        glfwGetWindowSize(win, &ww, &wh);
        const float width = static_cast<float>(ww);
        const float height = static_cast<float>(wh);

        panel(state, height);

        const float vpX = ui::PANEL_W;
        const float vpY = 0.0f;
        const float vpW = width - ui::PANEL_W;
        const float vpH = height;

        ImGui::SetNextWindowPos({vpX, vpY});
        ImGui::SetNextWindowSize({vpW, vpH});
        ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.051f, 0.067f, 0.090f, 1});
        ImGui::Begin(
            "##viewport",
            nullptr,
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        const ImVec2 vPos = ImGui::GetCursorScreenPos();
        const ImVec2 vSize = ImGui::GetContentRegionAvail();
        ImGui::InvisibleButton("##vp", vSize, ImGuiButtonFlags_MouseButtonLeft);

        if (state.mode3d && state.computed) {
            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                state.camTheta -= io.MouseDelta.x * 0.008f;
                state.camPhi = std::clamp(state.camPhi + io.MouseDelta.y * 0.008f, -1.5f, 1.5f);
            }
            if (ImGui::IsItemHovered() && io.MouseWheel != 0) {
                state.camDist *= io.MouseWheel > 0 ? 0.9f : 1.1f;
                state.camDist = std::clamp(state.camDist, 0.5f, 200.0f);
            }
        }

        ImDrawList* dl = ImGui::GetWindowDrawList();
        if (state.computed) {
            if (state.mode3d) {
                render3D(dl, state, vPos.x, vPos.y, vSize.x, vSize.y);
            } else {
                render2D(dl, state, vPos.x, vPos.y, vSize.x, vSize.y);
            }
        }

        if (state.mode3d && state.computed) {
            const char* hint = u8"Перетаскивайте для вращения · Колесо для масштаба";
            const ImVec2 ts = ImGui::CalcTextSize(hint);
            dl->AddText({vPos.x + vSize.x / 2.0f - ts.x / 2.0f, vPos.y + vSize.y - 24.0f}, ui::COL_LABEL, hint);
        }

        ImGui::End();
        ImGui::PopStyleColor();

        ImGui::GetForegroundDrawList()->AddLine(
            {ui::PANEL_W, 0.0f}, {ui::PANEL_W, height}, IM_COL32(48, 54, 61, 255), 1.0f);

        ImGui::Render();
        int fbW = 0;
        int fbH = 0;
        glfwGetFramebufferSize(win, &fbW, &fbH);
        glViewport(0, 0, fbW, fbH);
        glClearColor(0.051f, 0.067f, 0.090f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(win);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}

}
