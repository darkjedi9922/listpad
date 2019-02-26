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

    QObject::connect(ui->addCategory, SIGNAL(clicked(bool)),
                     this, SLOT(startCategoryAdding()));
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
        setupCategoryButton(button, table->getId());
        ui->ltCategories->addWidget(button);
    }
}

void Menu::removeUiButtons()
{
    QLayoutItem *child;
    while ((child = ui->ltCategories->takeAt(0)) != nullptr) {
        delete child;
    }
}

/**
 * Метод для первоначальной настройки новой кнопки категории.
 */
void Menu::setupCategoryButton(MenuButton *button, int newId)
{
    button->setMenuId(newId);
    QObject::connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClickedSlot()));
}

/**
 * Чисто визуальное добавление кнопки.
 */
void Menu::addButton(MenuButton *button)
{
    ui->ltCategories->addWidget(button);
}

int Menu::findMaxCategoryId() const
{
    int max = 0;
    QList<int> categoryIds = categories.keys();
    for (int id : categoryIds) {
        if (id > max) max = id;
    }
    return max;
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

void Menu::startCategoryAdding()
{
    // Эта кнопка потом будет привязана к слою, и при очистке слоя, автоматически
    // будет вызван delete.
    MenuButton *newButton = new MenuButton();
    newButton->setText("Новая категория");

    addButton(newButton);
    QObject::connect(newButton, SIGNAL(edited()), SLOT(categoryEditedAfterAdding()));
    newButton->startEditing();
}

/**
 * Одноразовый слот, который нужно активровать после редактирования категории при ее
 * добавлении. После этого он отключает сам себя.
 */
void Menu::categoryEditedAfterAdding()
{
    MenuButton* newButton = static_cast<MenuButton*>(QObject::sender());

    // Не вызываем этот метод сразу при добавлении, потому что он может добавить
    // обработчик события завершения редактирования, который отправит запрос на
    // сохранение нового имени. Так как эта кнопка еще только в процессе добавления,
    // нам не нужно ее переименовывать (в теории этой категории еще нет).
    this->setupCategoryButton(newButton, this->findMaxCategoryId() + 1);

    QObject::disconnect(newButton, SIGNAL(edited()),
                        this, SLOT(categoryEditedAfterAdding()));
}