#ifndef CURVEUNITS_H
#define CURVEUNITS_H

#include <QList>
#include "curveunit.h"

class CurveUnits
{
public:
    CurveUnits();
    void createBestWay();

    QList<CurveUnit> units;
};

#endif // CURVEUNITS_H
