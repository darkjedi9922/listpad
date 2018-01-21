#include "Menu.h"

// ==== PUBLIC ====
Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu),
    checkedButton(NULL)
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

void Menu::uncheckButton()
{
    if (checkedButton) {
        checkedButton->setChecked(false);
        emit buttonUnchecked(checkedButton);
        checkedButton = NULL;
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
    QObject::connect(ui->films, SIGNAL(clicked(bool)), this, SLOT(buttonClickedSlot()));
    QObject::connect(ui->serials, SIGNAL(clicked(bool)), this, SLOT(buttonClickedSlot()));
    QObject::connect(ui->books, SIGNAL(clicked(bool)), this, SLOT(buttonClickedSlot()));
}
void Menu::setupMenuIds()
{
    int menus = ui->verticalLayout->count();
    for (int i = 0; i < menus; i++) {
        QLayoutItem *item = ui->verticalLayout->itemAt(i);
        if (item && item->widget()) static_cast<MenuButton*>(item->widget())->setMenuId(i);
    }
}

void Menu::setButtonChecked(MenuButton *button)
{
    if (checkedButton != button) {
        if (checkedButton) checkedButton->setChecked(false);
        button->setChecked(true);
        checkedButton = button;
        emit newButtonChecked(button);
    }
}

// ==== PRIVATE SLOTS ====
void Menu::buttonClickedSlot()
{
    MenuButton* button = static_cast<MenuButton*>(QObject::sender());
    // Qt кнопки сами выключают check'нутость даже, если они уже check'нутые
    // Нам это не нужно, так что включим check'нутость обратно (костыль)
    if (button == checkedButton) button->setChecked(true);
    else setButtonChecked(button);
}
