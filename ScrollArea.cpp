#include "ScrollArea.h"
#include <QEvent>

ScrollArea::ScrollArea(QWidget *parent) :
    QWidget(parent),
    vScrollBar(new ScrollBar(Qt::Vertical, this)),
    loggingCategory("ScrollArea"),
    widget(nullptr)
{
    vScrollBar->resize(15, height());

    QObject::connect(vScrollBar->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(vSliderValueChanged(int)));
}
ScrollArea::~ScrollArea()
{
    delete vScrollBar;
    vScrollBar = nullptr;
}
QSize ScrollArea::sizeHint() const
{
    if (widget) {
        qCDebug(loggingCategory) << "sizeHint" << widget->sizeHint();
        return widget->sizeHint();
    } else {
        return QSize(0, 0);
    }
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
    qCDebug(loggingCategory) << "Resize event received";
    update();
}
bool ScrollArea::event(QEvent *event) {
    if (event->type() == QEvent::LayoutRequest) {
        qCDebug(loggingCategory) << "Layout request event received";
        if (widget) {
            qCDebug(loggingCategory) << "layoutRequest.widget.sizeHint" << widget->sizeHint();
            qCDebug(loggingCategory) << "layoutRequest.currentSize" << size();

            int vScrollBarWidth = vScrollBar->width();
            if (vScrollBar->isHidden()) {
                vScrollBarWidth = 0;
            }
            int width = this->width() - vScrollBarWidth;
            int height = widget->sizeHint().height();
            widget->resize(width, height);
            update();

            updateGeometry();
            return true;
        }
    }
    return QWidget::event(event);
}

void ScrollArea::vSliderValueChanged(int value)
{
    if (widget) widget->move(0, -value);
}
