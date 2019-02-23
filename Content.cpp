#include "Content.h"
#include "ui_Content.h"
#include <QFile>
#include <QDir>
#include <QXmlStreamReader>
#include "core/TableRows.h"

// ==== PUBLIC ====
Content::Content(QWidget *parent) :
    Block(parent),
    ui(new Ui::Content),
    table(new Table),
    tableId(-1)
{
    ui->setupUi(this);
    ui->scrollArea->setWidget(table);
    ui->scrollArea->hide();
    ui->scrollArea->getVerticalScrollBar()->setFixedWidth(12);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QObject::connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addButtonClicked()));
    QObject::connect(ui->deleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteButtonClicked()));
    QObject::connect(ui->editButton, SIGNAL(clicked(bool)), this, SLOT(editButtonClicked()));

    QObject::connect(table, SIGNAL(rowChecked(int)), this, SLOT(tableRowChecked(int)));
    QObject::connect(table, SIGNAL(rowsUnchecked()), this, SLOT(tableRowsUnchecked()));
    QObject::connect(table, SIGNAL(rowDeleted(int)), this, SLOT(tableRowDeleted()));
    QObject::connect(table, SIGNAL(editingFinished(int)), this, SLOT(tableRowEdited(int)));
}
Content::~Content()
{
    delete table;
    delete ui;

    table = nullptr;
    ui = nullptr;
}
QSize Content::sizeHint() const
{
    int tableHeight = 0;
    if (!ui->scrollArea->isHidden()) tableHeight = 18 + table->sizeHint().height();
    int fix = 4; // костыль. без этого размеры почему то возвращаются правильные, а на экране нет
    int height = 9 + ui->addButton->sizeHint().height() + tableHeight + 9 + fix;
    return QSize(100, height);
}

void Content::setTables(const QMap<int, Core::Table*> &tables)
{
    this->tables = tables;
}

void Content::show()
{
    Block::show();
    ui->scrollArea->getVerticalScrollBar()->setValue(0);
    ui->scrollArea->update();
    table->setFocus();
}
void Content::hide()
{
    table->uncheckRows();
    Block::hide();
}
void Content::loadTable(int id)
{
    tableId = id;
    Core::Table *table = tables.value(id, nullptr);
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

        for (int i = 1; i < table->getRowRealCount(); i++)
        {
            if (table->hasRealRow(i))
            {
                QList<QString> row = table->getRealRow(i);
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
        table->empty();
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
void Content::tableRowEdited(int row)
{
    if (table->isStringsEmpty(row)) table->deleteRow(row);
    updateTableSize();
}
void Content::resetTableState()
{
    table->uncheckRows();
    table->setFocus();
}

void Content::addButtonClicked()
{
    addTableEmptyRow();
    updateTableScrollingByRow(table->getEditingRow());
    table->startRowEditing(table->getLastAddedRow());
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
void Content::addTableRow(const QList<QString> &list)
{
    if (ui->scrollArea->isHidden()) ui->scrollArea->show();

    if (table->getCheckedRow() == -1) table->appendRow(list);
    else table->insertRowAfter(list, table->getCheckedRow());

    updateTableSize();
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
