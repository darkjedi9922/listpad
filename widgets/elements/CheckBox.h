#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <QWidget>

namespace Ui {
  class CheckBox;
}

class CheckBox : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(bool checked READ isChecked WRITE setChecked NOTIFY toggled)
  Q_PROPERTY(QString text READ getText WRITE setText)

public:
  explicit CheckBox(QWidget *parent = nullptr);
  ~CheckBox();

  void setChecked(bool checked);
  bool isChecked() const;
  void setText(const QString& text);
  QString getText() const;
  void toggle();

signals:
  void toggled(bool checked);

protected:
  bool eventFilter(QObject *object, QEvent *event) override;

private:
  Ui::CheckBox *ui;
  bool checked;
};

#endif // CHECKBOX_H
