#ifndef INTERSECTSEGMENT_INTERSECT_H
#define INTERSECTSEGMENT_INTERSECT_H

#include "Vector3D.h"
#include "Segment3D.h"

Vector3D intersect(const Segment3D &segment1, const Segment3D &segment2);

void intersect2d(Segment3D AB, Segment3D CD);

Vector3D multiple_vectors(Segment3D & AB, Segment3D & CD);


#endif