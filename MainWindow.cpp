#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "widgets/SimpleScrollBar.h"

// ==== PUBLIC ====
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    loggingCategory("MainWindow")
{
    ui->setupUi(this);
    ui->content->hide();
    
    ui->menuScroll->setVerticalScrollBar(new SimpleScrollBar(Qt::Vertical));

    auto menuScrollBar = ui->menuScroll->verticalScrollBar();

    QObject::connect(ui->menu, SIGNAL(buttonChecked(MenuItem*)), this, SLOT(menuButtonChecked(MenuItem*)));
    QObject::connect(ui->menu, SIGNAL(buttonUnchecked(MenuItem*)), this, SLOT(menuButtonUnchecked(MenuItem*)));
    QObject::connect(ui->menu, &Menu::categoryReallyAdded, [&] () {
        // При добавлении новой категории, нужно прокрутить вниз на случай, если
        // новая категория окажется ниже видимой области.
        auto vScrollBar = ui->menuScroll->verticalScrollBar();
        vScrollBar->setValue(vScrollBar->maximum());
    });
    QObject::connect(ui->menu, &Menu::categoryAdded, [=] (int id) {
        this->addCategoryToDatabase(id, ui->menu->getCategoryName(id));
    });
    QObject::connect(ui->menu, &Menu::categoryRenamed, [=] (int id) {
        this->db->renameTable(id, ui->menu->getCategoryName(id));
    });
    QObject::connect(ui->menu, &Menu::categoryDeleted, [=] (int id) {
        if (ui->content->getCurrentTableId() == id) {
            ui->content->hide();
            ui->content->loadTable(-1);
        }
        db->removeTable(id);
    });
}
MainWindow::~MainWindow()
{
    ui->content->resetTableState();
    saveSettings();

    delete ui;
}

void MainWindow::setDatabase(Core::SqlData *db)
{
    this->db = db;
    if (!db) return;
    ui->content->setDatabase(db);

    QMap<int, QString> categories;
    QSqlQuery query = db->getCategories();
    while (query.next()) {
        categories.insert(query.value("id").toInt(), query.value("name").toString());
    }
    ui->menu->setCategories(categories);
}

void MainWindow::setSettings(QSettings *settings)
{
    this->settings = settings;
}
QSettings* MainWindow::getSettings() const
{
    return settings;
}

// ==== PUBLIC SLOTS ====
void MainWindow::menuButtonChecked(MenuItem *menu)
{
    if (ui->content->getCurrentTableId() != menu->getMenuId()) {
        ui->content->emptyTable();
        ui->content->loadTable(menu->getMenuId());
    }
    ui->content->show();
}
void MainWindow::menuButtonUnchecked(MenuItem *)
{
    ui->content->resetTableState();
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

    qCDebug(loggingCategory) << "Mouse release event" << menuScrollContains << menuContains;
    if (onlyMenuScrollClick) ui->menu->uncheckButton();
}

// ==== PRIVATE ====
void MainWindow::addCategoryToDatabase(int id, const QString &name)
{
    db->addTable(id, name);
}

void MainWindow::saveSettings()
{
    if (!settings) return;
    settings->setValue("maximized", isMaximized());
    settings->setValue("geometry", geometry());
}
