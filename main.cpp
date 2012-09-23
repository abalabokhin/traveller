#include <QCoreApplication>
#include <iostream>

#include <iostream>
#include <algorithm>
#include <qmath.h>

#include "curve.h"
#include "curves.h"

float calculateDistance(QVector<QVector<DistanceMatrix> > const & distanceMatrix, int curvesPointOrder, std::vector<int> const & permutation) {
    float resultDistance = 0;
    int currentCurveStartPoint = 0;
    int previousCurveEndPoint = !(curvesPointOrder & 1);
    curvesPointOrder = curvesPointOrder >> 1;
    for (unsigned int i = 1; i < permutation.size(); i++) {
        currentCurveStartPoint = curvesPointOrder & 1;
        if (!currentCurveStartPoint && !previousCurveEndPoint)
            resultDistance += distanceMatrix[permutation[i - 1]][permutation[i]].s11;
        if (currentCurveStartPoint && !previousCurveEndPoint)
            resultDistance += distanceMatrix[permutation[i - 1]][permutation[i]].s12;
        if (!currentCurveStartPoint && previousCurveEndPoint)
            resultDistance += distanceMatrix[permutation[i - 1]][permutation[i]].s21;
        if (currentCurveStartPoint && previousCurveEndPoint)
            resultDistance += distanceMatrix[permutation[i - 1]][permutation[i]].s22;


        previousCurveEndPoint = !currentCurveStartPoint;
        curvesPointOrder = curvesPointOrder >> 1;
    }
    return resultDistance;
}

void calculateDistances(QVector<QVector<DistanceMatrix> > const & distanceMatrix,
                         std::vector<int> const & permutation,
                         std::vector<int> & bestPermutation,
                         float & bestPermutationDistance,
                         int & bestCurvePointOrder)
{
    for (int i = 0; i < qPow(permutation.size(), 2); i++) {
        float distance = calculateDistance(distanceMatrix, i,  permutation);
        if (distance < bestPermutationDistance) {
            bestPermutationDistance = distance;
            bestPermutation = permutation;
            bestCurvePointOrder = i;
        }
    }
}


void showPermutation(std::vector<int> const & permutation) {
    for (unsigned int i = 0; i < permutation.size(); i++) {
        std::cout << " " << permutation[i];
    }
    std::cout << std::endl;
}

int main(int, char **)
{
    Curves curves;
    curves.curves.append(Curve(QPointF(0, 1), QPointF(0, 0)));
    curves.curves.append(Curve(QPointF(0, 2), QPointF(1, 2)));
    curves.curves.append(Curve(QPointF(2, 2), QPointF(3, 2)));
    curves.curves.append(Curve(QPointF(3, 1), QPointF(3, 0)));
    curves.curves.append(Curve(QPointF(3, -1), QPointF(2, -1)));
    curves.curves.append(Curve(QPointF(0, -1), QPointF(1, -1)));

    curves.curves.append(Curve(QPointF(4, 4), QPointF(5, 5)));
    curves.curves.append(Curve(QPointF(6, 6), QPointF(7, 7)));
    //curves.curves.append(Curve(QPointF(8, 8), QPointF(9, 9)));
    //curves.curves.append(Curve(QPointF(10, 10), QPointF(11, 11)));


    QVector<QVector<DistanceMatrix> > distanceMatrix = curves.CreateDistanceMatrix();

    std::vector<int> currentPermutation;

    for (int i = 0; i < curves.curves.size(); i++) {
        currentPermutation.push_back(i);
    }

    std::vector<int> bestPermutation(curves.curves.size());
    int bestCurvePointOrder = 0;
    float bestPermutationDistance = 10000;

    do {
        //showPermutation(currentPermutation);
        calculateDistances(distanceMatrix, currentPermutation, bestPermutation, bestPermutationDistance, bestCurvePointOrder);
    } while ( std::next_permutation (currentPermutation.begin(),currentPermutation.end()) );

    std::cout << "result: " << std::endl;
    //showPermutation(bestPermutation);
    std::cout << "distance is " << bestPermutationDistance << std::endl;
    std::cout << "ponts order in curves is: ";


    int curvesPointOrder = bestCurvePointOrder;
    for (unsigned int i = 0; i < bestPermutation.size(); i++) {
        int currentCurveStartPoint = curvesPointOrder & 1;
        std::cout << bestPermutation[i] << "(" << currentCurveStartPoint << ", " << !currentCurveStartPoint << "), ";
        curvesPointOrder = curvesPointOrder >> 1;
    }
    std::cout << "result: " << std::endl;
}
