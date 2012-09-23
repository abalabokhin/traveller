#ifndef CURVE_H
#define CURVE_H

#include <QPointF>

class Curve
{
public:
    Curve(QPointF const point1 = QPointF(), QPointF const & point2 = QPointF());
    float static distance(QPointF point1, QPointF point2);
    bool operator == (Curve const & right) const;
    QPointF point1;
    QPointF point2;
};

#endif // CURVE_H
