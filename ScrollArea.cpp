#include "ScrollArea.h"

ScrollArea::ScrollArea(QWidget *parent) :
    QWidget(parent),
    vScrollBar(new ScrollBar(Qt::Vertical, this)),
    widget(nullptr)
{
    vScrollBar->resize(15, height());

    QObject::connect(vScrollBar->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(vSliderValueChanged(int)));
}
QSize ScrollArea::sizeHint() const
{
    if (widget) return widget->sizeHint();
    else return QSize(0, 0);
}

void ScrollArea::update()
{
    vScrollBar->resize(vScrollBar->width(), height());
    vScrollBar->move(width() - vScrollBar->width(), 0);

    if (widget) {
        if (widget->height() - height() <= 0) {
            vScrollBar->getSlider()->setSliderFixedLength(0);
            vScrollBar->setMaximum(0);
            vScrollBar->hide();
        } else {
            vScrollBar->show();
            vScrollBar->getSlider()->setSliderFixedLength(height() * vScrollBar->getSlider()->height() / widget->height());
            vScrollBar->setMaximum(widget->height() - height());
        }
    } else {
        vScrollBar->hide();
    }
}
void ScrollArea::setWidget(QWidget *widget)
{
    this->widget = widget;
    widget->setParent(this);
    widget->show();
    vScrollBar->raise();
}
ScrollBar* ScrollArea::getVerticalScrollBar() const
{
    return vScrollBar;
}

void ScrollArea::resizeEvent(QResizeEvent *)
{
    update();
}

void ScrollArea::vSliderValueChanged(int value)
{
    if (widget) widget->move(0, -value);
}
