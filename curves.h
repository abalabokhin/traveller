#ifndef CURVES_H
#define CURVES_H

#include <QVector>
#include "curve.h"

struct DistanceMatrix {
    float distances[2][2];
};

class Curves
{
public:
    Curves();

    QVector<QVector<DistanceMatrix> > CreateDistanceMatrix();
    QVector<Curve> curves;
};

#endif // CURVES_H
