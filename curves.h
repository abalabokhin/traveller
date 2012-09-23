#ifndef CURVES_H
#define CURVES_H

#include <QVector>
#include "curve.h"

struct DistanceMatrix {
    DistanceMatrix(float s11 = 0, float s12 = 0, float s21 = 0, float s22 = 0)
        :   s11(s11), s12(s12), s21(s21), s22(s22)
    {}
    float s11;
    float s12;
    float s21;
    float s22;
};

class Curves
{
public:
    Curves();

    QVector<QVector<DistanceMatrix> > CreateDistanceMatrix();
    QVector<Curve> curves;
};

#endif // CURVES_H
