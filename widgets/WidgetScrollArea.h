#pragma once

#include <QScrollArea>

class WidgetScrollArea : public QScrollArea
{
  Q_OBJECT

public:
  using QScrollArea::QScrollArea;

  virtual QSize sizeHint() const;
  virtual QSize minimumSizeHint() const;

protected:
  bool event(QEvent *event) override;
};
