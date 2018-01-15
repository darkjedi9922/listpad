#include "Pixmap.h"
using namespace Ui;

Pixmap::Pixmap() {}
Pixmap::~Pixmap() {}
void Pixmap::draw(QPainter &painter, const QRect &rect)
{
    painter.setPen(Qt::NoPen);
    painter.drawPixmap(rect, pixmap);
}
