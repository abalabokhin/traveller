#include <QApplication>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QRunnable>
#include <QThreadPool>

#include <QImage>
#include <QPainter>

#include <iostream>
#include <fstream>
#include <vector>

#include <iostream>
#include <algorithm>
#include <qmath.h>


#define MAX_CURVE_NUMBER 100

int CURVE_NUMBER = 0;
float distanceMatrix[MAX_CURVE_NUMBER][MAX_CURVE_NUMBER][2][2];
std::vector<int> bestPermutation;
int bestCurvePointOrderInt = 0;
std::vector<int> bestCurvePointOrder;
float bestDistance = 150000;
QMutex * mutex;

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
        bestCurvePointOrderInt = curvePointOrder;
    }
    return bestDistance;
}

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

void calculatePackOfDistances(std::vector<int> permutation, float localBestDistance, long distancesInPack)
{
    long i = 0;
    do {
        localBestDistance = calculateDistances(permutation, localBestDistance);
        i++;
    }
    while (i < distancesInPack && std::next_permutation (permutation.begin(), permutation.end()));
}

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
        for (int j = i; j < CURVE_NUMBER; j++) {
            if (i == 2 && j == 3) {
                int temp = 6;
                temp++;
            }
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

void doBruteForce() {
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
            threadPool.start(new MyThread(currentPermutation, bestDistance, distancesInPack));
        }
        ++i;
    } while (std::next_permutation (currentPermutation.begin(), currentPermutation.end()));
    threadPool.waitForDone();

    for (int i = 0; i < CURVE_NUMBER; i++) {
        bestCurvePointOrder.push_back(bestCurvePointOrderInt & 1);
        bestCurvePointOrderInt = bestCurvePointOrderInt >> 1;
    }
}

void doAntColony() {

}

void visualizeIt() {
    QImage image(1000, 1000, QImage::Format_RGB32);
    QPainter painter(&image);

    QPointF firstCurvePoint;
    QPointF lastCurvePoint;
    QPointF previousCurveLastPoint;

    int i = 0;
    if (!bestCurvePointOrder[i]) {
        firstCurvePoint = QPointF(curves[bestPermutation[i]].x0, curves[bestPermutation[i]].y0);
        lastCurvePoint = QPointF(curves[bestPermutation[i]].x1, curves[bestPermutation[i]].y1);
    } else {
        firstCurvePoint = QPointF(curves[bestPermutation[i]].x1, curves[bestPermutation[i]].y1);
        lastCurvePoint = QPointF(curves[bestPermutation[i]].x0, curves[bestPermutation[i]].y0);
    }

    painter.setPen(Qt::black);
    QPointF (pointToText) = (firstCurvePoint + lastCurvePoint) / 2;
    painter.drawText(pointToText, QString::number(bestPermutation[i]) + "-" + QString::number(i));


    painter.setPen(Qt::blue);
    painter.drawLine(firstCurvePoint, lastCurvePoint);

    i++;
    for (; i < CURVE_NUMBER; i++) {
        previousCurveLastPoint = lastCurvePoint;
        if (!bestCurvePointOrder[i]) {
            firstCurvePoint = QPointF(curves[bestPermutation[i]].x0, curves[bestPermutation[i]].y0);
            lastCurvePoint = QPointF(curves[bestPermutation[i]].x1, curves[bestPermutation[i]].y1);
        } else {
            firstCurvePoint = QPointF(curves[bestPermutation[i]].x1, curves[bestPermutation[i]].y1);
            lastCurvePoint = QPointF(curves[bestPermutation[i]].x0, curves[bestPermutation[i]].y0);
        }
        painter.setPen(Qt::black);
        QPointF (pointToText) = (firstCurvePoint + lastCurvePoint) / 2;
        painter.drawText(pointToText, QString::number(bestPermutation[i]) + "-" + QString::number(i));
        painter.setPen(Qt::red);
        painter.drawLine(previousCurveLastPoint, firstCurvePoint);
        painter.setPen(Qt::blue);
        painter.drawLine(firstCurvePoint, lastCurvePoint);
    }
    painter.end();
    image.save("output.png");
}

int main(int argc, char * argv[])
{
    QDateTime begin = QDateTime::currentDateTime();
    mutex = new QMutex();
    std::ifstream istream("input.txt");
    int curveNumber = 0;
    char commaCh;
    while (!istream.eof()) {
        istream >> curves[curveNumber].x0;
        istream >> commaCh;
        istream >> curves[curveNumber].y0;
        istream >> commaCh;
        istream >> curves[curveNumber].x1;
        istream >> commaCh;
        istream >> curves[curveNumber].y1;
        istream >> commaCh;
        istream >> curves[curveNumber].s;
        curveNumber++;
    }

    CURVE_NUMBER = curveNumber;
    float curveLenghtsSum = createDistanceMatrix();

    if (curveNumber <= 9)
        doBruteForce();
    else
        doAntColony();

    std::ofstream outputStream("output.txt");
    std::ofstream resultStream("result.txt");
    resultStream << bestDistance + curveLenghtsSum;

    for (int i = 0; i < CURVE_NUMBER; i++) {
        outputStream << bestPermutation[i] << " " << bestCurvePointOrder[i] << std::endl;
    }
    delete mutex;
    std::cout << "time in secs is " << begin.msecsTo(QDateTime::currentDateTime()) / 1000 << std::endl;
    QApplication app(argc, argv);
    visualizeIt();
}
