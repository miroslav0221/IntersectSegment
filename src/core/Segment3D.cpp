#include "Segment3D.h"

Segment3D::Segment3D(const Vector3D &start, const Vector3D &end)
        : start(start), end(end), segment_as_vector(end.get_x() - start.get_x(),
                                                    end.get_y() - start.get_y(),
                                                    end.get_z() - start.get_z()) {
}

const Vector3D &Segment3D::get_end() const {
    return end;
}

const Vector3D &Segment3D::get_start() const {
    return start;
}

const Vector3D &Segment3D::get_vector() const {
    return segment_as_vector;
}




