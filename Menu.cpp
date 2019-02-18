#include "Menu.h"

// ==== PUBLIC ====
Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu),
    checkedButton(nullptr)
{
    ui->setupUi(this);
    setFixedWidth(240);
    setupConnectings();
    setupMenuIds();
}
Menu::~Menu()
{
    delete ui;
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
void Menu::mouseReleaseEvent(QMouseEvent *)
{
    uncheckButton();
}

// ==== PRIVATE ====
void Menu::setupConnectings()
{
    walkButtons([](Menu* self, MenuButton* button, int) {
        QObject::connect(button, SIGNAL(clicked(bool)), self, SLOT(buttonClickedSlot()));
    });
}
void Menu::setupMenuIds()
{
    walkButtons([](Menu*, MenuButton* button, int index) {
        button->setMenuId(index);
    });
}

void Menu::walkButtons(void (*handler)(Menu* self, MenuButton*, int index))
{
    int buttonCount = ui->buttonLayout->count();
    for (int i = 0; i < buttonCount; i++) {
        QLayoutItem *item = ui->buttonLayout->itemAt(i);
        if (item && item->widget()) {
            auto button = static_cast<MenuButton*>(item->widget());
            handler(this, button, i);
        }
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
