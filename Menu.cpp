#include "Menu.h"
#include "tools/LayoutIterator.h"
#include <QMouseEvent>

// ==== PUBLIC ====
Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu),
    checkedButton(nullptr)
{
    ui->setupUi(this);
    ui->addCategory->setCheckable(false);
    setFixedWidth(240);
}
Menu::~Menu()
{
    delete ui;
}

void Menu::setCategories(const QMap<int, Core::Table *> &categories)
{
    this->categories = categories;
    setupUiButtons();
}

void Menu::checkButton(MenuButton *button)
{
    if (checkedButton != button) {
        if (checkedButton) uncheckButton();
        button->setChecked(true);
        checkedButton = button;
        emit buttonChecked(button);
    }
}
void Menu::uncheckButton()
{
    if (checkedButton) {
        checkedButton->setChecked(false);
        emit buttonUnchecked(checkedButton);
        checkedButton = nullptr;
    }
}
MenuButton* Menu::getCheckedButton() const
{
    return checkedButton;
}

// ==== EVENTS ====
void Menu::mouseReleaseEvent(QMouseEvent *e)
{
    // addCategory кнопка в меню находится ниже других, поэтому ориентируемся на нее.
    if (e->y() > ui->addCategory->geometry().bottom())
        uncheckButton();
}

// ==== PRIVATE ====
void Menu::setupUiButtons()
{
    removeUiButtons();
    foreach (Core::Table* table, categories) {
        MenuButton* button = new MenuButton;
        button->setText(table->getName());
        button->setMenuId(table->getId());
        ui->ltCategories->addWidget(button);
        QObject::connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClickedSlot()));
    }
}

void Menu::removeUiButtons()
{
    QLayoutItem *child;
    while ((child = ui->ltCategories->takeAt(0)) != nullptr) {
        delete child;
    }
}

// ==== PRIVATE SLOTS ====
void Menu::buttonClickedSlot()
{
    MenuButton* button = static_cast<MenuButton*>(QObject::sender());
    // Qt кнопки сами выключают check'нутость даже, если они уже check'нутые
    // Нам это не нужно, так что включим check'нутость обратно (костыль)
    if (button == checkedButton) button->setChecked(true);
    else checkButton(button);
}
