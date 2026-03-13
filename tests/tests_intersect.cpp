#include <cmath>
#include <gtest/gtest.h>

#include "Intersect.h"

const double EPS_TEST = 1e-7;

TEST(Intersect3d, IntersectionInXYPlane) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(2, 2, 0));
    Segment3D CD(Vector3D(0, 2, 0), Vector3D(2, 0, 0));
    auto [p1, p2] = intersect3d(AB, CD);
    EXPECT_NEAR(p1.get_x(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_y(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_z(), 0.0, EPS_TEST);
    EXPECT_NEAR(p1.get_x(), p2.get_x(), EPS_TEST);
    EXPECT_NEAR(p1.get_y(), p2.get_y(), EPS_TEST);
    EXPECT_NEAR(p1.get_z(), p2.get_z(), EPS_TEST);
}

TEST(Intersect3d, IntersectionTrue3D) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(2, 2, 2));
    Segment3D CD(Vector3D(2, 0, 0), Vector3D(0, 2, 2));
    auto [p1, p2] = intersect3d(AB, CD);
    EXPECT_NEAR(p1.get_x(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_y(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_z(), 1.0, EPS_TEST);
}

TEST(Intersect3d, IntersectionAtEndpoint) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, 1, 1));
    Segment3D CD(Vector3D(1, 1, 1), Vector3D(2, 0, 0));
    auto [p1, p2] = intersect3d(AB, CD);
    EXPECT_NEAR(p1.get_x(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_y(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_z(), 1.0, EPS_TEST);
}

TEST(Intersect3d, IntersectionAtOrigin) {
    Segment3D AB(Vector3D(-1, -1, 0), Vector3D(1, 1, 0));
    Segment3D CD(Vector3D(-1, 1, 0), Vector3D(1, -1, 0));
    auto [p1, p2] = intersect3d(AB, CD);
    EXPECT_NEAR(p1.get_x(), 0.0, EPS_TEST);
    EXPECT_NEAR(p1.get_y(), 0.0, EPS_TEST);
}

TEST(Intersect3d, IntersectionAlongAxes) {
    Segment3D AB(Vector3D(-1, 0, 0), Vector3D(1, 0, 0));
    Segment3D CD(Vector3D(0, -1, 0), Vector3D(0, 1, 0));
    auto [p1, p2] = intersect3d(AB, CD);
    EXPECT_NEAR(p1.get_x(), 0.0, EPS_TEST);
    EXPECT_NEAR(p1.get_y(), 0.0, EPS_TEST);
    EXPECT_NEAR(p1.get_z(), 0.0, EPS_TEST);
}

TEST(Intersect3d, IntersectionInXZPlane) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(2, 0, 2));
    Segment3D CD(Vector3D(2, 0, 0), Vector3D(0, 0, 2));
    auto [p1, p2] = intersect3d(AB, CD);
    EXPECT_NEAR(p1.get_x(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_y(), 0.0, EPS_TEST);
    EXPECT_NEAR(p1.get_z(), 1.0, EPS_TEST);
}

TEST(Intersect3d, IntersectionInYZPlane) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(0, 2, 2));
    Segment3D CD(Vector3D(0, 2, 0), Vector3D(0, 0, 2));
    auto [p1, p2] = intersect3d(AB, CD);
    EXPECT_NEAR(p1.get_x(), 0.0, EPS_TEST);
    EXPECT_NEAR(p1.get_y(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_z(), 1.0, EPS_TEST);
}

// Параллельные

TEST(Intersect3d, Parallel3D) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, 1, 1));
    Segment3D CD(Vector3D(1, 0, 0), Vector3D(2, 1, 1));
    EXPECT_THROW({
                     try { intersect3d(AB, CD); }
                     catch (const std::runtime_error &e) {
                         EXPECT_STREQ(e.what(), "Отрезки параллельны");
                         throw;
                     }
                 }, std::runtime_error);
}

