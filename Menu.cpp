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
}
Menu::~Menu()
{
    delete ui;
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

void Menu::setButtonChecked(MenuButton *button)
{
    if (checkedButton != button) {
        if (checkedButton) checkedButton->setChecked(false);
        button->setChecked(true);
        checkedButton = button;
        emit newButtonChecked(button);
    }
}
void Menu::uncheckButton()
{
    if (checkedButton) {
        checkedButton->setChecked(false);
        emit buttonUnchecked(checkedButton);
        checkedButton = NULL;
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
