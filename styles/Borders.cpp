#include "Borders.h"
using namespace Ui;

Borders::Borders() {}
void Borders::draw(QPainter &painter, const QRect &rect)
{
    painter.setPen(Qt::NoPen);

    //painter.setBrush()

    painter.drawRect(rect);
}
