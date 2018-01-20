#include "Slider.h"
#include <QPainter>
#include <QMouseEvent>

#include <QDebug>

Slider::Slider(Qt::Orientation orientation, QWidget *parent) :
    QSlider(orientation, parent),
    slider(new SliderItem(this)),
    sliderFixedLength(-1),
    lastPixelValue(0),
    moving(false)
{
    setMinimum(0);
    setMaximum(10);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    updateSliderItemSize();
    slider->show();
}
void Slider::setValue(int value)
{
    if (value != this->value()) {
        if (value < 0) {
            QSlider::setValue(minimum());
            setSliderPosition(0);
        } else if (value > maximum()) {
            QSlider::setValue(maximum());
            setSliderPosition(getLength() - getSliderLength());
        } else {
            if (value % this->singleStep() != 0) {
                if (value > this->value()) value += singleStep() - value % singleStep();
                else if (value < this->value()) value -= singleStep() - value % singleStep();
            }
            QSlider::setValue(value);
            setSliderPosition(calcPixelValueFromReal(value));
        }
    }
}
void Slider::setSliderPosition(int position)
{
    if (orientation() == Qt::Horizontal) slider->move(position, slider->geometry().y());
    else slider->move(slider->geometry().x(), position);
}
void Slider::setSliderFixedLength(int length)
{
    this->sliderFixedLength = length;
    this->updateSliderItemSize();
}
void Slider::setSliderAutoLength()
{
    this->sliderFixedLength = -1;
    this->updateSliderItemSize();
}
int Slider::getSliderLength()
{
    return getOrientationValue(slider->width(), slider->height());
}
int Slider::getLength()
{
    return getOrientationValue(width(), height());
}

void Slider::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(rect());
}
void Slider::resizeEvent(QResizeEvent *)
{
    updateSliderItemSize();
    updateSliderPosition();
}
void Slider::mousePressEvent(QMouseEvent *e)
{
    if (slider->geometry().contains(e->pos())) {
        moving = true;
        if (orientation() == Qt::Horizontal) {
            lastPressPoint = e->x();
            lastPixelValue = slider->geometry().x();
        } else {
            lastPressPoint = e->y();
            lastPixelValue = slider->geometry().y();
        }
    }
}
void Slider::mouseMoveEvent(QMouseEvent *e)
{
    if (moving) {
        float newPixelValue = lastPixelValue + getOrientationValue(e->x(), e->y()) - lastPressPoint;
        int realValue = this->calcRealValueFromPixel(newPixelValue);
        if (realValue % this->singleStep() == 0) this->setValue(realValue);
    }
}
void Slider::mouseReleaseEvent(QMouseEvent *)
{
    moving = false;
}

void Slider::updateSliderItemSize()
{
    int sliderLength = (sliderFixedLength == -1 ? calcSliderAutoLength() : sliderFixedLength);
    if (orientation() == Qt::Horizontal) slider->setFixedSize(sliderLength, height());
    else slider->setFixedSize(width(), sliderLength);
    repaint();
}
void Slider::updateSliderPosition()
{
    this->setSliderPosition(this->calcPixelValueFromReal(this->value()));
    repaint();
}
int Slider::calcSliderAutoLength()
{
    if (maximum() - minimum() + 1 == 0) return getLength();
    else return (getLength() / (maximum() - minimum() + 1));
}
int Slider::calcRealValueFromPixel(int pxValue)
{
    if (getLength() - getSliderLength() == 0) return 0;
    else return (minimum() + pxValue * (maximum() - minimum()) / (getLength() - getSliderLength()));
}
int Slider::calcPixelValueFromReal(int realValue)
{
    if (maximum() - minimum() == 0) return 0;
    return ((realValue - minimum()) * (getLength() - getSliderLength()) / (maximum() - minimum()));
}
int Slider::getOrientationValue(int hValue, int vValue)
{
    if (orientation() == Qt::Horizontal) return hValue;
    else return vValue;
}
