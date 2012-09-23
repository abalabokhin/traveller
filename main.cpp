#include <QCoreApplication>
#include <iostream>
#include "curve.h"
#include "curves.h"



int main(int, char **)
{
    Curves curves;
    curves.curves.append(Curve(QPointF(0, 0), QPointF(0, 1)));
    curves.curves.append(Curve(QPointF(1, 2), QPointF(2, 2)));

    QVector<QVector<DistanceMatrix> > distanceMatrix = curves.CreateDistanceMatrix();

    int i = 56;












}
