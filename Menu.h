#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QLoggingCategory>
#include <QMenu>
#include "ui_Menu.h"

namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

signals:
    void buttonChecked(MenuItem*);
    void buttonUnchecked(MenuItem*);

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

    int countButtons();

    void setCategories(const QMap<int, QString> &);
    void deleteCategory(MenuItem *category);
    QString getCategoryName(int id) const;

    void checkButton(MenuItem *button);
    void uncheckButton();

    MenuItem* getCheckedButton() const;

private:
    void setupUiButtons();
    void removeUiButtons();
    void setupCategoryButton(MenuItem *, int newId);
    void addCategory(MenuItem *);
    void startButtonEditing(MenuItem *);
    int findMaxCategoryId() const;
    void setupContextMenu();
    MenuItem* allocCategoryButton();

    Ui::Menu *ui;
    QMenu *contextMenu;
    QMap<int, QString> categories;
    MenuItem *checkedButton;
    MenuItem *contextMenuButton;
    QLoggingCategory loggingCategory;

private slots:
    void buttonClickedSlot();
    void startCategoryAdding();
    void categoryEditedAfterAdding();
    void openCategoryContextMenu(const QPoint &pos);
};

#endif // MENU_H