TEST(Intersect3d, ParallelInXYPlane) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, 0, 0));
    Segment3D CD(Vector3D(0, 1, 0), Vector3D(1, 1, 0));
    EXPECT_THROW({
                     try { intersect3d(AB, CD); }
                     catch (const std::runtime_error &e) {
                         EXPECT_STREQ(e.what(), "Отрезки параллельны");
                         throw;
                     }
                 }, std::runtime_error);
}

TEST(Intersect3d, ParallelOppositeDirection) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(2, 2, 2));
    Segment3D CD(Vector3D(3, 1, 1), Vector3D(1, -1, -1));
    EXPECT_THROW({
                     try { intersect3d(AB, CD); }
                     catch (const std::runtime_error &e) {
                         EXPECT_STREQ(e.what(), "Отрезки параллельны");
                         throw;
                     }
                 }, std::runtime_error);
}

// Коллинеарные: не пересекаются

TEST(Intersect3d, CollinearDisjoint) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, 1, 1));
    Segment3D CD(Vector3D(3, 3, 3), Vector3D(4, 4, 4));
    EXPECT_THROW({
                     try { intersect3d(AB, CD); }
                     catch (const std::runtime_error &e) {
                         EXPECT_STREQ(e.what(), "Отрезки лежат на одной прямой и не пересекаются");
                         throw;
                     }
                 }, std::runtime_error);
}

TEST(Intersect3d, CollinearDisjointAlongX) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, 0, 0));
    Segment3D CD(Vector3D(5, 0, 0), Vector3D(6, 0, 0));
    EXPECT_THROW({
                     try { intersect3d(AB, CD); }
                     catch (const std::runtime_error &e) {
                         EXPECT_STREQ(e.what(), "Отрезки лежат на одной прямой и не пересекаются");
                         throw;
                     }
                 }, std::runtime_error);
}

// Коллинеарные: пересечение — отрезок

TEST(Collinear_overlap, LengthOneSegmentLessEPS) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(0.00001, 0.00001, 0.00001));
    Segment3D CD(Vector3D(1, 1, 1), Vector3D(2, 2, 2));

    EXPECT_THROW(
            {
                try {
                    intersect3d(AB, CD);
                }
                catch (const std::runtime_error &e) {
                    EXPECT_STREQ(e.what(), "Отрезки лежат на одной прямой и не пересекаются");
                    throw;
                }
            }, std::runtime_error);
}

TEST(Collinear_overlap, LengthTwoSegmentLessEPS) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(0.00001, 0.00001, 0.00001));
    Segment3D CD(Vector3D(1, 1, 1), Vector3D(1.00001, 1.00001, 1.00001));

    EXPECT_THROW(
            {
                try {
                    intersect3d(AB, CD);
                }
                catch (const std::runtime_error &e) {
                    EXPECT_STREQ(e.what(), "Отрезки лежат на одной прямой и не пересекаются");
                    throw;
                }
            }, std::runtime_error);
}

TEST(Collinear_overlap, StartOnePoint) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(0.00001, 0.00001, 0.00001));
    Segment3D CD(Vector3D(0, 0, 0), Vector3D(0.000001, 0.000001, 0.000001));

    auto point = intersect3d(AB, CD);
    EXPECT_NEAR(point.first.get_x(), point.second.get_x(), EPS_TEST);
    EXPECT_NEAR(point.first.get_y(), point.second.get_y(), EPS_TEST);
    EXPECT_NEAR(point.first.get_z(), point.second.get_z(), EPS_TEST);
    EXPECT_NEAR(point.first.get_x(), 0, EPS_TEST);
    EXPECT_NEAR(point.first.get_y(), 0, EPS_TEST);
    EXPECT_NEAR(point.first.get_z(), 0, EPS_TEST);
}

