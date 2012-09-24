#include <QCoreApplication>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QFutureSynchronizer>
#include <QtConcurrentRun>
#include <iostream>
#include <vector>

#include <iostream>
#include <algorithm>
#include <qmath.h>


#define MAX_CURVE_NUMBER 100

int CURVE_NUMBER = 0;
float distanceMatrix[MAX_CURVE_NUMBER][MAX_CURVE_NUMBER][2][2];
std::vector<int> bestPermutation;
int bestCurvePointOrder = 0;
float bestDistance = 150000;

struct Curve {
    Curve(float x0 = 0, float y0 = 0, float x1 = 0, float y1 = 0, float s = 0)
        : x0(x0), y0(y0), x1(x1), y1(y1), s(s)
    {}
    float x0;
    float y0;
    float x1;
    float y1;
    float s;
};

Curve curves[MAX_CURVE_NUMBER];

float calculateDistance(std::vector<int> const & permutation, int curvesPointOrder, float localBestDistance) {
    float resultDistance = 0;
    int currentCurveStartPoint = 0;
    int previousCurveEndPoint = !(curvesPointOrder & 1);
    curvesPointOrder = curvesPointOrder >> 1;
    for (int i = 1; i < CURVE_NUMBER; i++) {
        currentCurveStartPoint = curvesPointOrder & 1;
        resultDistance += distanceMatrix[permutation[i - 1]][permutation[i]][previousCurveEndPoint][currentCurveStartPoint];
        if (resultDistance > localBestDistance)
            return resultDistance;
        previousCurveEndPoint = !currentCurveStartPoint;
        curvesPointOrder = curvesPointOrder >> 1;
    }
    return resultDistance;
}

float setNewDistance(float distance, std::vector<int> const & permutation, int curvePointOrder) {
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if (distance < bestDistance) {
        bestDistance = distance;
        bestPermutation = permutation;
        bestCurvePointOrder = curvePointOrder;
    }
    return bestDistance;
}

float calculateDistances(std::vector<int> permutation, float localBestDistance)
{
    for (int i = 0; i < qPow(CURVE_NUMBER, 2); i++) {
        float distance = calculateDistance(permutation, i, bestDistance);
        if (distance < localBestDistance) {
            localBestDistance = setNewDistance(distance, permutation, i);
        }
    }
    return localBestDistance;
}

void calculate1000Distances(std::vector<int> permutation, float localBestDistance)
{
    int i = 0;
    do {
        localBestDistance = calculateDistances(permutation, localBestDistance);
        i++;
    }
    while (i < 1000 && std::next_permutation (permutation.begin(), permutation.end()));
}


void showPermutation(int const * permutation) {
    for (int i = 0; i < CURVE_NUMBER; i++) {
        std::cout << " " << permutation[i];
    }
    std::cout << std::endl;
}

inline float calculateDistance(float x0, float y0, float x1, float y1)
{
    return qSqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
}

float createDistanceMatrix() {
    float curveLenghtsSum = 0;
    for (int i = 0; i < CURVE_NUMBER; i++) {
        for (int j = 0; j < i; j++) {
            distanceMatrix[j][i][0][0] = distanceMatrix[i][j][0][0] =
                    calculateDistance(curves[i].x0, curves[i].y0, curves[j].x0, curves[j].y0);
            distanceMatrix[j][i][1][0] = distanceMatrix[i][j][0][1] =
                    calculateDistance(curves[i].x0, curves[i].y0, curves[j].x1, curves[j].y1);
            distanceMatrix[j][i][0][1] = distanceMatrix[i][j][1][0] =
                    calculateDistance(curves[i].x1, curves[i].y1, curves[j].x0, curves[j].y0);
            distanceMatrix[j][i][1][1] = distanceMatrix[i][j][1][1] =
                    calculateDistance(curves[i].x1, curves[i].y1, curves[j].x1, curves[j].y1);
        }
        distanceMatrix[i][i][0][0] = 0;
        distanceMatrix[i][i][0][1] = 0;
        distanceMatrix[i][i][1][0] = 0;
        distanceMatrix[i][i][1][1] = 0;
        curveLenghtsSum += curves[i].s;
    }
    return curveLenghtsSum;
}

int main(int, char **)
{
    QDateTime begin = QDateTime::currentDateTime();
    /// TODO: reading from file
    curves[0] = Curve(1, 1, 0, 0, 3);
    curves[1] = Curve(2, 2, 3, 3, 5);
    curves[2] = Curve(4, 4, 5, 5, 3);
    curves[3] = Curve(6, 6, 7, 7, 5);
    curves[4] = Curve(8, 8, 9, 9, 3);
    curves[5] = Curve(10, 10, 11, 11, 5);
    curves[6] = Curve(12, 12, 13, 13, 3);
    curves[7] = Curve(14, 14, 15, 15, 5);
    curves[8] = Curve(16, 16, 17, 17, 5);
    curves[9] = Curve(18, 18, 19, 19, 5);
    curves[10] = Curve(20, 20, 21, 21, 5);
    CURVE_NUMBER = 11;

    createDistanceMatrix();

    std::vector<int> currentPermutation(CURVE_NUMBER);

    for (int i = 0; i < CURVE_NUMBER; i++) {
        currentPermutation[i] = i;
    }

    QFutureSynchronizer<void> synchronizer;
    int i = 0;
    do {
        if (i % 1000 == 0)
            synchronizer.addFuture(QtConcurrent::run(calculate1000Distances, currentPermutation, bestDistance));
        ++i;
    } while (std::next_permutation (currentPermutation.begin(), currentPermutation.end()));
    synchronizer.waitForFinished();

    std::cout << "result: " << std::endl;
    std::cout << "distance is " << bestDistance << std::endl;
    std::cout << "ponts order in curves is: ";


    int curvesPointOrder = bestCurvePointOrder;
    for (int i = 0; i < CURVE_NUMBER; i++) {
        int currentCurveStartPoint = curvesPointOrder & 1;
        std::cout << bestPermutation[i] << "(" << currentCurveStartPoint << ", " << !currentCurveStartPoint << "), ";
        curvesPointOrder = curvesPointOrder >> 1;
    }
    std::cout << "time in secs is " << begin.msecsTo(QDateTime::currentDateTime()) / 1000 << std::endl;
}
