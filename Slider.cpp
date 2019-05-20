#include "Slider.h"
#include <QPainter>
#include <QMouseEvent>

// ==== PUBLIC ====
Slider::Slider(Qt::Orientation orientation, QWidget *parent) :
    QWidget(parent),
    slider(new SliderItem(this)),
    orientation(orientation),
    singleStep(1),
    minimum(0),
    maximum(10),
    value(0),
    sliderFixedLength(-1),
    lastPixelValue(0),
    moving(false)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    updateSliderItemSize();
    slider->show();
}

SliderItem* Slider::getSliderItem() const
{
    return slider;
}

void Slider::setOrientation(Qt::Orientation orientation)
{
    this->orientation = orientation;
    updateSliderItemSize();
    updateSliderPosition();
}
Qt::Orientation Slider::getOrientation() const
{
    return orientation;
}

void Slider::setSingleStep(int step)
{
    if (step == 0) step += 1;
    singleStep = step;
}
int Slider::getSingleStep() const
{
    return singleStep;
}

void Slider::setMinimum(int m)
{
    minimum = m;
    if (value < m) setValue(m);
    updateSliderItemSize();
}
int Slider::getMinimum() const
{
    return minimum;
}
void Slider::setMaximum(int m)
{
    maximum = m;
    if (value > m) setValue(m);
    updateSliderItemSize();
}
int Slider::getMaximum() const
{
    return maximum;
}
void Slider::setValue(int value)
{
    if (value != this->value) {
        if (value < minimum) {
            this->value = minimum;
            setSliderPosition(0);
            emit valueChanged(this->value);
        } else if (value > maximum) {
            this->value = maximum;
            setSliderPosition(getLength() - getSliderLength());
            emit valueChanged(this->value);
        } else {
            if (value % singleStep != 0) {
                if (value > this->value) value += singleStep - value % singleStep;
                else if (value < this->value) value -= singleStep - value % singleStep;
            }
            this->value = value;
            setSliderPosition(calcPixelValueFromReal(value));
            emit valueChanged(this->value);
        }
    }
}
int Slider::getValue() const
{
    return value;
}
void Slider::setSliderPosition(int position)
{
    if (orientation == Qt::Horizontal) slider->move(position, slider->geometry().y());
    else slider->move(slider->geometry().x(), position);
}
int Slider::getSliderPosition() const
{
    if (orientation == Qt::Horizontal) return slider->geometry().x();
    else return slider->geometry().y();
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

// ==== EVENTS ====
void Slider::resizeEvent(QResizeEvent *)
{
    updateSliderItemSize();
    updateSliderPosition();
}
void Slider::mousePressEvent(QMouseEvent *e)
{
    if (slider->geometry().contains(e->pos())) {
        moving = true;
        if (orientation == Qt::Horizontal) {
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
        if (realValue % singleStep == 0) {
            this->setValue(realValue);
        }
    }
}
void Slider::mouseReleaseEvent(QMouseEvent *)
{
    moving = false;
}

// ==== PRIVATE ====
void Slider::updateSliderItemSize()
{
    int sliderLength = (sliderFixedLength == -1 ? calcSliderAutoLength() : sliderFixedLength);
    if (orientation == Qt::Horizontal) slider->setFixedSize(sliderLength, height());
    else slider->setFixedSize(width(), sliderLength);
    repaint();
}
void Slider::updateSliderPosition()
{
    this->setSliderPosition(this->calcPixelValueFromReal(value));
    repaint();
}
int Slider::calcSliderAutoLength()
{
    if (maximum - minimum + 1 == 0) return getLength();
    else return (getLength() / (maximum - minimum + 1));
}
int Slider::calcRealValueFromPixel(int pxValue)
{
    if (getLength() - getSliderLength() == 0) return 0;
    else return (minimum + pxValue * (maximum - minimum) / (getLength() - getSliderLength()));
}
int Slider::calcPixelValueFromReal(int realValue)
{
    if (maximum - minimum == 0) return 0;
    return ((realValue - minimum) * (getLength() - getSliderLength()) / (maximum - minimum));
}
int Slider::getOrientationValue(int hValue, int vValue)
{
    if (orientation == Qt::Horizontal) return hValue;
    else return vValue;
}
