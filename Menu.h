#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include "ui_Menu.h"

namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

signals:
    void newButtonChecked(MenuButton*);
    void buttonUnchecked(MenuButton*);

public:
    explicit Menu(QWidget *parent = 0);
    ~Menu();

    MenuButton* getCheckedButton() const;

protected:
    virtual void mouseReleaseEvent(QMouseEvent *);

private:
    void setupConnectings();

    void setButtonChecked(MenuButton *button);
    void uncheckButton();

    Ui::Menu *ui;
    MenuButton *checkedButton;

private slots:
    void buttonClickedSlot();
};

#endif // MENU_H
