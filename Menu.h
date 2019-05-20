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

    /**
     * Выбрасывается, при чисто визуальном добавлении.
     */
    void categoryReallyAdded(int id);

    /**
     * Выбрасывается, при визуальном добавлении через контекстное меню.
     */
    void categoryAdded(int id);

    /**
     * Выбрасывается при переименовании категории, которая уже была добавлена (то
     * есть, начальное переименование при создании - не в счет).
     */
    void categoryRenamed(int id);

    void categoryDeleted(int id);

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

    virtual QSize sizeHint() const;

    int countButtons();

    void setCategories(const QMap<int, QString> &);
    void deleteCategory(MenuButton *category);
    QString getCategoryName(int id) const;

    void checkButton(MenuButton *button);
    void uncheckButton();

    MenuButton* getCheckedButton() const;

private:
    void setupUiButtons();
    void removeUiButtons();
    void setupCategoryButton(MenuButton *, int newId);
    void addCategory(MenuButton *);
    void startButtonEditing(MenuButton *);
    int findMaxCategoryId() const;
    void setupContextMenu();
    MenuButton* allocCategoryButton();

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
