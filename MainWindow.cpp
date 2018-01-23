#include "MainWindow.h"
#include "ui_MainWindow.h"

// ==== PUBLIC ====
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    settings(new QSettings("settings.ini", QSettings::IniFormat))
{
    ui->setupUi(this);
    ui->content->hide();

    QObject::connect(ui->menu, SIGNAL(buttonChecked(MenuButton*)), this, SLOT(menuButtonChecked(MenuButton*)));
    QObject::connect(ui->menu, SIGNAL(buttonUnchecked(MenuButton*)), this, SLOT(menuButtonUnchecked(MenuButton*)));
}
MainWindow::~MainWindow()
{
    ui->content->saveTable();
    saveSettings();

    delete ui;
    delete settings;
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
    ui->content->saveTable();
    ui->content->hide();
}

// ==== PROTECTED ====
void MainWindow::mousePressEvent(QMouseEvent *)
{
    if (!ui->content->isHidden()) ui->content->resetTableState();
}

// ==== PRIVATE ====
void MainWindow::saveSettings()
{
    settings->setValue("maximized", isMaximized());
    settings->setValue("geometry", geometry());
}
