#include "ScrollBar.h"
#include <QPainter>
#include <QMouseEvent>

#include <QDebug>

ScrollBar::ScrollBar(Qt::Orientation o, QWidget *parent) :
    QScrollBar(o, parent),
    documentLength(0),
    currentValue(0),
    lastMousePressPoint(0),
    lastValue(0),
    moving(false)
{}
void ScrollBar::setValue(int value)
{
    currentValue = value;
    repaint();
}
void ScrollBar::setDocumentLength(int length)
{
    if (length == 0) setMaximum(0);
    else setMaximum((length - height()) / length);
    documentLength = length;
}
int ScrollBar::value() const
{
    return currentValue;
}
int ScrollBar::getDocumentLength() const
{
    return documentLength;
}
const QRect ScrollBar::calculateSliderRect() const
{
    return QRect(0, value(), width(), height() * height() / getDocumentLength());
}

void ScrollBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(rect());
    painter.setBrush(Qt::white);
    painter.drawRect(calculateSliderRect());
}
void ScrollBar::mousePressEvent(QMouseEvent *e)
{
    if (calculateSliderRect().contains(e->pos())) {
        lastMousePressPoint = e->y();
        lastValue = value();
        moving = true;
    }
}
void ScrollBar::mouseMoveEvent(QMouseEvent *e)
{
    if (moving) {
        if (e->y() < 0) setValue(0);
        else if (e->y() > maximum()) setValue(maximum());
        else {
            int diff = e->y() - lastMousePressPoint;
            int newValue = lastValue + diff;
            if (newValue >= minimum() && newValue <= maximum()) setValue(lastValue + diff);
        }
    }
}
void ScrollBar::mouseReleaseEvent(QMouseEvent *)
{
    moving = false;
}
