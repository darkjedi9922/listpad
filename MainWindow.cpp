#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QLabel>
#include "ScrollBar.h"

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

    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->verticalScrollBar()->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->scrollArea->setVerticalScrollBar(new ScrollBar(Qt::Vertical, ui->scrollArea));
    ui->block->hide();

    QObject::connect(ui->menu->ui->films, SIGNAL(clicked(bool)), this, SLOT(menuButtonClicked()));
    QObject::connect(ui->menu->ui->serials, SIGNAL(clicked(bool)), this, SLOT(menuButtonClicked()));
    QObject::connect(ui->menu->ui->books, SIGNAL(clicked(bool)), this, SLOT(menuButtonClicked()));
    QObject::connect(ui->menu, SIGNAL(clicked()), this, SLOT(menuBackgroundClicked()));
    QObject::connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addButtonClicked()));
    QObject::connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonClicked()));
    QObject::connect(ui->editButton, SIGNAL(clicked(bool)), this, SLOT(editButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateBlockMaxHeight()
{
    int tableHeight = 0;
    if (currentTable && !isTableHidden()) tableHeight = 18 + currentTable->sizeHint().height();
    int fix = 4; // костыль. без этого размеры почему то возвращаются правильные, а на экране нет
    ui->block->setMaximumHeight(9 + ui->addButton->sizeHint().height() + tableHeight + 9 + fix);
}

void MainWindow::setupNewTable()
{
    unsetupTable();
    currentTable = new Table(ui->block);
    ui->scrollArea->setWidget(currentTable);
    hideTable();

    QObject::connect(currentTable, SIGNAL(rowChecked()), this, SLOT(tableRowChecked()));
    QObject::connect(currentTable, SIGNAL(rowsUnchecked()), this, SLOT(tableRowsUnchecked()));
}
void MainWindow::unsetupTable()
{
    if (currentTable) {
        tableRowsUnchecked();
        hideTable();
        delete currentTable;
        currentTable = NULL;
    }
}

void MainWindow::updateScrollBar()
{
    ScrollBar *bar = static_cast<ScrollBar*>(ui->scrollArea->verticalScrollBar());
    bar->setDocumentLength(currentTable ? currentTable->height() : 0);
}

void MainWindow::hideTable()
{
    ui->scrollArea->hide();
    updateBlockMaxHeight();
}
void MainWindow::showTable()
{
    ui->scrollArea->show();
    updateBlockMaxHeight();
}
bool MainWindow::isTableHidden() const
{
    return ui->scrollArea->isHidden();
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
        updateBlockMaxHeight();
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

void MainWindow::addButtonClicked()
{
    if (isTableHidden()) showTable();
    QList<QString> list;
    list.append("Название");
    list.append("Статус");
    list.append("Оценка");
    list.append("Комментарий");
    currentTable->addRow(list);
    updateBlockMaxHeight();
    updateScrollBar();
}

void MainWindow::deleteButtonClicked()
{
    if (currentTable && currentTable->getCheckedRow() != -1) {
        currentTable->deleteRow(currentTable->getCheckedRow());
        if (currentTable->getRowCount() == 1) hideTable();
        else {
            updateBlockMaxHeight();
            updateScrollBar();
        }
    }
}

void MainWindow::editButtonClicked()
{
    if (currentTable && currentTable->getCheckedRow() != -1) {
        currentTable->startRowEditing(currentTable->getCheckedRow());
    }
}
