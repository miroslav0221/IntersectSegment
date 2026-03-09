#include <iostream>

#include "Intersect.h"
#include "Segment3D.h"
#include "Vector3D.h"


int main(int argc, char** argv) {

    Vector3D A(1.0, 1.5, 0);
    Vector3D B(2.0, 1.5, 0);
    Vector3D C(3.0, 1.5, 0);
    Vector3D D(4.0, 1.5, 0);


    Segment3D AB(A, B);
    Segment3D CD(C, D);

    intersect2d(AB, CD);

    return 0;
}