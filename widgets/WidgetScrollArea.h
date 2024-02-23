#pragma once

#include <QScrollArea>
#include <QLoggingCategory>

class WidgetScrollArea : public QScrollArea
{
  Q_OBJECT

public:
  WidgetScrollArea(QWidget *parent = nullptr);

  virtual QSize sizeHint() const;
  virtual QSize minimumSizeHint() const;

protected:
  bool event(QEvent *event) override;

private:
  QLoggingCategory loggingCategory;
};
