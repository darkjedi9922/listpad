#include "ScrollArea.h"
#include <QDebug>

ScrollArea::ScrollArea(QWidget *parent) : QScrollArea(parent) {}
QSize ScrollArea::sizeHint() const
{
    if (this->widget()) return widget()->size();
    else return QScrollArea::sizeHint();
}

