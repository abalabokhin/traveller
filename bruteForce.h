#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

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

#include "global.h"

float calculateDistance(std::vector<int> const & permutation, int curvesPointOrder, float localBestDistance);

float setNewDistance(float distance, std::vector<int> const & permutation, int curvePointOrder);

float calculateDistances(std::vector<int> permutation, float localBestDistance);

void calculatePackOfDistances(std::vector<int> permutation, float localBestDistance, long distancesInPack);
/// вычислить перестановку перебором. комментарии про функции в cpp файле
void doBruteForce();

#endif // BRUTEFORCE_H
