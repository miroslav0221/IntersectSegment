#include <iostream>
#include <cmath>
#include <algorithm>
#include <stdexcept>

#include "сore/Intersect.h"

const CoordAccessor COORD_X = {&Vector3D::get_x};
const CoordAccessor COORD_Y = {&Vector3D::get_y};
const CoordAccessor COORD_Z = {&Vector3D::get_z};

Vector3D multiple_vectors(const Vector3D &AB, const Vector3D &CD) {
    double x = AB.get_y() * CD.get_z() - AB.get_z() * CD.get_y();
    double y = AB.get_z() * CD.get_x() - AB.get_x() * CD.get_z();
    double z = AB.get_x() * CD.get_y() - AB.get_y() * CD.get_x();

    return Vector3D{x, y, z};
}

double scalar_multiple(const Vector3D &a, const Vector3D &b) {
    return a.get_x() * b.get_x() + a.get_y() * b.get_y() + a.get_z() * b.get_z();
}

//Функция для поиска отрезка пересечения отрезков, если известно, что они лежат на одной прямой. Если отрезки все же
//не пересекаются бросается исключение
static std::pair<Vector3D, Vector3D> collinear_overlap(const Segment3D &AB, const Segment3D &CD) {
    double ab_sq = AB.get_vector().get_x() * AB.get_vector().get_x() + AB.get_vector().get_y() * AB.get_vector().get_y() +
            AB.get_vector().get_z() * AB.get_vector().get_z();

    if (ab_sq < EPS) {
        double cd_sq = CD.get_vector().get_x() * CD.get_vector().get_x() +
                       CD.get_vector().get_y() * CD.get_vector().get_y() +
                       CD.get_vector().get_z() * CD.get_vector().get_z();
        if (cd_sq < EPS) {
            double dx = AB.get_start().get_x() - CD.get_start().get_x();
            double dy = AB.get_start().get_y() - CD.get_start().get_y();
            double dz = AB.get_start().get_z() - CD.get_start().get_z();
            if (dx * dx + dy * dy + dz * dz < EPS) {
                return {AB.get_start(), AB.get_start()};
            }
            throw std::runtime_error("Отрезки лежат на одной прямой и не пересекаются");
        }
        Segment3D CA(CD.get_start(), AB.get_start());
        double u = (CA.get_vector().get_x() * CD.get_vector().get_x() + CA.get_vector().get_y() * CD.get_vector().get_y() +
                 CA.get_vector().get_z() * CD.get_vector().get_z()) / cd_sq;
        if (u >= -EPS and u <= 1 + EPS) {
            return {AB.get_start(), AB.get_start()};
        }
        throw std::runtime_error("Отрезки лежат на одной прямой и не пересекаются");
    }

    Segment3D AC(AB.get_start(), CD.get_start());
    Segment3D AD(AB.get_start(), CD.get_end());

    double t_C =
            (AC.get_vector().get_x() * AB.get_vector().get_x() + AC.get_vector().get_y() * AB.get_vector().get_y() +
             AC.get_vector().get_z() * AB.get_vector().get_z()) / ab_sq;
    double t_D =
            (AD.get_vector().get_x() * AB.get_vector().get_x() + AD.get_vector().get_y() * AB.get_vector().get_y() +
             AD.get_vector().get_z() * AB.get_vector().get_z()) / ab_sq;

    double cd_min = std::min(t_C, t_D);
    double cd_max = std::max(t_C, t_D);

    double overlap_start = std::max(0.0, cd_min);
    double overlap_end = std::min(1.0, cd_max);

    if (overlap_start > overlap_end + EPS) {
        throw std::runtime_error("Отрезки лежат на одной прямой и не пересекаются");
    }

    Vector3D p1(
            AB.get_start().get_x() + AB.get_vector().get_x() * overlap_start,
            AB.get_start().get_y() + AB.get_vector().get_y() * overlap_start,
            AB.get_start().get_z() + AB.get_vector().get_z() * overlap_start
    );
    Vector3D p2(
            AB.get_start().get_x() + AB.get_vector().get_x() * overlap_end,
            AB.get_start().get_y() + AB.get_vector().get_y() * overlap_end,
            AB.get_start().get_z() + AB.get_vector().get_z() * overlap_end
    );

    return {p1, p2};
}

std::pair<double, double> intersect2d(const Segment3D &AB, const Segment3D &CD,
                                      CoordAccessor c1, CoordAccessor c2) {
    Segment3D AC(AB.get_start(), CD.get_start());

    double A1 = (AB.get_vector().*c1.point)();
    double A2 = (AB.get_vector().*c2.point)();

    double B1 = (CD.get_vector().*c1.point)();
    double B2 = (CD.get_vector().*c2.point)();

    double C1 = (AC.get_vector().*c1.point)();
    double C2 = (AC.get_vector().*c2.point)();

    if ((std::abs(A1) < EPS and std::abs(A2) < EPS) or
        (std::abs(B1) < EPS and std::abs(B2) < EPS)) {
        throw CollinearException();
    }

    double det_ab = -(A1 * B2 - A2 * B1);

    if (std::abs(det_ab) < EPS) {
        double cross_2d = C1 * A2 - C2 * A1;
        if (std::abs(cross_2d) < EPS) {
            throw CollinearException();
        }
        throw std::runtime_error("Параллельные прямые");
    }

    double det_t = -(C1 * B2 - B1 * C2);
    double det_u = -(C1 * A2 - C2 * A1);

    double t = det_t / det_ab;
    double u = det_u / det_ab;

    if (!(t >= 0 and t <= 1 and u >= 0 and u <= 1)) {
        throw std::runtime_error("Отрезки не пересекаются");
    }

    double coord1 = (AB.get_start().*c1.point)() + A1 * t;
    double coord2 = (AB.get_start().*c2.point)() + A2 * t;

    return std::make_pair(coord1, coord2);
}

std::pair<Vector3D, Vector3D> intersect3d(const Segment3D &AB, const Segment3D &CD) {

    Vector3D A = AB.get_start();
    Vector3D C = CD.get_start();

    Vector3D u(AB.get_vector().get_x(), AB.get_vector().get_y(), AB.get_vector().get_z());
    Vector3D v(CD.get_vector().get_x(), CD.get_vector().get_y(), CD.get_vector().get_z());

    Vector3D w(A.get_x() - C.get_x(),
               A.get_y() - C.get_y(),
               A.get_z() - C.get_z());

    Vector3D n = multiple_vectors(u, v);
    double n2 = scalar_multiple(n, n);

    if (n2 < EPS) {

        Vector3D cross = multiple_vectors(w, u);

        if (std::abs(cross.get_x()) < EPS &&
            std::abs(cross.get_y()) < EPS &&
            std::abs(cross.get_z()) < EPS) {

            return collinear_overlap(AB, CD);
        }

        throw std::runtime_error("Отрезки параллельны");
    }

    if (std::abs(scalar_multiple(w, n)) > EPS) {
        throw std::runtime_error("Отрезки скрещиваются");
    }

    Vector3D CA(C.get_x() - A.get_x(),
                C.get_y() - A.get_y(),
                C.get_z() - A.get_z());

    double t = scalar_multiple(multiple_vectors(CA, v), n) / n2;
    double s = scalar_multiple(multiple_vectors(CA, u), n) / n2;

    if (t < -EPS || t > 1 + EPS || s < -EPS || s > 1 + EPS) {
        throw std::runtime_error("Отрезки не пересекаются");
    }

    Vector3D point(
            A.get_x() + u.get_x() * t,
            A.get_y() + u.get_y() * t,
            A.get_z() + u.get_z() * t
    );

    return {point, point};
}