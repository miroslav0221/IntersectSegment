#ifndef INTERSECTSEGMENT_INTERSECT_H
#define INTERSECTSEGMENT_INTERSECT_H

#include <stdexcept>
#include <utility>

#include "Vector3D.h"
#include "Segment3D.h"

//Точность решения
const double EPS = 1e-9;

//Исключения для обработки вырожденных случаев. Например, проекции прямой в точку.
struct CollinearException : std::runtime_error {
    CollinearException() : std::runtime_error("Лежат на одной прямой") {}
};

//Структура для удобства получения координат из вектора 2d случая. Удобно, так как можно рассмотреть в функции
//intersect2d не только XY, но и XZ, YZ
struct CoordAccessor {
    double (Vector3D::*point)() const;
};

extern const CoordAccessor COORD_X;
extern const CoordAccessor COORD_Y;
extern const CoordAccessor COORD_Z;

//Функция поиска точки пересечения 2 отрезков в двухмерном случае. Если отрезки лежат друг на друге,
//то они пересекаются, но не в одной точке, а в отрезке. По этой причине функция возвращает std::pair. Если отрезок
//пересечения, то вернет начало и конец отрезка пересечения, если одна точка, то значение std::pair совпадают
std::pair<double, double> intersect2d(const Segment3D &AB, const Segment3D &CD,
                                      CoordAccessor c1, CoordAccessor c2);

//Функция поиск точки пересечения 2 отрезков в трехмерном случае. По аналогии с двухмерным случаем возвращает std::pair
std::pair<Vector3D, Vector3D> intersect3d(const Segment3D &AB, const Segment3D &CD);

//Векторное произведение векторов
Vector3D multiple_vectors(const Vector3D &AB, const Vector3D &CD);

//Скалярное произведение векторов
double scalar_multiple(const Vector3D &a, const Vector3D &b);


#endif