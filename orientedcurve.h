#ifndef ORIENTEDCURVE_H
#define ORIENTEDCURVE_H

struct OrientedCurve {
    OrientedCurve (int curveNumber = 0, int beginPoint = 0)
        : curveNumber(curveNumber), beginPoint(beginPoint)
    {}
    int curveNumber;
    int beginPoint;
};

#endif // ORIENTEDCURVE_H
