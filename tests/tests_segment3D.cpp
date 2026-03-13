#include <cmath>
#include <gtest/gtest.h>

#include "сore/Intersect.h"


TEST(Segment3D, SegmentCoord) {
    Segment3D AB(Vector3D(1, 1, 1), Vector3D(5, 5, 5));
    EXPECT_EQ(AB.get_vector().get_x(), 4);
    EXPECT_EQ(AB.get_vector().get_y(), 4);
    EXPECT_EQ(AB.get_vector().get_z(), 4);
}
