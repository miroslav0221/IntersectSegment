#include <iostream>
#include <cmath>
#include <stdexcept>

#include "Intersect.h"
#include "Segment3D.h"
#include "Vector3D.h"

void print_result(const std::pair<Vector3D, Vector3D> &result) {
    double dx = result.first.get_x() - result.second.get_x();
    double dy = result.first.get_y() - result.second.get_y();
    double dz = result.first.get_z() - result.second.get_z();
    if (dx * dx + dy * dy + dz * dz < 1e-14) {
        std::cout << "Точка: (" << result.first.get_x() << ", "
                  << result.first.get_y() << ", " << result.first.get_z() << ")" << std::endl;
    } else {
        std::cout << "Отрезок пересечения: ("
                  << result.first.get_x() << ", " << result.first.get_y() << ", " << result.first.get_z()
                  << ") — ("
                  << result.second.get_x() << ", " << result.second.get_y() << ", " << result.second.get_z()
                  << ")" << std::endl;
    }
}


int main(int argc, char **argv) {

    // 2D тест: отрезки на одной прямой
    Vector3D A(1.0, 1.5, 0);
    Vector3D B(2.0, 1.5, 0);
    Vector3D C(3.0, 1.5, 0);
    Vector3D D(4.0, 1.5, 0);

    Segment3D AB(A, B);
    Segment3D CD(C, D);

    std::cout << "=== 2D (коллинеарные) ===" << std::endl;
    try {
        auto r = intersect2d(AB, CD, COORD_X, COORD_Y);
        std::cout << "Точка: (" << r.first << ", " << r.second << ")" << std::endl;
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }

    // 2D тест: пересекающиеся отрезки
    Vector3D A2(0, 0, 0);
    Vector3D B2(2, 2, 0);
    Vector3D C2(0, 2, 0);
    Vector3D D2(2, 0, 0);

    Segment3D AB2(A2, B2);
    Segment3D CD2(C2, D2);

    std::cout << "\n=== 2D (пересечение) ===" << std::endl;
    try {
        auto r = intersect2d(AB2, CD2, COORD_X, COORD_Y);
        std::cout << "Точка: (" << r.first << ", " << r.second << ")" << std::endl;
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }

    // 3D тест (intersect): пересекающиеся отрезки в плоскости z=0
    Vector3D A3(0, 0, 0);
    Vector3D B3(2, 2, 0);
    Vector3D C3(0, 2, 0);
    Vector3D D3(2, 0, 0);

    Segment3D AB3(A3, B3);
    Segment3D CD3(C3, D3);
    // 3D тест (intersect3d): пересечение в плоскости z=0
    std::cout << "\n=== 3D (intersect3d, z=0) ===" << std::endl;
    try {
        auto result = intersect3d(AB3, CD3);
        print_result(result);
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }

    // 3D тест (intersect3d): настоящее 3D пересечение
    Vector3D A5(0, 0, 0);
    Vector3D B5(2, 2, 2);
    Vector3D C5(2, 0, 0);
    Vector3D D5(0, 2, 2);

    Segment3D AB5(A5, B5);
    Segment3D CD5(C5, D5);

    std::cout << "\n=== 3D (intersect3d, настоящее 3D) ===" << std::endl;
    try {
        auto result = intersect3d(AB5, CD5);
        print_result(result);
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }

    // 3D тест: скрещивающиеся отрезки
    Vector3D A4(0, 0, 0);
    Vector3D B4(1, 0, 0);
    Vector3D C4(-2, 0, 0);
    Vector3D D4(2, 0, 0);

    Segment3D AB4(A4, B4);
    Segment3D CD4(C4, D4);

    std::cout << "\n=== 3D (intersect3d, Перекрываются) ===" << std::endl;
    try {
        auto result = intersect3d(AB4, CD4);
        print_result(result);
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    Vector3D A6(1, 1, 0);
    Vector3D B6(1, 1, 1);
    Vector3D C6(1, 1, 2);
    Vector3D D6(1, 1, 3);

    Segment3D AB6(A6, B6);
    Segment3D CD6(C6, D6);

    std::cout << "\n=== 3D (intersect3d, коллинеарные с пересечением) ===" << std::endl;
    try {
        auto result = intersect3d(AB6, CD6);
        print_result(result);
    } catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}