#ifndef CURVEUNITS_H
#define CURVEUNITS_H

#include <QList>
#include "curveunit.h"
/// класс для работы с объединениями кривых
class CurveUnits
{
public:
    /// здесь создаются начальные прямые
    CurveUnits();
	/// создать лучший путь
    void createBestWay();

    QList<CurveUnit> units;
};

#endif // CURVEUNITS_H
