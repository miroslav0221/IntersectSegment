#ifndef INTERSECTSEGMENT_VECTOR3D_H
#define INTERSECTSEGMENT_VECTOR3D_H



class Vector3D {
    double x;
    double y;
    double z;

public:
    Vector3D(double x, double y, double z);

    double get_x() const;

    double get_y() const;

    double get_z() const;

};



#endif
