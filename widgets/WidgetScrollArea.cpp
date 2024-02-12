#include "WidgetScrollArea.h"
#include <QScrollBar>
#include <QEvent>

QSize WidgetScrollArea::sizeHint() const {
  return QSize(
    this->viewportSizeHint().width(),
    this->viewportSizeHint().height() +
      (this->horizontalScrollBar()->isVisible() ? this->horizontalScrollBar()->height() : 0)
  );
}

QSize WidgetScrollArea::minimumSizeHint() const {
  return QSize(0, 0);
}

bool WidgetScrollArea::event(QEvent *event)
{
  auto result = QScrollArea::event(event);
  if (event->type() == QEvent::LayoutRequest) {
    updateGeometry(); // makes this scrollarea resize based on new size hint
  }
  return result;
}
