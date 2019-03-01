#include "MainWindow.h"
#include "ui_MainWindow.h"

// ==== PUBLIC ====
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->content->hide();

    QObject::connect(ui->menu, SIGNAL(buttonChecked(MenuButton*)),
                     this, SLOT(menuButtonChecked(MenuButton*)));
    QObject::connect(ui->menu, SIGNAL(buttonUnchecked(MenuButton*)),
                     this, SLOT(menuButtonUnchecked(MenuButton*)));
    QObject::connect(ui->menu, &Menu::categoryAdded, [=] (int id) {
        this->addCategoryToDatabase(id, ui->menu->getCategoryName(id));
    });
    QObject::connect(ui->menu, &Menu::categoryRenamed, [=] (int id) {
        this->data->renameTable(id, ui->menu->getCategoryName(id));
        this->data->saveTables();
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
    ui->content->setTables(data->getTables());

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