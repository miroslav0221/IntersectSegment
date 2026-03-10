#include "Segment3D.h"

Segment3D::Segment3D(const Vector3D &start, const Vector3D &end) : start(start), end(end) {
    x = end.get_x() - start.get_x();
    y = end.get_y() - start.get_y();
    z = end.get_z() - start.get_z();
}

const Vector3D& Segment3D::get_end() const {
    return end;
}

const Vector3D& Segment3D::get_start() const {
    return start;
}

double Segment3D::get_x() const {
    return x;
}

double Segment3D::get_y() const {
    return y;
}

double Segment3D::get_z() const {
    return z;
}



