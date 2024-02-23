#include "LineEdit.h"
#include <QTimer>

LineEdit::LineEdit(QWidget *parent) :
  QLineEdit(parent),
  loggingCategory("LineEdit")
{
  initialize();
}
LineEdit::LineEdit(const QString &text, QWidget *parent) :
  QLineEdit(text, parent),
  loggingCategory("LineEdit")
{
  initialize();
}
void LineEdit::initialize()
{
  setUpdateGeometryMode(UpdateGeometryMode::EDITING_FINISHED);
}

QSize LineEdit::sizeHint() const
{
  QFontMetrics fm(font());
  // the text width lacks some pixels for some reason, so add them manually
  return QSize(fm.horizontalAdvance(text()) + 5, QLineEdit::sizeHint().height());
}
QSize LineEdit::minimumSizeHint() const
{
  return QSize();
}

void LineEdit::showEvent(QShowEvent *)
{
  setCursorPosition(0);
  updateGeometry();
}

void LineEdit::setUpdateGeometryMode(UpdateGeometryMode mode)
{
  updateGeometryMode = mode;
  QObject::disconnect(updateGeometryConnection);
  auto updateGeometryLambda = [&]() {
    updateGeometry();
  };
  switch (mode) {
    case EDITING_FINISHED:
      updateGeometryConnection = QObject::connect(this, &QLineEdit::editingFinished, updateGeometryLambda);
      break;
    case TEXT_CHANGED:
      updateGeometryConnection = QObject::connect(this, &QLineEdit::textChanged, updateGeometryLambda);
      break;
  }
}
