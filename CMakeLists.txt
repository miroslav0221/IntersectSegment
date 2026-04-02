cmake_minimum_required(VERSION 3.29)
project(IntersectSegment)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/googletest)

add_library(intersect_core STATIC
        src/core/Intersect.cpp
        src/core/Segment3D.cpp
        src/core/Vector3D.cpp)
target_include_directories(intersect_core PUBLIC include)

add_executable(tests
        tests/tests_intersect.cpp
        tests/tests_segment3D.cpp)
target_link_libraries(tests intersect_core gtest gtest_main)

include(FetchContent)

FetchContent_Declare(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.91.8
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

add_executable(IntersectView
    view/main_view.cpp
    view/src/app.cpp
    view/src/compute.cpp
    view/src/panel.cpp
    view/src/render_2d.cpp
    view/src/render_3d.cpp
)
target_link_libraries(IntersectView intersect_core imgui)
