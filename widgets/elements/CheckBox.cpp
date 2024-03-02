#include "CheckBox.h"
#include "ui_CheckBox.h"

CheckBox::CheckBox(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::CheckBox)
{
  ui->setupUi(this);
  ui->box->installEventFilter(this);
  
  auto markSizePolicy = ui->mark->sizePolicy();
  markSizePolicy.setRetainSizeWhenHidden(true);
  ui->mark->setSizePolicy(markSizePolicy);
}
CheckBox::~CheckBox()
{
  delete ui;
}

void CheckBox::setChecked(bool checked)
{
  this->checked = checked;
  checked ? ui->mark->show() : ui->mark->hide();
  emit toggled(checked);
}
bool CheckBox::isChecked() const
{
  return checked;
}
void CheckBox::setText(const QString& text)
{
  ui->label->setText(text);
}
QString CheckBox::getText() const
{
  return ui->label->text();
}
void CheckBox::toggle()
{
  setChecked(!checked);
}

bool CheckBox::eventFilter(QObject *obj, QEvent *event)
{
  switch (event->type()) {
    case QEvent::MouseButtonRelease: toggle(); return true;
  }
  return false;
}
