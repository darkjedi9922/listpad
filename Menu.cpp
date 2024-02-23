#include "Menu.h"
#include "tools/LayoutIterator.h"
#include "tools/EventCallback.h"
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>

// ==== PUBLIC ====
Menu::Menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu),
    contextMenu(new QMenu),
    checkedButton(nullptr),
    contextMenuButton(nullptr),
    loggingCategory("Menu")
{
    ui->setupUi(this);

    QObject::connect(ui->addCategory, SIGNAL(clicked()),
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

int Menu::countButtons()
{
    return ui->ltCategories->count() + 1;
}

void Menu::setCategories(const QMap<int, QString> &categories)
{
    this->categories = categories;
    setupUiButtons();
}

void Menu::deleteCategory(MenuItem* category)
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

void Menu::checkButton(MenuItem *button)
{
    if (checkedButton != button) {
        qCInfo(loggingCategory) << "Checking button" << button->getMenuId();
        if (checkedButton) uncheckButton();
        button->setChecked(true);
        checkedButton = button;
        emit buttonChecked(button);
    }
}
void Menu::uncheckButton()
{
    if (checkedButton) {
        qCInfo(loggingCategory) << "Unchecking button" << checkedButton->getMenuId();
        checkedButton->setChecked(false);
        emit buttonUnchecked(checkedButton);
        checkedButton = nullptr;
    }
}
MenuItem* Menu::getCheckedButton() const
{
    return checkedButton;
}

void Menu::paintEvent(QPaintEvent *)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// ==== PRIVATE ====
void Menu::setupUiButtons()
{
    removeUiButtons();

    QMapIterator<int, QString> i(categories);
    while (i.hasNext()) {
        i.next();
        MenuItem* button = allocCategoryButton();
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
void Menu::setupCategoryButton(MenuItem *button, int newId)
{
    button->setMenuId(newId);
    button->setCheckable(true);
    button->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(button, SIGNAL(clicked()), this, SLOT(buttonClickedSlot()));
    QObject::connect(button, SIGNAL(customContextMenuRequested(const QPoint &)),
                     SLOT(openCategoryContextMenu(const QPoint &)));
    QObject::connect(button, &MenuItem::edited, [=] {
        categories[newId] = button->getText();
        emit this->categoryRenamed(newId);
    });
}

/**
 * Чисто визуальное добавление кнопки.
 */
void Menu::addCategory(MenuItem *category)
{
    ui->ltCategories->addWidget(category);
    category->show();
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

MenuItem* Menu::allocCategoryButton()
{
    auto button = new MenuItem;
    button->setIconPath(":/images/folder.png");
    button->setCheckedIconPath(":/images/checkedFolder.png");
    return button;
}

// ==== PRIVATE SLOTS ====
void Menu::buttonClickedSlot()
{
    MenuItem* button = static_cast<MenuItem*>(QObject::sender());
    // Qt кнопки сами выключают check'нутость даже, если они уже check'нутые
    // Нам это не нужно, так что включим check'нутость обратно (костыль)
    if (button == checkedButton) {
        qCInfo(loggingCategory) << "Checking button on button clicked slot" << button->getMenuId();
        button->setChecked(true);
    } else {
        checkButton(button);
    }
}

void Menu::startCategoryAdding()
{
    // Эта кнопка привязывается к слою, и при очистке слоя, автоматически будет
    // вызван delete.
    MenuItem *newButton = allocCategoryButton();
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
    MenuItem* newButton = static_cast<MenuItem*>(QObject::sender());

    // Не вызываем этот метод сразу при добавлении, потому что он может добавить
    // обработчик события завершения редактирования, который отправит запрос на
    // сохранение нового имени. Так как эта кнопка еще только в процессе добавления,
    // нам не нужно ее переименовывать (в теории этой категории еще нет).
    int id = this->findMaxCategoryId() + 1;
    categories.insert(id, newButton->getText());
    setupCategoryButton(newButton, id);

    QObject::disconnect(newButton, SIGNAL(edited()),
                        this, SLOT(categoryEditedAfterAdding()));

    emit categoryAdded(id);
}

void Menu::openCategoryContextMenu(const QPoint &pos)
{
    MenuItem* button = static_cast<MenuItem*>(QObject::sender());
    // There is a bug in Qt - if create context menu and then click outside the button, it stays hovered.
    // So just unhover it manually with a repaint
    EventCallback::setup(contextMenu, QEvent::Hide, [button](QEvent*) {
        button->repaint();
        return false;
    });
    contextMenuButton = button;
    contextMenu->popup(button->mapToGlobal(pos));
}