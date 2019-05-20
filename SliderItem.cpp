#include "SliderItem.h"
#include <QPainter>

SliderItem::SliderItem(QWidget *parent) : QWidget(parent)
{
    setAutoFillBackground(true);
    setBrush(QBrush(QColor("#249dcd")));
}

void SliderItem::setBrush(const QBrush &brush)
{
    QPalette pal;
    pal.setBrush(QPalette::Background, brush);
    setPalette(pal);
}

QBrush SliderItem::getBrush() const
{
    return palette().brush(QPalette::Background);
}
