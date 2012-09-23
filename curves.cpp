#include "curves.h"

Curves::Curves()
{

}

QVector<QVector<DistanceMatrix> > Curves::CreateDistanceMatrix()
{
    /// Можно оптимизировать - вычислять только треугольную матрицу
    /// считаем что любые две кривые не имют совпадающих концов - хотя может это и не страшно, увидим по ходу реализации
    QVector<QVector<DistanceMatrix> > result;
    foreach (Curve curve1, curves) {
        QVector<DistanceMatrix> string;
        foreach (Curve curve2, curves) {
            if (curve1 == curve2) {
                string.append(DistanceMatrix(0, 0, 0, 0));
                continue;
            }
            string.append(DistanceMatrix(
              Curve::distance(curve1.point1, curve2.point1),
              Curve::distance(curve1.point1, curve2.point2),
              Curve::distance(curve1.point2, curve2.point1),
              Curve::distance(curve1.point2, curve2.point2)
            ));
        }
        result.append(string);
    }
    return result;
}
