#pragma once

#include <QScrollBar>
#include <QLoggingCategory>

/**
 * Scroll bar implementation without arrow elements 
 */
class SimpleScrollBar : public QScrollBar
{
public:
  SimpleScrollBar(Qt::Orientation);

protected:
  virtual bool event(QEvent *event);

private:
  double calculateScrollSpeed();
  int getPointValue(const QPoint&);
  int getRectSize(const QRect&);

  QLoggingCategory loggingCategory;
  QPoint initialMousePos;
  int initialSliderPos;
  bool isMoving;
};
