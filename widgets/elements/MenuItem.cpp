#include "MenuItem.h"
#include "ui_MenuItem.h"
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <QCursor>

MenuItem::MenuItem(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MenuItem),
  loggingCategory("MenuItem"),
  checkable(false),
  checked(false)
{
  ui->setupUi(this);
  ui->label->setAttribute(Qt::WA_TransparentForMouseEvents, true);

  originalQss = styleSheet();
  QObject::connect(ui->label, SIGNAL(editingFinished()), this, SLOT(endEditing()));
}
MenuItem::~MenuItem()
{
  delete ui;
}

void MenuItem::setCheckable(bool checkable)
{
  this->checkable = checkable;
  setChecked(false);
}
void MenuItem::setChecked(bool checked)
{
  if (!checkable && checked) {
    qCWarning(loggingCategory) << "Can't check as not checkable";
    return;
  }
  if (this->checked == checked) {
    qCDebug(loggingCategory) << "Checked is already" << checked;
    return;
  }
  qCDebug(loggingCategory) << "Checked is" << this->checked << "before changing";
  this->checked = checked;
  updateStyleSheet();
  emit checkedStateChanged(checked);
  qCInfo(loggingCategory) << "Checked" << checked;
}
bool MenuItem::isCheckable() const
{
  return this->checkable;
}
bool MenuItem::isChecked() const
{
  return this->checked;
}
void MenuItem::setMenuId(int id)
{
  menuId = id;
}
int MenuItem::getMenuId() const
{
  return menuId;
}
void MenuItem::setText(const QString &text)
{
  ui->label->setText(text);
}
QString MenuItem::getText() const
{
  return ui->label->text();
}
void MenuItem::setIconPath(const QString &icon)
{
  overridenIconPath = icon;
  updateStyleSheet();
}
QString MenuItem::getOverridenIconPath() const
{
  return overridenIconPath;
}
void MenuItem::setCheckedIconPath(const QString &icon)
{
  overridenCheckedIconPath = icon;
  updateStyleSheet();
}
QString MenuItem::getOverridenCheckedIconPath() const
{
  return overridenCheckedIconPath;
}

void MenuItem::startEditing()
{
  ui->label->setAttribute(Qt::WA_TransparentForMouseEvents, false);
  ui->label->setReadOnly(false);
  ui->label->setFocus();
  ui->label->selectAll();
}
void MenuItem::endEditing()
{
  ui->label->setAttribute(Qt::WA_TransparentForMouseEvents, true);
  ui->label->setReadOnly(true);
  ui->label->setSelection(0, 0);
  emit edited();
}

void MenuItem::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

  if (ui->label->isReadOnly()) {
    ui->label->deselect();
  }
}
void MenuItem::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton && checkable && !checked) {
    qCInfo(loggingCategory) << "Checking on mouse press event";
    setChecked(true);
  }
  QWidget::mousePressEvent(event);
}
void MenuItem::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) {
    emit clicked();
  }
  QWidget::mouseReleaseEvent(event);
}

void MenuItem::updateStyleSheet()
{
  QString result = originalQss;
  if (!overridenIconPath.isEmpty()) {
    result += QString("#icon {background: url(%1);}").arg(overridenIconPath);
  }
  if (!overridenCheckedIconPath.isEmpty()) {
    result += QString("QWidget[checked=\"true\"] #icon {background: url(%1);}").arg(overridenCheckedIconPath);
  }
  setStyleSheet(result);
}
