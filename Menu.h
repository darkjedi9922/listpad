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

public:
    explicit Menu(QWidget *parent = 0);
    ~Menu();

    Ui::Menu *ui;

signals:
    void clicked();

protected:
    virtual void mouseReleaseEvent(QMouseEvent *);
};

#endif // MENU_H
