#include <iostream>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include "Intersect.h"

const double EPS = 1e-9;

const CoordAccessor COORD_X = {&Vector3D::get_x, &Segment3D::get_x};
const CoordAccessor COORD_Y = {&Vector3D::get_y, &Segment3D::get_y};
const CoordAccessor COORD_Z = {&Vector3D::get_z, &Segment3D::get_z};

Vector3D multiple_vectors(const Segment3D & AB, const Segment3D & CD) {
    double x = AB.get_y() * CD.get_z() - AB.get_z() * CD.get_y();
    double y = AB.get_z() * CD.get_x() - AB.get_x() * CD.get_z();
    double z = AB.get_x() * CD.get_y() - AB.get_y() * CD.get_x();

    return Vector3D{x, y, z};
}

double dot_product(const Vector3D & a, const Vector3D & b) {
    return a.get_x() * b.get_x() + a.get_y() * b.get_y() + a.get_z() * b.get_z();
}

static std::pair<Vector3D, Vector3D> collinear_overlap(const Segment3D &AB, const Segment3D &CD) {
    double ab_sq = AB.get_x()*AB.get_x() + AB.get_y()*AB.get_y() + AB.get_z()*AB.get_z();

    if (ab_sq < EPS) {
        // AB вырожден (точка)
        double cd_sq = CD.get_x()*CD.get_x() + CD.get_y()*CD.get_y() + CD.get_z()*CD.get_z();
        if (cd_sq < EPS) {
            double dx = AB.get_start().get_x() - CD.get_start().get_x();
            double dy = AB.get_start().get_y() - CD.get_start().get_y();
            double dz = AB.get_start().get_z() - CD.get_start().get_z();
            if (dx*dx + dy*dy + dz*dz < EPS) {
                return {AB.get_start(), AB.get_start()};
            }
            throw std::runtime_error("Отрезки лежат на одной прямой и не пересекаются");
        }
        // Проверяем, лежит ли точка A на отрезке CD
        Segment3D CA(CD.get_start(), AB.get_start());
        double u = (CA.get_x()*CD.get_x() + CA.get_y()*CD.get_y() + CA.get_z()*CD.get_z()) / cd_sq;
        if (u >= -EPS and u <= 1 + EPS) {
            return {AB.get_start(), AB.get_start()};
        }
        throw std::runtime_error("Отрезки лежат на одной прямой и не пересекаются");
    }

    // Проецируем C и D на прямую AB: P(t) = A + t*(B-A), где t=0 → A, t=1 → B
    Segment3D AC(AB.get_start(), CD.get_start());
    Segment3D AD(AB.get_start(), CD.get_end());

    double t_C = (AC.get_x()*AB.get_x() + AC.get_y()*AB.get_y() + AC.get_z()*AB.get_z()) / ab_sq;
    double t_D = (AD.get_x()*AB.get_x() + AD.get_y()*AB.get_y() + AD.get_z()*AB.get_z()) / ab_sq;

    double cd_min = std::min(t_C, t_D);
    double cd_max = std::max(t_C, t_D);

    // Пересечение интервалов [0, 1] и [cd_min, cd_max]
    double overlap_start = std::max(0.0, cd_min);
    double overlap_end = std::min(1.0, cd_max);

    if (overlap_start > overlap_end + EPS) {
        throw std::runtime_error("Отрезки лежат на одной прямой и не пересекаются");
    }

    Vector3D p1(
        AB.get_start().get_x() + AB.get_x() * overlap_start,
        AB.get_start().get_y() + AB.get_y() * overlap_start,
        AB.get_start().get_z() + AB.get_z() * overlap_start
    );
    Vector3D p2(
        AB.get_start().get_x() + AB.get_x() * overlap_end,
        AB.get_start().get_y() + AB.get_y() * overlap_end,
        AB.get_start().get_z() + AB.get_z() * overlap_end
    );

    return {p1, p2};
}

