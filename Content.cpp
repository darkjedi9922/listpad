#include "Content.h"
#include "ui_Content.h"
#include <QFile>
#include <QDir>
#include <QTimer>
#include <QStyleOption>
#include <QPainter>
#include "core/TableRows.h"
#include "widgets/SimpleScrollBar.h"
#include "widgets/elements/LineEdit.h"

// ==== PUBLIC ====
Content::Content(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Content),
    db(nullptr),
    eSearch(nullptr),
    tableId(-1),
    loggingCategory("Content")
{
    ui->setupUi(this);
    ui->scrollArea->setVerticalScrollBar(new SimpleScrollBar(Qt::Vertical));
    ui->scrollArea->setHorizontalScrollBar(new SimpleScrollBar(Qt::Horizontal));
    ui->scrollArea->hide();

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QObject::connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addButtonClicked()));
    QObject::connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonClicked()));
    QObject::connect(ui->editButton, SIGNAL(clicked(bool)), this, SLOT(editButtonClicked()));

    QObject::connect(ui->table, &Table::rowIdChecked, this, &Content::tableRowChecked);
    QObject::connect(ui->table, SIGNAL(rowsUnchecked()), this, SLOT(tableRowsUnchecked()));
    QObject::connect(ui->table, SIGNAL(rowDeleted(int)), this, SLOT(tableRowDeleted()));
    QObject::connect(ui->table, SIGNAL(editingStarted(int)), this, SLOT(tableRowEditingStarted(int)));
    QObject::connect(ui->table, &CollectionTable::editingFinishedById, this, &Content::tableRowEditingFinished);
    QObject::connect(ui->table, &CollectionTable::rowTextEdited, this, &Content::tableRowTextEdited);
    QObject::connect(ui->table, &CollectionTable::rowStarToggled, this, &Content::tableRowStarToggled);
    QObject::connect(ui->table, &CollectionTable::cellCursorPositionChanged, [&](LineEdit *cell) {
        QTimer::singleShot(25, [cell, this]() {
            adjustHorizontalScrollForCellCursor(cell);
        });
    });

    eSearch = new SearchEngine(ui->searchLine->getSearchLine(), ui->table);
}
Content::~Content()
{
    delete eSearch;
    eSearch = nullptr;

    // Таблица удалится сама, так как она является дочерним виджетом scrollArea,
    // а в Qt объекты сами удаляют своих детей, которые установили своих родителей
    // в конструкторе или как setParent(), что и происходит в реализации scrollArea.
    // Если пытаться ее удалять тут, возникает ошибка записи памяти в куче.
    // delete table;
    // table = nullptr;

    delete ui;
    ui = nullptr;
}

void Content::setDatabase(Core::SqlData *db)
{
    this->db = db;
}

void Content::show()
{
    QWidget::show();
    ui->scrollArea->verticalScrollBar()->setValue(0);
    ui->scrollArea->update();
    ui->table->setFocus();
}
void Content::hide()
{
    ui->table->uncheckRows();
    QWidget::hide();
    eSearch->reset();
}
void Content::loadTable(int id)
{
    emptyTable();
    tableId = id;
    QSqlQuery query = db->getTableRows(id);
    while (query.next()) {
        addTableRow(query.value("id").toULongLong(), {
            query.value("starred").toBool(),
            query.value("title").toString(),
            query.value("status").toString(),
            query.value("rating").toString(),
            query.value("comment").toString()
        });
    }
}
void Content::emptyTable()
{
    if (tableId != -1) {
        tableId = -1;
        ui->table->empty();
        ui->scrollArea->hide();
    }
}

int Content::getCurrentTableId() const
{
    return tableId;
}

