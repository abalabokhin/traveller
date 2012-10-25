#ifndef GLOBAL_H
#define GLOBAL_H

#include <QMutex>
#include <QDateTime>
#include "orientedcurve.h"

#define MAX_CURVE_NUMBER 100

extern int CURVE_NUMBER;
extern float distanceMatrix[MAX_CURVE_NUMBER][MAX_CURVE_NUMBER][2][2];
extern float bestDistance;
extern QList<OrientedCurve> bestCurvesOrder;
extern QDateTime beginTime;

#endif // GLOBAL_H
