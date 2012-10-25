#include "bruteForce.h"

std::vector<int> bestPermutation;
int bestCurvePointOrderInt;
QSharedPointer<QMutex> mutex(new QMutex());

/// вычислить перестановку для дистанции с концами
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
/// установка новой лучшей дистанции
float setNewDistance(float distance, std::vector<int> const & permutation, int curvePointOrder) {
    static QMutex mutex;
    QMutexLocker locker(&mutex);
    if (distance < bestDistance) {
        bestDistance = distance;
        bestPermutation = permutation;
        bestCurvePointOrderInt = curvePointOrder;
    }
    return bestDistance;
}
/// вычислить лучшую дистанцию для перестановки
float calculateDistances(std::vector<int> permutation, float localBestDistance)
{
    for (int i = 0; i < qPow(2, CURVE_NUMBER); i++) {
        float distance = calculateDistance(permutation, i, bestDistance);
        if (distance < localBestDistance) {
            localBestDistance = setNewDistance(distance, permutation, i);
        }
    }
    return localBestDistance;
}
/// метод для вычисления порции перестановок
void calculatePackOfDistances(std::vector<int> permutation, float localBestDistance, long distancesInPack)
{
    long i = 0;
    do {
        localBestDistance = calculateDistances(permutation, localBestDistance);
        i++;
    }
    while (i < distancesInPack && std::next_permutation (permutation.begin(), permutation.end()));
}

/// поток для вычиления порции перестановок
class MyThread : public QRunnable {
public:
    MyThread(std::vector<int> const & permutation, float localBestDistance, long distancesInPack)
        :permutation(permutation), localBestDistance(localBestDistance), distancesInPack(distancesInPack)
    {}
protected:
    virtual void run() {
       calculatePackOfDistances(permutation, localBestDistance, distancesInPack);
    }
private:
    std::vector<int> permutation;
    float localBestDistance;
    long distancesInPack;
};

void doBruteForce() {
	/// построить начальную перестановку
    std::vector<int> currentPermutation(CURVE_NUMBER);
	
    for (int i = 0; i < CURVE_NUMBER; i++) {
        currentPermutation[i] = i;
    }

    QThreadPool threadPool;
    threadPool.setMaxThreadCount(8);
    long i = 0;
    // 10! per 8 threads.
    long distancesInPack = 3628800 / 8;
    do {
        if (i % distancesInPack == 0) {
			/// запустить очередной поток для вычисления очередной порции перестановок
            threadPool.start(new MyThread(currentPermutation, bestDistance, distancesInPack));
        }
        ++i;
    } while (std::next_permutation (currentPermutation.begin(), currentPermutation.end()));
	///подождать выполнения всех потоков.
    threadPool.waitForDone();

	/// сохранение лучшего варианта в глобальную переменную
    for (int i = 0; i < CURVE_NUMBER; i++) {
        bestCurvesOrder.append(OrientedCurve(bestPermutation[i], bestCurvePointOrderInt & 1));
        bestCurvePointOrderInt = bestCurvePointOrderInt >> 1;
    }
}
