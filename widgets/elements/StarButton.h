#ifndef STARBUTTON_H
#define STARBUTTON_H

#include <QPushButton>

namespace Ui {
  class StarButton;
}

class StarButton : public QPushButton
{
  Q_OBJECT

public:
  explicit StarButton(QWidget *parent = nullptr);
  ~StarButton();

  void showIcon();
  void hideIconIfUnchecked();

protected:
  virtual void enterEvent(QEvent *);
  virtual void leaveEvent(QEvent *);

private:
  Ui::StarButton *ui;
};

#endif // STARBUTTON_H
