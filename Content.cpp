#include "Content.h"
#include "ui_Content.h"

// ==== PUBLIC ====
Content::Content(QWidget *parent) :
    Block(parent),
    ui(new Ui::Content),
    table(new Table)
{
    ui->setupUi(this);
    ui->scrollArea->setWidget(table);
    ui->scrollArea->hide();
    ui->scrollArea->getVerticalScrollBar()->setFixedWidth(10);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QObject::connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addButtonClicked()));
    QObject::connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonClicked()));
    QObject::connect(ui->editButton, SIGNAL(clicked(bool)), this, SLOT(editButtonClicked()));

    QObject::connect(table, SIGNAL(rowChecked(int)), this, SLOT(tableRowChecked(int)));
    QObject::connect(table, SIGNAL(rowsUnchecked()), this, SLOT(tableRowsUnchecked()));
    QObject::connect(table, SIGNAL(rowDeleted(int)), this, SLOT(tableRowDeleted()));
    QObject::connect(table, SIGNAL(editingFinished()), this, SLOT(tableRowEdited()));
}
Content::~Content()
{
    delete ui;
}
QSize Content::sizeHint() const
{
    int tableHeight = 0;
    if (!ui->scrollArea->isHidden()) tableHeight = 18 + table->sizeHint().height();
    int fix = 4; // костыль. без этого размеры почему то возвращаются правильные, а на экране нет
    int height = 9 + ui->addButton->sizeHint().height() + tableHeight + 9 + fix;
    return QSize(100, height);
}

void Content::show()
{
    Block::show();
    //table->setFocus();
}
void Content::hide()
{
    table->uncheckRows();
    ui->scrollArea->hide();
    Block::hide();
}

// ==== PUBLIC SLOTS ====
void Content::tableRowChecked(int row)
{
    updateTableScrollingByRow(row);

    ui->editButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
}
void Content::tableRowsUnchecked()
{
    ui->editButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
}
void Content::tableRowDeleted()
{
    updateTableSize();
    if (table->getRowCount() == 1) ui->scrollArea->hide();
    else table->setFocus();
}
void Content::tableRowEdited()
{
    updateTableSize();
}
void Content::resetTableState()
{
    table->uncheckRows();
    table->setFocus();
}

void Content::addButtonClicked()
{
    if (ui->scrollArea->isHidden()) ui->scrollArea->show();
    addTableRow();
    updateTableSize();
    updateTableScrollingByRow(table->getEditingRow());
}
void Content::deleteButtonClicked()
{
    table->deleteRow(table->getCheckedRow());
}
void Content::editButtonClicked()
{
    table->startRowEditing(table->getCheckedRow());
}

// ==== EVENTS ====
void Content::resizeEvent(QResizeEvent *)
{
    updateTableSize();
}

// ==== PRIVATE ====
void Content::addTableRow()
{
    QList<QString> list;
    list << "" << "" << "" << "";
    if (table->getCheckedRow() == -1) table->appendRow(list);
    else table->insertRowAfter(list, table->getCheckedRow());
}
void Content::updateTableSize()
{
    if (!ui->scrollArea->isHidden())
    {
        int vScrollBarWidth = ui->scrollArea->getVerticalScrollBar()->width();
        if (ui->scrollArea->getVerticalScrollBar()->isHidden()) vScrollBarWidth = 0;
        int width = ui->scrollArea->width() - vScrollBarWidth;
        int height = table->sizeHint().height();
        table->resize(width, height);

        ui->scrollArea->setMaximumHeight(table->sizeHint().height());
        ui->scrollArea->setMinimumWidth(table->sizeHint().width());
        ui->scrollArea->update();
    }
}
void Content::updateTableScrollingByRow(int row)
{
    QRect rowRect = table->getRowRect(row);
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
