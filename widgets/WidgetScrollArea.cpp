#include "WidgetScrollArea.h"
#include <QScrollBar>
#include <QEvent>

WidgetScrollArea::WidgetScrollArea(QWidget *parent) :
  QScrollArea(parent),
  loggingCategory("WidgetScrollArea")
{}

QSize WidgetScrollArea::sizeHint() const
{
  return QSize(
    this->viewportSizeHint().width() +
      (this->verticalScrollBar()->isVisible() ? this->verticalScrollBar()->width() : 0),
    this->viewportSizeHint().height() +
      (this->horizontalScrollBar()->isVisible() ? this->horizontalScrollBar()->height() : 0)
  );
}
QSize WidgetScrollArea::minimumSizeHint() const
{
  return QSize(0, 0);
}

bool WidgetScrollArea::event(QEvent *event)
{
  auto result = QScrollArea::event(event);
  if (event->type() == QEvent::LayoutRequest) {
    qCDebug(loggingCategory)
      << "Updating geometry,"
      << "sizeHint" << sizeHint()
      << "widget.sizeHint" << (widget() ? widget()->sizeHint() : QSize());
    updateGeometry(); // makes this scrollarea resize based on new size hint
  }
  return result;
}
