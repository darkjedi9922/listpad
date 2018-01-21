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
void MainWindow::resetTableFocus()
{
    if (currentTable) {
        currentTable->uncheckRows();
        currentTable->setFocus();
    }
}

void MainWindow::newMenuButtonChecked(MenuButton *)
{
    ui->block->show();
    setupTable();
    updateBlockMaxHeight();
    currentTable->setFocus();
}
void MainWindow::menuButtonUnchecked(MenuButton *)
{
    if (currentTable) currentTable->uncheckRows();
    removeTable();
    ui->scrollArea->hide();
    ui->block->hide();
}

void MainWindow::addButtonClicked()
{
    if (ui->scrollArea->isHidden()) ui->scrollArea->show();
    addTableRow();
    updateTableSize();
    updateTableScrollingByRow(currentTable->getEditingRow());
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

void MainWindow::tableRowChecked(int row)
{
    updateTableScrollingByRow(row);

    ui->editButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
}
void MainWindow::tableRowsUnchecked()
{
    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
}
void MainWindow::tableRowDeleted()
{
    updateTableSize();
    if (currentTable->getRowCount() == 1) ui->scrollArea->hide();
    currentTable->setFocus();
}
void MainWindow::tableRowEdited()
{
    updateScrollAreaMinWidth();
}

// ==== EVENTS ====
void MainWindow::resizeEvent(QResizeEvent *)
{
    updateTableSize();
}

// ==== PRIVATE ====
void MainWindow::setupStartConnectings()
{
    QObject::connect(ui->logo, SIGNAL(mousePressed()), this, SLOT(resetTableFocus()));
    QObject::connect(ui->block, SIGNAL(mousePressed()), this, SLOT(resetTableFocus()));

    QObject::connect(ui->menu, SIGNAL(newButtonChecked(MenuButton*)), this, SLOT(newMenuButtonChecked(MenuButton*)));
    QObject::connect(ui->menu, SIGNAL(buttonUnchecked(MenuButton*)), this, SLOT(menuButtonUnchecked(MenuButton*)));

    QObject::connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addButtonClicked()));
    QObject::connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonClicked()));
    QObject::connect(ui->editButton, SIGNAL(clicked(bool)), this, SLOT(editButtonClicked()));
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
void MainWindow::updateScrollAreaMinWidth()
{
    if (currentTable) ui->scrollArea->setMinimumWidth(currentTable->sizeHint().width());
}

void MainWindow::setupTable()
{
    if (!currentTable) {
        currentTable = new Table(ui->block);
        ui->scrollArea->setWidget(currentTable);

        QObject::connect(currentTable, SIGNAL(rowChecked(int)), this, SLOT(tableRowChecked(int)));
        QObject::connect(currentTable, SIGNAL(rowsUnchecked()), this, SLOT(tableRowsUnchecked()));
        QObject::connect(currentTable, SIGNAL(rowDeleted(int)), this, SLOT(tableRowDeleted()));
        QObject::connect(currentTable, SIGNAL(editingFinished()), this, SLOT(tableRowEdited()));
    }
}
void MainWindow::removeTable()
{
    if (currentTable) {
        currentTable->hide();
        delete currentTable;
        currentTable = NULL;
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
    if (currentTable) {
        QList<QString> list;
        list << "" << "" << "" << "";
        if (currentTable->getCheckedRow() == -1) currentTable->appendRow(list);
        else currentTable->insertRowAfter(list, currentTable->getCheckedRow());
    }
}
void MainWindow::updateTableScrollingByRow(int row)
{
    QRect rowRect = currentTable->getRowRect(row);
    int scrollValue = ui->scrollArea->getVerticalScrollBar()->getValue();
    int rowGlobalTop = rowRect.top() - scrollValue;
    if (rowGlobalTop < 0) ui->scrollArea->getVerticalScrollBar()->setValue(scrollValue + rowGlobalTop);
    else {
        int rowGlobalBottom = rowRect.bottom() - scrollValue;
        int scrollAreaHeight = ui->scrollArea->height();
        if (rowGlobalBottom > scrollAreaHeight) {
            ui->scrollArea->getVerticalScrollBar()->setValue(scrollValue + rowGlobalBottom - scrollAreaHeight + 1);
        }
    }
}
