#include <iostream>
#include "Intersect.h"

Vector3D multiple_vectors(Segment3D & AB, Segment3D & CD) {
    double x = AB.get_y() * CD.get_z() - AB.get_z() * CD.get_y();
    double y = AB.get_z() * CD.get_x() - AB.get_x() * CD.get_z();
    double z = AB.get_x() * CD.get_y() - AB.get_y() * CD.get_x();

    return Vector3D{x, y, z};
}

void intersect2d(Segment3D AB, Segment3D CD) {
    double u, t;

    Segment3D AC(AB.get_start(), CD.get_start());

    double A1 = AB.get_x();
    double A2 = AB.get_y();

    double B1 = CD.get_x();
    double B2 = CD.get_y();

    double C1 = AC.get_x();
    double C2 = AC.get_y();

    double det_ab, det_t, det_u;
    det_ab = -(A1 * B2 - A2 * B1);

    //Значит отрезки либо параллельны, либо лежат на одной прямой
    if (det_ab == 0) {
        Vector3D AB_CD = multiple_vectors(AC, AB);
        if (AB_CD.get_x() == 0 and AB_CD.get_y() == 0 and AB_CD.get_z() == 0) {
            std::cout << "Лежат на одной прямой" << std::endl;
        }
        else {
            std::cout << "Параллельные прямые" << std::endl;
        }
        return;
    }

    det_t = -(C1 * B2 - B1 * C2);
    det_u = -(C1 * A2 - C2 * A1);

    t = det_t / det_ab;
    u = det_u / det_ab;

    std::cout << "t = " << t << std::endl;
    std::cout << "u = " << u << std::endl;

    if (!(t <= 1 and u <= 1)) {
        std::cout << "Отрезки не пересекаются" << std::endl;
        return;
    }

    double x_intersect = AB.get_start().get_x() + AB.get_x() * t;
    double y_intersect = AB.get_start().get_y() + AB.get_y() * t;

    double x_intersect2 = CD.get_start().get_x() + CD.get_x() * u;
    double y_intersect2 = CD.get_start().get_y() + CD.get_y() * u;

    std::cout << "x = " << x_intersect << " y = " << y_intersect << std::endl;
    std::cout << "x = " << x_intersect2 << " y = " << y_intersect2 << std::endl;
}

Vector3D intersect(const Segment3D &segment1, const Segment3D &segment2) {
}
