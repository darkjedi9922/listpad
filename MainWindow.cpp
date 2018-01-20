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
    ui->scrollArea->getVerticalScrollBar()->setFixedWidth(10);

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
    addTableRow();
}
void MainWindow::deleteButtonClicked()
{
    if (currentTable && currentTable->getCheckedRow() != -1) {
        currentTable->deleteRow(currentTable->getCheckedRow());
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
void MainWindow::tableRowDeleted()
{
    updateTableSize();
    if (currentTable->getRowCount() == 1) ui->scrollArea->hide();
    updateBlockMaxHeight();
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
    float fix = 1.5; // table size hint width в данный момент кода почему то возвращает не то значение
    ui->scrollArea->setMinimumWidth(currentTable->sizeHint().width() * fix);
    updateTableSize();

    QObject::connect(currentTable, SIGNAL(rowChecked()), this, SLOT(tableRowChecked()));
    QObject::connect(currentTable, SIGNAL(rowsUnchecked()), this, SLOT(tableRowsUnchecked()));
    QObject::connect(currentTable, SIGNAL(rowDeleted(int)), this, SLOT(tableRowDeleted()));
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
void MainWindow::updateTableSize()
{
    if (currentTable) {
        int vScrollBarWidth = ui->scrollArea->getVerticalScrollBar()->width();
        if (ui->scrollArea->getVerticalScrollBar()->isHidden()) vScrollBarWidth = 0;
        int width = ui->scrollArea->width() - vScrollBarWidth;
        int height = currentTable->sizeHint().height();
        currentTable->resize(width, height);

        ui->scrollArea->update();
        updateBlockMaxHeight();
    }
}

void MainWindow::addTableRow()
{
    if (ui->scrollArea->isHidden()) ui->scrollArea->show();
    if (!currentTable) setupNewTable();

    QList<QString> list;
    list.append("Название");
    list.append("Статус");
    list.append("Оценка");
    list.append("Комментарий");
    currentTable->addRow(list);

    updateTableSize();
    int sliderMaximum = ui->scrollArea->getVerticalScrollBar()->getSlider()->getMaximum();
    ui->scrollArea->getVerticalScrollBar()->getSlider()->setValue(sliderMaximum);
}
