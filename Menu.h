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
    void deleteCategory(MenuButton *category);

    void checkButton(MenuButton *button);
    void uncheckButton();

    MenuButton* getCheckedButton() const;

protected:
    virtual void mouseReleaseEvent(QMouseEvent *);

private:
    void setupUiButtons();
    void removeUiButtons();
    void setupCategoryButton(MenuButton *, int newId);
    void addButton(MenuButton *);
    void startButtonEditing(MenuButton *);
    int findMaxCategoryId() const;
    void setupContextMenu();

    Ui::Menu *ui;
    QMenu *contextMenu;
    QMap<int, Core::Table*> categories;
    MenuButton *checkedButton;
    MenuButton *contextMenuButton;

private slots:
    void buttonClickedSlot();
    void startCategoryAdding();
    void categoryEditedAfterAdding();
    void openCategoryContextMenu(const QPoint &pos);
};

#endif // MENU_H
