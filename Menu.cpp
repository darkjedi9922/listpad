#include "Menu.h"
#include "tools/LayoutIterator.h"

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
    LayoutIterator<MenuButton> buttonIterator(ui->buttonLayout);
    while (auto button = buttonIterator.next()) {
        QObject::connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClickedSlot()));
    }
}
void Menu::setupMenuIds()
{
    LayoutIterator<MenuButton> buttonIt(ui->buttonLayout);
    int index = 0;
    while (auto button = buttonIt.next()) {
        button->setMenuId(index++);
    };
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
