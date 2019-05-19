#include "Menu.h"
#include "tools/LayoutIterator.h"
#include <QMouseEvent>
#include <QMenu>

// ==== PUBLIC ====
Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu),
    contextMenu(new QMenu),
    checkedButton(nullptr),
    contextMenuButton(nullptr)
{
    ui->setupUi(this);
    ui->addCategory->setIcon(QIcon(":/images/plus-circle.png"));
    ui->addCategory->setCheckable(false);

    QObject::connect(ui->addCategory, SIGNAL(clicked(bool)),
                     this, SLOT(startCategoryAdding()));

    setupContextMenu();
}
Menu::~Menu()
{
    delete contextMenu;
    contextMenu = nullptr;

    delete ui;
    ui = nullptr;
}

QSize Menu::sizeHint() const
{
    return QSize(
        QWidget::sizeHint().width(),
        ui->ltButtons->sizeHint().height()
    );
}

int Menu::countButtons()
{
    return ui->ltCategories->count() + 1;
}

void Menu::setCategories(const QMap<int, QString> &categories)
{
    this->categories = categories;
    setupUiButtons();
}

void Menu::deleteCategory(MenuButton* category)
{
    category->hide();
    int id = category->getMenuId();
    ui->ltCategories->takeAt(ui->ltCategories->indexOf(category));
    categories.remove(id);
    emit categoryDeleted(id);
}

QString Menu::getCategoryName(int id) const
{
    return categories[id];
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

    QMapIterator<int, QString> i(categories);
    while (i.hasNext()) {
        i.next();
        MenuButton* button = allocCategoryButton();
        button->setText(i.value());
        setupCategoryButton(button, i.key());
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
 * Метод для первоначальной настройки новой кнопки категории, которая уже является
 * добавленной и переименованой (точнее, начально именованой).
 */
void Menu::setupCategoryButton(MenuButton *button, int newId)
{
    button->setMenuId(newId);
    button->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(button, SIGNAL(clicked(bool)), this, SLOT(buttonClickedSlot()));
    QObject::connect(button, SIGNAL(customContextMenuRequested(const QPoint &)),
                     SLOT(openCategoryContextMenu(const QPoint &)));
    QObject::connect(button, &MenuButton::edited, [=] {
        categories[newId] = button->text();
        emit this->categoryRenamed(newId);
    });
}

/**
 * Чисто визуальное добавление кнопки.
 */
void Menu::addCategory(MenuButton *category)
{
    ui->ltCategories->addWidget(category);
    emit categoryReallyAdded(category->getMenuId());
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

void Menu::setupContextMenu()
{
    contextMenu->setStyleSheet("QMenu {"
                               "    background: #05080e;"
                               "    font-family: Calibri;"
                               "    font-size: 12pt;"
                               "    font-weight: bold;"
                               "    color: #249dcd;"
                               "}"
                               "::item {"
                               "    padding: 10px 20px;"
                               "    background: transparent;"
                               "}"
                               "::item:selected {"
                               "    background: #203153;"
                               "}");

    contextMenu->addAction("Переименовать", [=] {
        contextMenuButton->startEditing();
    });
    contextMenu->addAction("Удалить", [=] {
        deleteCategory(contextMenuButton);
    });
}

MenuButton* Menu::allocCategoryButton()
{
    QIcon icon;
    icon.addFile(":/images/folder.png", QSize(), QIcon::Normal);
    icon.addFile(":/images/checkedFolder.png", QSize(), QIcon::Selected);

    auto button = new MenuButton;
    button->setIcon(icon);
    return button;
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
    // Эта кнопка привязывается к слою, и при очистке слоя, автоматически будет
    // вызван delete.
    MenuButton *newButton = allocCategoryButton();
    newButton->setText("Новая категория");

    addCategory(newButton);
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
    int id = this->findMaxCategoryId() + 1;
    categories.insert(id, newButton->text());
    setupCategoryButton(newButton, id);

    QObject::disconnect(newButton, SIGNAL(edited()),
                        this, SLOT(categoryEditedAfterAdding()));

    emit categoryAdded(id);
}

void Menu::openCategoryContextMenu(const QPoint &pos)
{
    MenuButton* button = static_cast<MenuButton*>(QObject::sender());
    contextMenuButton = button;
    contextMenu->popup(button->mapToGlobal(pos));
}