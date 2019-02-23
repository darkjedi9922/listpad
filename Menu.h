#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include "ui_Menu.h"
#include <QList>
#include "core/Table.h"

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

    void setCategories(const QMap<int, Core::Table*> &);

    void checkButton(MenuButton *button);
    void uncheckButton();
    MenuButton* getCheckedButton() const;

protected:
    virtual void mouseReleaseEvent(QMouseEvent *);

private:
    void setupUiButtons();
    void removeUiButtons();

    Ui::Menu *ui;
    QMap<int, Core::Table*> categories;
    MenuButton *checkedButton;

private slots:
    void buttonClickedSlot();
};

#endif // MENU_H
