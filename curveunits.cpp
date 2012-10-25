#include "curveunits.h"

#include "global.h"

CurveUnits::CurveUnits()
{
    for (int i = 0; i < CURVE_NUMBER; i++) {
        units.push_back(CurveUnit(i));
    }
}

void CurveUnits::createBestWay()
{
/// полка юнитов не станет 1
    while (units.size() > 1) {
        float bestDistanceBetweenTwoPoints = 1500;
        int bestUnit1 = 0;
        int bestLinkedPointInFirstCurve = 0;
        int bestUnit2 = 0;
        int bestLinkedPointInSecondCurve = 0;
		/// поиск минимальной дистанции
        for (int unit1 = 0; unit1 < units.size() - 1; ++unit1) {
            for (int unit2 = unit1 + 1; unit2 < units.size(); ++unit2) {			
                for (int i = 0; i < 2; i++) {
                    for (int j = 0; j < 2; j++) {
                        int curveNumber1 = 0;
                        int pointInCurve1 = 0;
                        int curveNumber2 = 0;
                        int pointInCurve2 = 0;

                        units[unit1].getFreePoint(i, curveNumber1, pointInCurve1);
                        units[unit2].getFreePoint(j, curveNumber2, pointInCurve2);
                        float localDistance = distanceMatrix[curveNumber1][curveNumber2][pointInCurve1][pointInCurve2];
                        if (localDistance < bestDistanceBetweenTwoPoints) {
                            bestDistanceBetweenTwoPoints = localDistance;
                            bestUnit1 = unit1;
                            bestLinkedPointInFirstCurve = i;
                            bestUnit2 = unit2;
                            bestLinkedPointInSecondCurve = j;
                        }
                    }
                }
            }
        }
		/// соединение найденных юнитов
        units[bestUnit1].AddUnit(units[bestUnit2], bestLinkedPointInFirstCurve, bestLinkedPointInSecondCurve, bestDistanceBetweenTwoPoints);
        units.removeAt(bestUnit2);
    }
    bestCurvesOrder = units.begin()->curves;
    bestDistance = units.begin()->summaryLength;
}
