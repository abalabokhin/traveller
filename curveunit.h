#ifndef CURVEUNIT_H
#define CURVEUNIT_H

#include <QList>
#include "orientedcurve.h"
/// одно объединение соединенных кривых
struct CurveUnit
{
    CurveUnit(int curveNumber = 0);
    void AddUnit(CurveUnit & right, int linkedPointInFirstCurve, int linkedPointInSecondCurve, float distance);
    void getFreePoint(int number, int & curveNumber, int & pointInCurve);
    void reverse();
    QList<OrientedCurve> curves;
    float summaryLength;
};

#endif // CURVEUNIT_H