// ==== PUBLIC SLOTS ====
void Content::tableRowChecked(Table::row_id rowId)
{
    qCDebug(loggingCategory) << "Checking table row id" << rowId;
    updateTableScrollingByRow(rowId);

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
    if (ui->table->getRowCount() == 1) ui->scrollArea->hide();
    else ui->table->setFocus();
}
void Content::tableRowEditingStarted(int row)
{
    ui->editButton->setEnabled(false);
}
void Content::tableRowEditingFinished(Table::row_id rowId)
{
    ui->addButton->setEnabled(true);
    ui->editButton->setEnabled(true);
    if (ui->table->isStringsEmpty(rowId)) {
        qCInfo(loggingCategory) << "Deleting row id" << rowId;
        db->removeTableRow(rowId);
        ui->table->deleteRow(rowId);
    } else {
        qCInfo(loggingCategory) << "Updating row id" << rowId;
        db->updateTableRow(rowId, ui->table->getRow(rowId));
    }
}
void Content::tableRowTextEdited(Table::row_id rowId)
{
    ui->addButton->setEnabled(!ui->table->isStringsEmpty(rowId));
}
void Content::tableRowStarToggled(Table::row_id rowId, bool starred)
{
    db->updateTableRowStarred(rowId, starred);
}
void Content::resetTableState()
{
    ui->table->uncheckRows();
    ui->table->setFocus();
}

void Content::addButtonClicked()
{
    auto rowId = db->addTableRow(tableId, {false, "", "", "", ""});
    addTableEmptyRow(rowId);
    ui->addButton->setEnabled(false);
    ui->table->startRowEditing(rowId);
    qCDebug(loggingCategory) << "Started row editing";
    
    QTimer::singleShot(100, [rowId, this]() {
        updateTableScrollingByRow(rowId);
    });
}
void Content::deleteButtonClicked()
{
    qCInfo(loggingCategory) << "Deleting row id" << ui->table->getCheckedRowId();
    db->removeTableRow(ui->table->getCheckedRowId());
    ui->table->deleteRow(ui->table->getCheckedRowId());
}
void Content::editButtonClicked()
{
    ui->table->startRowEditing(ui->table->getCheckedRowId());
}

void Content::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

// ==== PRIVATE ====
void Content::addTableRow(Table::row_id id, const Core::TableRow &data)
{
    qCDebug(loggingCategory) << "Adding table row with starred =" << data.starred;
    if (ui->scrollArea->isHidden()) ui->scrollArea->show();

    if (ui->table->getCheckedRow() == -1) {
        ui->table->appendRow(id, data);
    } else {
        qCDebug(loggingCategory) << "Inserting a row after checked row" << ui->table->getCheckedRow();
        ui->table->insertRowAfter(id, data, ui->table->getCheckedRow());
    }
}
void Content::addTableEmptyRow(Table::row_id id)
{
    addTableRow(id, {});
}

void Content::createDataDirectoryIfItDoesNotExist()
{
    QDir appDirectory("./");
    if (!appDirectory.exists("data")) {
        appDirectory.mkdir("data");
    }
}

void Content::updateTableScrollingByRow(Table::row_id rowId)
{
    QRect rowRect = ui->table->getRowRect(rowId);
    int scrollValue = ui->scrollArea->verticalScrollBar()->value();
    qCDebug(loggingCategory) << "updateTableScrollingByRow id" << rowId << "rowRect =" << rowRect;
    int rowGlobalTop = rowRect.top() - scrollValue;
    if (rowGlobalTop < 0) {
        qCDebug(loggingCategory) << "updateTableScrollingByRow.scroll (1)" << scrollValue + rowGlobalTop; 
        ui->scrollArea->verticalScrollBar()->setValue(scrollValue + rowGlobalTop);
    } else {
        int rowGlobalBottom = rowRect.bottom() - scrollValue;
        int viewportHeight = ui->scrollArea->viewport()->height();
        if (rowGlobalBottom > viewportHeight) {
            ui->scrollArea->verticalScrollBar()->setValue(scrollValue + rowGlobalBottom - viewportHeight + 1);
        }
    }
}
void Content::adjustHorizontalScrollForCellCursor(LineEdit *cell)
{
    auto viewport = ui->scrollArea->viewport();
    auto margins = ui->table->layout()->contentsMargins();
    auto viewportRight = viewport->mapToGlobal(viewport->rect().topRight()).x() - margins.right();
    auto viewportLeft = viewport->mapToGlobal(viewport->rect().topLeft()).x() + margins.left();
    auto cursorPos = cell->mapToGlobal(cell->cursorRect().topRight()).x();
    auto horizontalScrollBar = ui->scrollArea->horizontalScrollBar();
    if (cursorPos > viewportRight) {
        horizontalScrollBar->setValue(horizontalScrollBar->value() + cursorPos - viewportRight);
    } else if (cursorPos < viewportLeft) {
        horizontalScrollBar->setValue(horizontalScrollBar->value() - (viewportLeft - cursorPos));
    }
}
