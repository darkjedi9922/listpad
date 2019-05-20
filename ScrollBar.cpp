#include "ScrollBar.h"

ScrollBar::ScrollBar(Qt::Orientation orientation, QWidget *parent) : QWidget(parent)
{
    layout = new QBoxLayout(QBoxLayout::LeftToRight);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    slider = new Slider(orientation);

    setOrientation(orientation);
    setMinimum(0);

    this->setLayout(layout);
    this->layout->addWidget(slider);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
void ScrollBar::setOrientation(Qt::Orientation orientation)
{
    if (orientation == Qt::Horizontal) layout->setDirection(QBoxLayout::LeftToRight);
    else layout->setDirection(QBoxLayout::TopToBottom);
    slider->setOrientation(orientation);
}
void ScrollBar::setMinimum(int min)
{
    slider->setMinimum(min);
}
void ScrollBar::setMaximum(int max)
{
    slider->setMaximum(max);
}
void ScrollBar::setSingleStep(int step)
{
    slider->setSingleStep(step);
}

int ScrollBar::getMinimum()
{
    return slider->getMinimum();
}
int ScrollBar::getMaximum()
{
    return slider->getMaximum();
}
Slider* ScrollBar::getSlider() const
{
    return slider;
}

void ScrollBar::setValue(int value)
{
    slider->setValue(value);
}
int ScrollBar::getValue() const
{
    return slider->getValue();
}
