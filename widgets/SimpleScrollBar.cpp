#include <QScrollArea>
#include <QMouseEvent>
#include <QEvent>
#include <QStyle>
#include <QStyleOptionSlider>
#include "SimpleScrollBar.h"

SimpleScrollBar::SimpleScrollBar(Qt::Orientation orientation) :
  QScrollBar(orientation),
  loggingCategory("SimpleScrollBar")
{};

bool SimpleScrollBar::event(QEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick) {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect sliderRect = style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarSlider, this);
    isMoving = true;
    initialMousePos = mouseEvent->pos();
    if (sliderRect.contains(mouseEvent->pos())) {
      initialSliderPos = this->sliderPosition();
    } else {
      int newValue = qBound(
        minimum(),
        (int)((getPointValue(mouseEvent->pos()) - getRectSize(sliderRect) / 2.0) * calculateScrollSpeed()),
        maximum()
      );
      setValue(newValue);
      initialSliderPos = newValue;
    }
    return true;
  } else if (event->type() == QEvent::MouseMove) {
    if (isMoving) {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
      int delta = getPointValue(mouseEvent->pos()) - getPointValue(initialMousePos);
      this->setSliderPosition(initialSliderPos + delta * calculateScrollSpeed());
    }
    return true;
  } else if (event->type() == QEvent::MouseButtonRelease) {
    isMoving = false;
    return true;
  }
  return QScrollBar::event(event);
}

double SimpleScrollBar::calculateScrollSpeed()
{
  auto size = getRectSize(rect());
  auto innerWidgetHeight = maximum() - minimum() + size;
  double scrollSpeedCoefficient = (double)innerWidgetHeight / (double)size;
  return scrollSpeedCoefficient;
}
int SimpleScrollBar::getPointValue(const QPoint &point)
{
  switch (orientation()) {
    case Qt::Vertical: return point.y();
    case Qt::Horizontal: return point.x();
    default: throw "Unknown orientation value";
  }
}
int SimpleScrollBar::getRectSize(const QRect &rect)
{
  switch (orientation()) {
    case Qt::Vertical: return rect.height();
    case Qt::Horizontal: return rect.width();
    default: throw "Unknown orientation value";
  }
}
