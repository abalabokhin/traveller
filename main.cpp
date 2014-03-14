#include <QDateTime>
#include <QFile>
#include <QTextStream>

#include <iostream>

#include <algorithm>
#include <qmath.h>

#include "global.h"
#include "curveunits.h"
#include "localOptimization.h"
#include "bruteForce.h"

int CURVE_NUMBER = 0;
float distanceMatrix[MAX_CURVE_NUMBER][MAX_CURVE_NUMBER][2][2];
float bestDistance = 150000;
QList<OrientedCurve> bestCurvesOrder;
QDateTime beginTime;

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

inline float calculateDistance(float x0, float y0, float x1, float y1)
{
    return qSqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
}

/// создать матрицу расстояний
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

void doNextNeighbor() {
    CurveUnits units;
	/// выстроить кандидата на результат
    units.createBestWay();
	/// искать лучший путь, пока можно и пока не истечет время
    while (findBetterWay(4) && beginTime.msecsTo(QDateTime::currentDateTime()) < 9000);
}

//void visualizeIt() {
//    QImage image(1000, 1000, QImage::Format_RGB32);
//    QPainter painter(&image);

//    QPointF firstCurvePoint;
//    QPointF lastCurvePoint;
//    QPointF previousCurveLastPoint;

//    int j = 0;
//    QList<OrientedCurve>::iterator i = bestCurvesOrder.begin();//
//    if (!i->beginPoint) {
//        firstCurvePoint = QPointF(curves[i->curveNumber].x0, curves[i->curveNumber].y0);
//        lastCurvePoint = QPointF(curves[i->curveNumber].x1, curves[i->curveNumber].y1);
//    } else {
//        firstCurvePoint = QPointF(curves[i->curveNumber].x1, curves[i->curveNumber].y1);
//        lastCurvePoint = QPointF(curves[i->curveNumber].x0, curves[i->curveNumber].y0);
//    }

//    painter.setPen(Qt::black);
//    QPointF (pointToText) = (firstCurvePoint + lastCurvePoint) / 2;
//    painter.drawText(pointToText, QString::number(i->curveNumber) + "-" + QString::number(j));

//    painter.setPen(Qt::blue);
//    painter.drawLine(firstCurvePoint, lastCurvePoint);

//    ++j;
//    ++i;
//    for (; i != bestCurvesOrder.end(); ++i, ++j) {
//        previousCurveLastPoint = lastCurvePoint;
//        if (!i->beginPoint) {
//            firstCurvePoint = QPointF(curves[i->curveNumber].x0, curves[i->curveNumber].y0);
//            lastCurvePoint = QPointF(curves[i->curveNumber].x1, curves[i->curveNumber].y1);
//        } else {
//            firstCurvePoint = QPointF(curves[i->curveNumber].x1, curves[i->curveNumber].y1);
//            lastCurvePoint = QPointF(curves[i->curveNumber].x0, curves[i->curveNumber].y0);
//        }
//        painter.setPen(Qt::black);
//        QPointF (pointToText) = (firstCurvePoint + lastCurvePoint) / 2;
//        painter.drawText(pointToText, QString::number(i->curveNumber) + "-" + QString::number(j));
//        painter.setPen(Qt::red);
//        painter.drawLine(previousCurveLastPoint, firstCurvePoint);
//        painter.setPen(Qt::blue);
//        painter.drawLine(firstCurvePoint, lastCurvePoint);
//    }
//    painter.end();
//    image.save("output.png");
//}

int main(int argc, char * argv[])
{    QString inputFileName("input1.txt");
     QString outputFileName("balabok1.txt");

    beginTime = QDateTime::currentDateTime();
    QFile data(inputFileName);
    if (!data.open(QFile::ReadOnly))
        return -1;
	/// Считывание данных из файла
    QTextStream out(&data);
    int curveNumber = 0;
    while (!out.atEnd()) {
        QString string = out.readLine();
        QStringList strings = string.split(",");
        if (strings.size() != 5)
            continue;
        curves[curveNumber].x0 = strings[0].toFloat();
        curves[curveNumber].y0 = strings[1].toFloat();
        curves[curveNumber].x1 = strings[2].toFloat();
        curves[curveNumber].y1 = strings[3].toFloat();
        curves[curveNumber].s = strings[4].toFloat();
        curveNumber++;
    }

    CURVE_NUMBER = curveNumber;
	/// построить матрицу расстояний
    float curveLenghtsSum = createDistanceMatrix();
    if (curveNumber <= 8)
	/// запустить брутфорс при малом количестве кривых
        doBruteForce();
    else
	/// запустить алгоритм поиска ближайших соседей
        doNextNeighbor();

	/// вывод конечных данных
    QFile output(outputFileName);
    output.open(QFile::WriteOnly);

    QTextStream outputStream(&output);
    outputStream << qSetRealNumberPrecision(10) << bestDistance + curveLenghtsSum << endl;

    foreach (OrientedCurve i, bestCurvesOrder) {
        outputStream << i.curveNumber + 1 << " " << i.beginPoint + 1 << endl;
    }
    std::cout << "time in msecs is " << beginTime.msecsTo(QDateTime::currentDateTime()) << std::endl;
    //QApplication app(argc, argv);
    //visualizeIt();
}
