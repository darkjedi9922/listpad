#include "SliderItem.h"
#include <QPainter>

SliderItem::SliderItem(QWidget *parent) : QWidget(parent) {}
void SliderItem::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(rect());
}