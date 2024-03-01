#include "StarButton.h"
#include "ui_StarButton.h"

StarButton::StarButton(QWidget *parent) :
  QPushButton(parent),
  ui(new Ui::StarButton)
{
  ui->setupUi(this);
  hideIconIfUnchecked(); // do not show icon initially
  QObject::connect(this, &QPushButton::toggled, [this](bool checked) {
    showIcon();
  });
}
StarButton::~StarButton()
{
  delete ui;
}

QSize StarButton::sizeHint() const
{
  return QSize(30, 30);
}

void StarButton::showIcon()
{
  setText(isChecked() ? "▾" : ""); // fontello icons
}
void StarButton::hideIconIfUnchecked()
{
  if (!isChecked()) {
    setText("");
  }
}

void StarButton::enterEvent(QEvent *)
{
  setCursor(Qt::PointingHandCursor);
}
void StarButton::leaveEvent(QEvent *)
{
  setCursor(Qt::ArrowCursor);
}
