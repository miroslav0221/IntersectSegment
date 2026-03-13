#ifndef INTERSECTSEGMENT_SEGMENT3D_H
#define INTERSECTSEGMENT_SEGMENT3D_H

#include "Vector3D.h"

class Segment3D {
    Vector3D start;
    Vector3D end;

    //Координаты вектора c началом в start и концом в end
    Vector3D segment_as_vector;

public:
    Segment3D(const Vector3D &start, const Vector3D &end);

    const Vector3D &get_start() const;

    const Vector3D &get_end() const;

    const Vector3D &get_vector() const;
};


#endif