std::pair<double, double> intersect2d(const Segment3D &AB, const Segment3D &CD,
                                      CoordAccessor c1, CoordAccessor c2) {
    Segment3D AC(AB.get_start(), CD.get_start());

    double A1 = (AB.*c1.segment)();
    double A2 = (AB.*c2.segment)();

    double B1 = (CD.*c1.segment)();
    double B2 = (CD.*c2.segment)();

    double C1 = (AC.*c1.segment)();
    double C2 = (AC.*c2.segment)();

    // Проверка на вырожденность проекции (отрезок проецируется в точку)
    if ((std::abs(A1) < EPS and std::abs(A2) < EPS) or
        (std::abs(B1) < EPS and std::abs(B2) < EPS)) {
        throw CollinearException();
    }

    double det_ab = -(A1 * B2 - A2 * B1);

    //Значит отрезки либо параллельны, либо лежат на одной прямой
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
    // Векторное произведение направляющих векторов
    Vector3D n = multiple_vectors(AB, CD);
    double n_squared = dot_product(n, n);

    Segment3D AC(AB.get_start(), CD.get_start());

    // Проверка параллельности (AB × CD ≈ 0)
    if (n_squared < EPS) {
        Vector3D ac_cross_ab = multiple_vectors(AC, AB);
        if (std::abs(ac_cross_ab.get_x()) < EPS and
            std::abs(ac_cross_ab.get_y()) < EPS and
            std::abs(ac_cross_ab.get_z()) < EPS) {
            return collinear_overlap(AB, CD);
        }
        throw std::runtime_error("Отрезки параллельны");
    }

    // Проверка компланарности (скалярное тройное произведение AC · (AB × CD))
    Vector3D ac_dir(AC.get_x(), AC.get_y(), AC.get_z());
    double scalar_triple = dot_product(ac_dir, n);
    if (std::abs(scalar_triple) > EPS) {
        throw std::runtime_error("Отрезки скрещиваются");
    }

    // Отрезки компланарны и не параллельны — ищем точку пересечения через проекции
    double result_x = 0, result_y = 0, result_z = 0;
    bool has_x = false, has_y = false, has_z = false;

    // XY проекция
    try {
        auto r = intersect2d(AB, CD, COORD_X, COORD_Y);
        result_x = r.first; has_x = true;
        result_y = r.second; has_y = true;
    } catch (const CollinearException &) {
    } catch (const std::runtime_error &) {
        throw std::runtime_error("Отрезки не пересекаются");
    }

    // XZ проекция
    try {
        auto r = intersect2d(AB, CD, COORD_X, COORD_Z);
        if (has_x and std::abs(result_x - r.first) > EPS) {
            throw std::runtime_error("Отрезки не пересекаются");
        }
        if (!has_x) { result_x = r.first; has_x = true; }
        result_z = r.second; has_z = true;
    } catch (const CollinearException &) {
    } catch (const std::runtime_error &) {
        throw std::runtime_error("Отрезки не пересекаются");
    }

    // YZ проекция
    try {
        auto r = intersect2d(AB, CD, COORD_Y, COORD_Z);
        if (has_y and std::abs(result_y - r.first) > EPS) {
            throw std::runtime_error("Отрезки не пересекаются");
        }
        if (has_z and std::abs(result_z - r.second) > EPS) {
            throw std::runtime_error("Отрезки не пересекаются");
        }
        if (!has_y) { result_y = r.first; has_y = true; }
        if (!has_z) { result_z = r.second; has_z = true; }
    } catch (const CollinearException &) {
    } catch (const std::runtime_error &) {
        throw std::runtime_error("Отрезки не пересекаются");
    }

    // Восстановление недостающих координат через параметрическое уравнение
    if (!has_x or !has_y or !has_z) {
        double t = 0;
        bool t_found = false;

        if (has_x and std::abs(AB.get_x()) > EPS) {
            t = (result_x - AB.get_start().get_x()) / AB.get_x();
            t_found = true;
        } else if (has_y and std::abs(AB.get_y()) > EPS) {
            t = (result_y - AB.get_start().get_y()) / AB.get_y();
            t_found = true;
        } else if (has_z and std::abs(AB.get_z()) > EPS) {
            t = (result_z - AB.get_start().get_z()) / AB.get_z();
            t_found = true;
        }

        if (!t_found) {
            throw std::runtime_error("Не удалось определить точку пересечения");
        }

        if (t < -EPS or t > 1 + EPS) {
            throw std::runtime_error("Отрезки не пересекаются");
        }

        if (!has_x) result_x = AB.get_start().get_x() + AB.get_x() * t;
        if (!has_y) result_y = AB.get_start().get_y() + AB.get_y() * t;
        if (!has_z) result_z = AB.get_start().get_z() + AB.get_z() * t;
    }

    // Проверка принадлежности точки второму отрезку
    double u = 0;
    bool u_found = false;

    if (std::abs(CD.get_x()) > EPS) {
        u = (result_x - CD.get_start().get_x()) / CD.get_x();
        u_found = true;
    } else if (std::abs(CD.get_y()) > EPS) {
        u = (result_y - CD.get_start().get_y()) / CD.get_y();
        u_found = true;
    } else if (std::abs(CD.get_z()) > EPS) {
        u = (result_z - CD.get_start().get_z()) / CD.get_z();
        u_found = true;
    }

    if (u_found) {
        if (u < -EPS or u > 1 + EPS) {
            throw std::runtime_error("Отрезки не пересекаются");
        }

        double verify_x = CD.get_start().get_x() + CD.get_x() * u;
        double verify_y = CD.get_start().get_y() + CD.get_y() * u;
        double verify_z = CD.get_start().get_z() + CD.get_z() * u;

        if (std::abs(result_x - verify_x) > EPS or
            std::abs(result_y - verify_y) > EPS or
            std::abs(result_z - verify_z) > EPS) {
            throw std::runtime_error("Отрезки не пересекаются");
        }
    }

    Vector3D point(result_x, result_y, result_z);
    return {point, point};
}
