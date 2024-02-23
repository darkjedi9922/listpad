#pragma once

#include <QLineEdit>
#include <QLoggingCategory>

/**
 * Line edit with sizeHint adjusted to its contents
 */
class LineEdit : public QLineEdit
{
public:
  enum UpdateGeometryMode {EDITING_FINISHED, TEXT_CHANGED};

  LineEdit(QWidget *parent = nullptr);
  LineEdit(const QString &text, QWidget *parent = nullptr);

  virtual QSize sizeHint() const;
  virtual QSize minimumSizeHint() const;

  using QLineEdit::cursorRect;
  void setUpdateGeometryMode(UpdateGeometryMode mode);

protected:
  virtual void showEvent(QShowEvent *);

private:
  void initialize();
  QLoggingCategory loggingCategory;
  UpdateGeometryMode updateGeometryMode;
  QMetaObject::Connection updateGeometryConnection;
};
