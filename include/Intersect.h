#ifndef INTERSECTSEGMENT_INTERSECT_H
#define INTERSECTSEGMENT_INTERSECT_H

#include <stdexcept>
#include <utility>
#include "Vector3D.h"
#include "Segment3D.h"

struct CollinearException : std::runtime_error {
    CollinearException() : std::runtime_error("Лежат на одной прямой") {}
};

struct CoordAccessor {
    double (Vector3D::*point)() const;
    double (Segment3D::*segment)() const;
};

extern const CoordAccessor COORD_X;
extern const CoordAccessor COORD_Y;
extern const CoordAccessor COORD_Z;

std::pair<double, double> intersect2d(const Segment3D &AB, const Segment3D &CD,
                                      CoordAccessor c1, CoordAccessor c2);

std::pair<Vector3D, Vector3D> intersect3d(const Segment3D &AB, const Segment3D &CD);

Vector3D multiple_vectors(const Segment3D & AB, const Segment3D & CD);

double dot_product(const Vector3D & a, const Vector3D & b);


#endif