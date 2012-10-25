#include "curveunit.h"

CurveUnit::CurveUnit(int curveNumber)
    : summaryLength(0)
{
    curves.push_back(OrientedCurve(curveNumber, 0));
}

void CurveUnit::AddUnit(CurveUnit & right, int linkedPointInFirstCurve, int linkedPointInSecondCurve, float distance)
{
    if (!linkedPointInFirstCurve)
        reverse();
    if (linkedPointInSecondCurve)
        right.reverse();
    curves.append(right.curves);
    summaryLength = summaryLength + right.summaryLength + distance;
}

void CurveUnit::getFreePoint(int number, int &curveNumber, int &pointInCurve)
{
    if (!number) {
        curveNumber = curves.begin()->curveNumber;
        pointInCurve = curves.begin()->beginPoint;
        return;
    } else {
        curveNumber = curves.last().curveNumber;
        pointInCurve = !curves.last().beginPoint;
    }
}

void CurveUnit::reverse()
{
    QList<OrientedCurve> reversedCurves;
    foreach (OrientedCurve curve, curves) {
        curve.beginPoint = !curve.beginPoint;
        reversedCurves.push_front(curve);
    }
    curves = reversedCurves;
}
