#include "Vector3D.h"

Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

double Vector3D::get_x() const {
    return x;
}

double Vector3D::get_y() const {
    return y;
}

double Vector3D::get_z() const {
    return z;
}
