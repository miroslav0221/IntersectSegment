#ifndef INTERSECTSEGMENT_SEGMENT3D_H
#define INTERSECTSEGMENT_SEGMENT3D_H

#include "Vector3D.h"

class Segment3D {
    Vector3D start;
    Vector3D end;

    //Координаты вектора c началом в start и концом в end
    double x;
    double y;
    double z;

public:
    Segment3D(Vector3D& start, Vector3D& end);

    Vector3D& get_start();

    Vector3D& get_end();

    double get_x() const;

    double get_y() const;

    double get_z() const;


};


#endif