TEST(Collinear_overlap, LengthOneSegmentLessEPSAndSegmentsIntersect) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(0.00001, 0.00001, 0.00001));
    Segment3D CD(Vector3D(0, 0, 0), Vector3D(3, 3 ,3));

    auto point = intersect3d(AB, CD);
    EXPECT_NEAR(point.first.get_x(), point.second.get_x(), EPS_TEST);
    EXPECT_NEAR(point.first.get_y(), point.second.get_y(), EPS_TEST);
    EXPECT_NEAR(point.first.get_z(), point.second.get_z(), EPS_TEST);
    EXPECT_NEAR(point.first.get_x(), 0, EPS_TEST);
    EXPECT_NEAR(point.first.get_y(), 0, EPS_TEST);
    EXPECT_NEAR(point.first.get_z(), 0, EPS_TEST);
}







TEST(Intersect3d, CollinearOverlapping) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(2, 2, 2));
    Segment3D CD(Vector3D(1, 1, 1), Vector3D(3, 3, 3));
    auto [p1, p2] = intersect3d(AB, CD);
    EXPECT_NEAR(p1.get_x(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_y(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_z(), 1.0, EPS_TEST);
    EXPECT_NEAR(p2.get_x(), 2.0, EPS_TEST);
    EXPECT_NEAR(p2.get_y(), 2.0, EPS_TEST);
    EXPECT_NEAR(p2.get_z(), 2.0, EPS_TEST);
}

TEST(Intersect3d, CollinearOneContainsOther) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(4, 4, 4));
    Segment3D CD(Vector3D(1, 1, 1), Vector3D(2, 2, 2));
    auto [p1, p2] = intersect3d(AB, CD);
    EXPECT_NEAR(p1.get_x(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_y(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_z(), 1.0, EPS_TEST);
    EXPECT_NEAR(p2.get_x(), 2.0, EPS_TEST);
    EXPECT_NEAR(p2.get_y(), 2.0, EPS_TEST);
    EXPECT_NEAR(p2.get_z(), 2.0, EPS_TEST);
}

TEST(Intersect3d, CollinearOverlapAlongZ) {
    Segment3D AB(Vector3D(1, 1, 0), Vector3D(1, 1, 1));
    Segment3D CD(Vector3D(1, 1, 0.5), Vector3D(1, 1, 3));
    auto [p1, p2] = intersect3d(AB, CD);
    EXPECT_NEAR(p1.get_x(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_y(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_z(), 0.5, EPS_TEST);
    EXPECT_NEAR(p2.get_x(), 1.0, EPS_TEST);
    EXPECT_NEAR(p2.get_y(), 1.0, EPS_TEST);
    EXPECT_NEAR(p2.get_z(), 1.0, EPS_TEST);
}

TEST(Intersect3d, CollinearOppositeDirectionOverlap) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(3, 3, 3));
    Segment3D CD(Vector3D(2, 2, 2), Vector3D(1, 1, 1));
    auto [p1, p2] = intersect3d(AB, CD);
    EXPECT_NEAR(p1.get_x(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_y(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_z(), 1.0, EPS_TEST);
    EXPECT_NEAR(p2.get_x(), 2.0, EPS_TEST);
    EXPECT_NEAR(p2.get_y(), 2.0, EPS_TEST);
    EXPECT_NEAR(p2.get_z(), 2.0, EPS_TEST);
}

// Коллинеарные: касание в одной точке

TEST(Intersect3d, CollinearTouchAtEndpoint) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, 1, 1));
    Segment3D CD(Vector3D(1, 1, 1), Vector3D(2, 2, 2));
    auto [p1, p2] = intersect3d(AB, CD);
    EXPECT_NEAR(p1.get_x(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_y(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_z(), 1.0, EPS_TEST);
    EXPECT_NEAR(p1.get_x(), p2.get_x(), EPS_TEST);
    EXPECT_NEAR(p1.get_y(), p2.get_y(), EPS_TEST);
    EXPECT_NEAR(p1.get_z(), p2.get_z(), EPS_TEST);
}

// Скрещивающиеся

TEST(Intersect3d, Skew) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, 0, 0));
    Segment3D CD(Vector3D(0, 1, 1), Vector3D(0, 1, -1));
    EXPECT_THROW({
                     try { intersect3d(AB, CD); }
                     catch (const std::runtime_error &e) {
                         EXPECT_STREQ(e.what(), "Отрезки скрещиваются");
                         throw;
                     }
                 }, std::runtime_error);
}

