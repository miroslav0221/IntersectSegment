#include "Segment3D.h"
#include <iostream>

Segment3D::Segment3D(Vector3D &start, Vector3D &end) : start(start), end(end) {
    x = end.get_x() - start.get_x();
    y = end.get_y() - start.get_y();
    z = end.get_z() - start.get_z();
}

Vector3D & Segment3D::get_end() {
    return static_cast<Vector3D &>(end);
}

Vector3D& Segment3D::get_start() {
    return static_cast<Vector3D &>(start);
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



