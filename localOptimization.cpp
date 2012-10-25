#include <QVector>
#include <qmath.h>

#include "localOptimization.h"

/// вычисление дистанции для последовательности кривых
float calculateSumSeveralCurvesInRow(OrientedCurve * firstCurve, QList<OrientedCurve> curves, OrientedCurve * lastCurve) {
    float distance = firstCurve ? distanceMatrix[firstCurve->curveNumber][curves.first().curveNumber][!firstCurve->beginPoint][curves.first().beginPoint] : 0;
    OrientedCurve previousCurve = curves.first();
    for (QList<OrientedCurve>::iterator currentCurve = curves.begin() + 1; currentCurve != curves.end(); ++currentCurve) {
        distance += distanceMatrix[previousCurve.curveNumber][currentCurve->curveNumber][!previousCurve.beginPoint][currentCurve->beginPoint];
        previousCurve = *currentCurve;
    }
    distance += lastCurve ? distanceMatrix[curves.last().curveNumber][lastCurve->curveNumber][!curves.last().beginPoint][lastCurve->beginPoint] : 0;
    return distance;
}

bool findBetterWay(int permutationSize) {
    /// create first permutation
    QList<int> beginPermutation;
    for (int i = 0; i < permutationSize; i++) {
        beginPermutation.append(i);
    }
	/// перетасовка всех вершин в лучшей последовательности размером permutationSize.
	/// поиск лучшего решения
    for (int curveI = 0; curveI < CURVE_NUMBER - permutationSize + 1; curveI++) {
        QList<OrientedCurve> localCurves = bestCurvesOrder.mid(curveI, permutationSize);
        OrientedCurve * previousCurve = (curveI == 0 ? NULL : &bestCurvesOrder[curveI - 1]);
        OrientedCurve * nextCurve = (curveI == CURVE_NUMBER - permutationSize ? NULL : &bestCurvesOrder[curveI + permutationSize]);
        float currentBestDistance =
            calculateSumSeveralCurvesInRow(previousCurve, localCurves, nextCurve);
        int maxPointsOrder = qPow(2, permutationSize);
		/// проходим все возможные локальные перестановки
        for (int i = 0; i < maxPointsOrder; i++) {
            int localCurvesPointOrder = i;
            for (int j = 0; j < permutationSize; j++) {
                localCurves[j].beginPoint = localCurvesPointOrder & 1;
                localCurvesPointOrder = localCurvesPointOrder >> 1;
            }
            QList<int> currentPermutation = beginPermutation;
            do {
                QList<OrientedCurve> localChain;
                foreach(int curveNumberInOrder, currentPermutation) {
                    localChain.append(localCurves[curveNumberInOrder]);
                }
                float currentDistance =
                    calculateSumSeveralCurvesInRow(previousCurve, localChain, nextCurve);
				/// проверка текущей перестановки
                if (currentDistance < currentBestDistance) {
                    for (int k = 0; k < permutationSize; ++k) {
                        bestCurvesOrder[curveI + k] = localChain[k];
                    }
                    bestDistance = bestDistance - currentBestDistance + currentDistance;
                    return true;
                }
            } while (std::next_permutation (currentPermutation.begin(), currentPermutation.end()));
        }
    }
    return false;
}
