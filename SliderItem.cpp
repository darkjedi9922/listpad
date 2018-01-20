#include "SliderItem.h"
#include <QPainter>

SliderItem::SliderItem(QWidget *parent) : QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pal;
    pal.setBrush(QPalette::Background, QBrush(QColor("#249dcd")));
    setPalette(pal);
}