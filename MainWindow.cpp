#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QLabel>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    checkedMenuButton = NULL;
    currentTable = NULL;

    ui->menu->setFixedWidth(240);

    QPalette shadowPalette;
    QPixmap shadowPixmap("images/shadow.png");
    shadowPalette.setBrush(QPalette::Background, QBrush(shadowPixmap));
    ui->shadow->setPalette(shadowPalette);
    ui->shadow->setFixedWidth(shadowPixmap.width());

    ui->block->hide();

    QObject::connect(ui->menu->ui->films, SIGNAL(clicked(bool)), this, SLOT(menuButtonClicked()));
    QObject::connect(ui->menu->ui->serials, SIGNAL(clicked(bool)), this, SLOT(menuButtonClicked()));
    QObject::connect(ui->menu->ui->books, SIGNAL(clicked(bool)), this, SLOT(menuButtonClicked()));
    QObject::connect(ui->menu, SIGNAL(clicked()), this, SLOT(menuBackgroundClicked()));
    QObject::connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupNewTable()
{
    unsetupTable();
    currentTable = new Table(ui->block);
    ui->block->layout()->addWidget(currentTable);
    currentTable->hide();

    QObject::connect(currentTable, SIGNAL(rowChecked()), this, SLOT(tableRowChecked()));
    QObject::connect(currentTable, SIGNAL(rowsUnchecked()), this, SLOT(tableRowsUnchecked()));
}
void MainWindow::unsetupTable()
{
    if (currentTable) {
        tableRowsUnchecked();
        currentTable->hide();
        delete currentTable;
        currentTable = NULL;
    }
}

void MainWindow::menuButtonClicked()
{
    MenuButton *button = (MenuButton*)QObject::sender();
    if (button != checkedMenuButton)
    {
        if (checkedMenuButton) checkedMenuButton->setChecked(false);
        button->setChecked(true);
        checkedMenuButton = button;

        ui->block->show();
        setupNewTable();
    }
    // Qt кнопки сами выключают check'нутость даже, если они уже check'нутые
    // Нам это не нужно
    // Так что включим check'нутость обратно
    // (Костыль)
    else checkedMenuButton->setChecked(true);
}

void MainWindow::menuBackgroundClicked()
{
    if (checkedMenuButton) {
        checkedMenuButton->setChecked(false);
        checkedMenuButton = NULL;
        ui->block->hide();
        unsetupTable();
    }
}

void MainWindow::addButtonClicked()
{
    if (currentTable->isHidden()) currentTable->show();
    QLabel *name = new QLabel("Название", currentTable);
    QLabel *status = new QLabel("Статус", currentTable);
    QLabel *rating = new QLabel("Оценка", currentTable);
    QLabel *comment = new QLabel("Комментарий", currentTable);
    currentTable->addRow(name, status, rating, comment);
}

void MainWindow::tableRowChecked()
{
    ui->editButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
}

void MainWindow::tableRowsUnchecked()
{
    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
}
