#include "Content.h"
#include "ui_Content.h"
#include <QFile>
#include <QDir>
#include <QXmlStreamReader>
#include <QTimer>
#include "core/TableRows.h"
#include "widgets/SimpleScrollBar.h"

// ==== PUBLIC ====
Content::Content(QWidget *parent) :
    Block(parent),
    ui(new Ui::Content),
    data(nullptr),
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

    QObject::connect(ui->table, SIGNAL(rowChecked(int)), this, SLOT(tableRowChecked(int)));
    QObject::connect(ui->table, SIGNAL(rowsUnchecked()), this, SLOT(tableRowsUnchecked()));
    QObject::connect(ui->table, SIGNAL(rowDeleted(int)), this, SLOT(tableRowDeleted()));
    QObject::connect(ui->table, SIGNAL(editingFinished(int)), this, SLOT(tableRowEdited(int)));

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

void Content::setData(Core::Data *data)
{
    this->data = data;
}

void Content::show()
{
    Block::show();
    ui->scrollArea->verticalScrollBar()->setValue(0);
    ui->scrollArea->update();
    ui->table->setFocus();
}
void Content::hide()
{
    ui->table->uncheckRows();
    Block::hide();
    eSearch->reset();
}
void Content::loadTable(int id)
{
    emptyTable();
    tableId = id;
    Core::Table *table = data->getTables().value(id, nullptr);
    if (!table) return;
    Core::TableRows rows(table->getFilename());
    for (const QList<QString> &row : rows.getRows()) {
        addTableRow(row);
    }
}
void Content::saveTable()
{
    if (tableId != -1)
    {
        createDataDirectoryIfItDoesNotExist();

        QString filename = QString("data/table%1.xml").arg(tableId);
        QFile file(filename);
        file.open(QIODevice::WriteOnly);

        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("table");

        for (int i = 1; i < ui->table->getRowCount(); i++)
        {
            if (ui->table->hasRow(i))
            {
                QList<QString> row = ui->table->getRow(i);
                writer.writeStartElement("row");
                writer.writeAttribute("title", row.at(0));
                writer.writeAttribute("status", row.at(1));
                writer.writeAttribute("rating", row.at(2));
                writer.writeAttribute("comment", row.at(3));
                writer.writeEndElement();
            }
        }

        writer.writeEndElement();
        writer.writeEndDocument();
        file.close();
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
void Content::tableRowChecked(int row)
{
    qCDebug(loggingCategory) << "Checking table row" << row;
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
    if (ui->table->getRowCount() == 1) ui->scrollArea->hide();
    else ui->table->setFocus();
}
void Content::tableRowEdited(int row)
{
    if (ui->table->isStringsEmpty(row)) ui->table->deleteRow(row);
}
void Content::resetTableState()
{
    ui->table->uncheckRows();
    ui->table->setFocus();
}

void Content::addButtonClicked()
{
    addTableEmptyRow();
    ui->table->startRowEditing(ui->table->getLastAddedRow());
    qCDebug(loggingCategory) << "Started row editing";
    
    QTimer::singleShot(100, [&]() {
        updateTableScrollingByRow(ui->table->getEditingRow());
    });
}
void Content::deleteButtonClicked()
{
    ui->table->deleteRow(ui->table->getCheckedRow());
}
void Content::editButtonClicked()
{
    ui->table->startRowEditing(ui->table->getCheckedRow());
}

// ==== PRIVATE ====
void Content::addTableRow(const QList<QString> &list)
{
    qCDebug(loggingCategory) << "Adding table row of" << list.size() << "elements";
    if (ui->scrollArea->isHidden()) ui->scrollArea->show();

    if (ui->table->getCheckedRow() == -1) ui->table->appendRow(list);
    else ui->table->insertRowAfter(list, ui->table->getCheckedRow());
}
void Content::addTableEmptyRow()
{
    QList<QString> list;
    list << "" << "" << "" << "";
    addTableRow(list);
}

void Content::createDataDirectoryIfItDoesNotExist()
{
    QDir appDirectory("./");
    if (!appDirectory.exists("data")) {
        appDirectory.mkdir("data");
    }
}

void Content::updateTableScrollingByRow(int row)
{
    QRect rowRect = ui->table->getRowRect(row);
    int scrollValue = ui->scrollArea->verticalScrollBar()->value();
    qCDebug(loggingCategory) << "updateTableScrollingByRow" << row << "rowRect =" << rowRect;
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
