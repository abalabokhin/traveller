#include "curve.h"
#include <qmath.h>

Curve::Curve(QPointF const point1, QPointF const & point2)
    : point1(point1), point2(point2)
{}

float Curve::distance(QPointF point1, QPointF point2)
{
    return qSqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) + (point1.y() - point2.y()) * (point1.y() - point2.y()));
}

bool Curve::operator ==(const Curve &right) const
{
    return right.point1 == point1 && right.point2 == point2;
}

