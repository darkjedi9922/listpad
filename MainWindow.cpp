#include "MainWindow.h"
#include "ui_MainWindow.h"

// ==== PUBLIC ====
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->content->hide();
    ui->menuScroll->setWidget(ui->menu);

    ui->menu->setFixedWidth(240);
    ui->menuScroll->setFixedWidth(240);

    auto menuScrollBar = ui->menuScroll->getVerticalScrollBar();
    auto menuSliderItem = menuScrollBar->getSlider()->getSliderItem();
    menuScrollBar->setFixedWidth(6);
    menuSliderItem->setBrush(QColor("#979797"));

    QObject::connect(ui->menu, SIGNAL(buttonChecked(MenuButton*)),
                     this, SLOT(menuButtonChecked(MenuButton*)));
    QObject::connect(ui->menu, SIGNAL(buttonUnchecked(MenuButton*)),
                     this, SLOT(menuButtonUnchecked(MenuButton*)));
    QObject::connect(ui->menu, &Menu::categoryReallyAdded, [&] () {
        this->menuResized();

        // При добавлении новой категории, нужно прокрутить вниз на случай, если
        // новая категория окажется ниже видимой области.
        auto vScrollBar = ui->menuScroll->getVerticalScrollBar();
        vScrollBar->setValue(vScrollBar->getMaximum());
    });
    QObject::connect(ui->menu, &Menu::categoryAdded, [=] (int id) {
        this->addCategoryToDatabase(id, ui->menu->getCategoryName(id));
    });
    QObject::connect(ui->menu, &Menu::categoryRenamed, [=] (int id) {
        this->data->renameTable(id, ui->menu->getCategoryName(id));
        this->data->saveTables();
    });
    QObject::connect(ui->menu, &Menu::categoryDeleted, [=] (int id) {
        this->menuResized();

        if (ui->content->getCurrentTableId() == id) {
            ui->content->hide();
            ui->content->loadTable(-1);
        }
        data->removeTable(id);
        data->saveTables();
    });
}
MainWindow::~MainWindow()
{
    ui->content->resetTableState();
    ui->content->saveTable();
    saveSettings();

    delete ui;
}

void MainWindow::setData(Core::Data *data)
{
    this->data = data;
    if (!data) return;
    ui->content->setData(data);

    QMap<int, QString> categories;
    foreach (Core::Table *table, data->getTables()) {
        categories.insert(table->getId(), table->getName());
    }
    ui->menu->setCategories(categories);
}

void MainWindow::setSettings(QSettings *settings)
{
    this->settings = settings;
}

Core::Data* MainWindow::getData() const
{
    return data;
}

QSettings* MainWindow::getSettings() const
{
    return settings;
}

// ==== PUBLIC SLOTS ====
void MainWindow::menuButtonChecked(MenuButton *menu)
{
    if (ui->content->getCurrentTableId() != menu->getMenuId()) {
        ui->content->saveTable();
        ui->content->emptyTable();
        ui->content->loadTable(menu->getMenuId());
    }
    ui->content->show();
}
void MainWindow::menuButtonUnchecked(MenuButton *)
{
    ui->content->resetTableState();
    ui->content->saveTable();
    ui->content->hide();
}

// ==== PROTECTED ====
void MainWindow::mousePressEvent(QMouseEvent *)
{
    if (!ui->content->isHidden()) ui->content->resetTableState();
}
void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    // Узнаем находится ли нажатая позиция только в menuScroll, при этом не в menu
    auto menuScrollPos = ui->menuScroll->mapTo(this, e->pos());
    auto menuPos = ui->menu->mapTo(this, e->pos());
    bool menuScrollContains = ui->menuScroll->rect().contains(menuScrollPos);
    bool menuContains = ui->menu->rect().contains(menuPos);
    bool onlyMenuScrollClick = menuScrollContains && !menuContains;

    if (onlyMenuScrollClick) ui->menu->uncheckButton();
}
void MainWindow::showEvent(QShowEvent *)
{
    // Костыльное исправление размера виджета внутри скроллбара.
    ui->menu->resize(ui->menu->sizeHint());
}

// ==== PRIVATE ====
void MainWindow::addCategoryToDatabase(int id, const QString &name)
{
    Core::Table category(id, name, QString("./data/table%1.xml").arg(id));
    data->addTable(category);
    data->saveTables();
}

void MainWindow::saveSettings()
{
    if (!settings) return;
    settings->setValue("maximized", isMaximized());
    settings->setValue("geometry", geometry());
}

void MainWindow::menuResized()
{
    // Костыльное исправление размера виджета внутри скроллбара.
    ui->menu->resize(ui->menu->sizeHint());

    // Настройки скроллбара не подстраиваются под новый размер меню автоматически.
    ui->menuScroll->update();
}
