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
    void categoryAdded(int id);

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

    void setCategories(const QMap<int, QString> &);
    void deleteCategory(MenuButton *category);
    QString getCategoryName(int id) const;

    void checkButton(MenuButton *button);
    void uncheckButton();

    MenuButton* getCheckedButton() const;

protected:
    virtual void mouseReleaseEvent(QMouseEvent *);

private:
    void setupUiButtons();
    void removeUiButtons();
    void setupCategoryButton(MenuButton *, int newId);
    void addCategory(MenuButton *);
    void startButtonEditing(MenuButton *);
    int findMaxCategoryId() const;
    void setupContextMenu();

    Ui::Menu *ui;
    QMenu *contextMenu;
    QMap<int, QString> categories;
    MenuButton *checkedButton;
    MenuButton *contextMenuButton;

private slots:
    void buttonClickedSlot();
    void startCategoryAdding();
    void categoryEditedAfterAdding();
    void openCategoryContextMenu(const QPoint &pos);
};

#endif // MENU_H
