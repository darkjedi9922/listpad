#include "Background.h"
using namespace Ui;

Background::Background() :
    radius(QPoint(0, 0))
{}

void Background::setRadius(const QPoint &radius)
{
    this->radius = radius;
}

const QPoint& Background::getRadius() const
{
    return radius;
}