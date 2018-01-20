#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QLabel>
#include "ScrollBar.h"

#include <QDebug>

// ==== PUBLIC ====
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    currentTable = NULL;

    ui->setupUi(this);
    ui->block->hide();
    ui->scrollArea->hide();

    setupStartConnectings();
}
MainWindow::~MainWindow()
{
    delete ui;
}

// ==== PUBLIC SLOTS ====
void MainWindow::newMenuButtonChecked(MenuButton *)
{
    openBlock();
    //loadTable();
}
void MainWindow::menuButtonUnchecked(MenuButton *)
{
    closeBlock();
}

void MainWindow::addButtonClicked()
{
    if (!currentTable) setupNewTable();
    addTableRow();
}
void MainWindow::deleteButtonClicked()
{
    if (currentTable && currentTable->getCheckedRow() != -1) {
        currentTable->deleteRow(currentTable->getCheckedRow());
        updateTableSize();
        if (currentTable->getRowCount() == 1) ui->scrollArea->hide();
        updateBlockMaxHeight();
    }
}
void MainWindow::editButtonClicked()
{
    if (currentTable && currentTable->getCheckedRow() != -1) {
        currentTable->startRowEditing(currentTable->getCheckedRow());
    }
}

void MainWindow::tableRowChecked()
{
    currentTable->endRowsEditing();
    ui->editButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
}
void MainWindow::tableRowsUnchecked()
{
    if (currentTable) currentTable->endRowsEditing();
    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
}

// ==== EVENTS ====
void MainWindow::resizeEvent(QResizeEvent *)
{
    updateTableSize();
}

// ==== PRIVATE ====
void MainWindow::setupStartConnectings()
{
    QObject::connect(ui->menu, SIGNAL(newButtonChecked(MenuButton*)), this, SLOT(newMenuButtonChecked(MenuButton*)));
    QObject::connect(ui->menu, SIGNAL(buttonUnchecked(MenuButton*)), this, SLOT(menuButtonUnchecked(MenuButton*)));
    QObject::connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addButtonClicked()));
    QObject::connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonClicked()));
    QObject::connect(ui->editButton, SIGNAL(clicked(bool)), this, SLOT(editButtonClicked()));
}

void MainWindow::openBlock()
{
    ui->block->show();
    updateBlockMaxHeight();
}
void MainWindow::closeBlock()
{
    ui->block->hide();
}
void MainWindow::updateBlockMaxHeight()
{
    int tableHeight = 0;
    if (currentTable && !ui->scrollArea->isHidden()) {
        tableHeight = 18 + currentTable->sizeHint().height();
    }
    int fix = 4; // костыль. без этого размеры почему то возвращаются правильные, а на экране нет
    ui->block->setMaximumHeight(9 + ui->addButton->sizeHint().height() + tableHeight + 9 + fix);
}

void MainWindow::setupNewTable()
{
    removeOldTable();
    currentTable = new Table(ui->block);
    ui->scrollArea->show();
    ui->scrollArea->setWidget(currentTable);
    updateTableSize();
    updateBlockMaxHeight();

    QObject::connect(currentTable, SIGNAL(rowChecked()), this, SLOT(tableRowChecked()));
    QObject::connect(currentTable, SIGNAL(rowsUnchecked()), this, SLOT(tableRowsUnchecked()));
}
void MainWindow::removeOldTable()
{
    if (currentTable) {
        //tableRowsUnchecked();
        currentTable->hide();
        delete currentTable;
        currentTable = NULL;
        updateBlockMaxHeight();
    }
}
void MainWindow::hideTable()
{
    ui->scrollArea->hide();
    updateBlockMaxHeight();
}
void MainWindow::showTable()
{
    ui->scrollArea->show();
    updateTableSize();
    updateBlockMaxHeight();
}
void MainWindow::updateTableSize()
{
    if (currentTable) {
        int width = ui->scrollArea->width();
        int height = currentTable->sizeHint().height();
        currentTable->resize(width, height);
    }
}

void MainWindow::addTableRow()
{
    if (currentTable) {
        if (ui->scrollArea->isHidden()) ui->scrollArea->show();
        QList<QString> list;
        list.append("Название");
        list.append("Статус");
        list.append("Оценка");
        list.append("Комментарий");
        currentTable->addRow(list);
        updateTableSize();
        updateBlockMaxHeight();
    }
}