TEST(Intersect3d, SkewCloseButNotIntersecting) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, 0, 0));
    Segment3D CD(Vector3D(0.5, 0, 1), Vector3D(0.5, 1, 1));
    EXPECT_THROW({
                     try { intersect3d(AB, CD); }
                     catch (const std::runtime_error &e) {
                         EXPECT_STREQ(e.what(), "Отрезки скрещиваются");
                         throw;
                     }
                 }, std::runtime_error);
}

// Не пересекаются (компланарные, не параллельные)

TEST(Intersect3d, CoplanarNoIntersection) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, 0, 0));
    Segment3D CD(Vector3D(2, 1, 0), Vector3D(2, 2, 0));
    EXPECT_THROW({
                     try { intersect3d(AB, CD); }
                     catch (const std::runtime_error &e) {
                         EXPECT_STREQ(e.what(), "Отрезки не пересекаются");
                         throw;
                     }
                 }, std::runtime_error);
}

TEST(Intersect3d, LinesIntersectButSegmentsDont) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, 1, 0));
    Segment3D CD(Vector3D(5, 0, 0), Vector3D(6, -1, 0));
    EXPECT_THROW({
                     try { intersect3d(AB, CD); }
                     catch (const std::runtime_error &e) {
                         EXPECT_STREQ(e.what(), "Отрезки не пересекаются");
                         throw;
                     }
                 }, std::runtime_error);
}

// intersect2d

TEST(Intersect2d, BasicIntersection) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(2, 2, 0));
    Segment3D CD(Vector3D(0, 2, 0), Vector3D(2, 0, 0));
    auto r = intersect2d(AB, CD, COORD_X, COORD_Y);
    EXPECT_NEAR(r.first, 1.0, EPS_TEST);
    EXPECT_NEAR(r.second, 1.0, EPS_TEST);
}

TEST(Intersect2d, CollinearThrows) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, 0, 0));
    Segment3D CD(Vector3D(2, 0, 0), Vector3D(3, 0, 0));
    EXPECT_THROW(intersect2d(AB, CD, COORD_X, COORD_Y), CollinearException);
}

TEST(Intersect2d, ParallelThrows) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, 0, 0));
    Segment3D CD(Vector3D(0, 1, 0), Vector3D(1, 1, 0));
    EXPECT_THROW(intersect2d(AB, CD, COORD_X, COORD_Y), std::runtime_error);
}

TEST(Intersect2d, LengthOneVectorLessEPS) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(0.0000000001, 0.0000000001, 0));
    Segment3D CD(Vector3D(0, 1, 0), Vector3D(1, 1, 0));

    EXPECT_THROW(intersect2d(AB, CD, COORD_X, COORD_Y), CollinearException);
}

TEST(Intersect2d, NotIntersect) {
    Segment3D AB(Vector3D(0, 0, 0), Vector3D(1, -1, 0));
    Segment3D CD(Vector3D(0, 1, 0), Vector3D(1, 1, 0));

    EXPECT_THROW(intersect2d(AB, CD, COORD_X, COORD_Y), std::runtime_error);

}

// Скалярное и векторное умножение
TEST(Multiple_vectors, DefaultVectorsMultiple) {
    Vector3D v1{1, 2, 3};
    Vector3D v2{5, 8, 13};
    Vector3D v3 = multiple_vectors(v1, v2);
    EXPECT_NEAR(v3.get_x(), 2, EPS_TEST);
    EXPECT_NEAR(v3.get_y(), 2, EPS_TEST);
    EXPECT_NEAR(v3.get_z(), -2, EPS_TEST);
}

TEST(Scalar_multiple, DefaultScalarMultiple) {
    Vector3D v1{1, 2, 3};
    Vector3D v2{5, 8, 13};
    double result = scalar_multiple(v1, v2);
    EXPECT_EQ(result, 60.0);
}

