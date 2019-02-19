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
    void buttonChecked(MenuButton*);
    void buttonUnchecked(MenuButton*);

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

    void checkButton(MenuButton *button);
    void uncheckButton();
    MenuButton* getCheckedButton() const;

protected:
    virtual void mouseReleaseEvent(QMouseEvent *);

private:
    void setupConnectings();
    void setupMenuIds();

    Ui::Menu *ui;
    MenuButton *checkedButton;

private slots:
    void buttonClickedSlot();
};

#endif // MENU_H
