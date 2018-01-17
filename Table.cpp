#include "Table.h"
#include <QMouseEvent>
#include <QPainter>

#include <QDebug>

Table::Table(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Table),
    checkedRowBrush(QColor("#d5d4d4")),
    checkedRow(-1),
    rowHeight(27),
    rowCount(1),
    editingRow(-1)
{
    ui->setupUi(this);
    updateMinHeight();
}

Table::~Table()
{
    endRowsEditing();
    delete ui;
}

QSize Table::sizeHint() const
{
    return QSize(ui->gridLayout->sizeHint().width(), rowHeight * rowCount);
}

void Table::addRow(const QList<QString> &list)
{
    QLineEdit *newLineEdit;
    QPalette pal(palette());
    pal.setBrush(QPalette::Base, Qt::transparent);
    pal.setColor(QPalette::Text, Qt::black);
    pal.setColor(QPalette::HighlightedText, Qt::white);
    pal.setColor(QPalette::Highlight, "#249dcd");

    int column = 0;
    int rowToInsert = ui->gridLayout->rowCount();
    for (QList<QString>::const_iterator it = list.begin(); it != list.end(); it++) {
        newLineEdit = new QLineEdit(*it, this);
        newLineEdit->setFrame(false);
        newLineEdit->setPalette(pal);
        newLineEdit->setEnabled(false);
        newLineEdit->setMinimumWidth(QFontMetrics(newLineEdit->font()).width(newLineEdit->text()) + 10);
        newLineEdit->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored);
        ui->gridLayout->addWidget(newLineEdit, rowToInsert, column);
        QObject::connect(newLineEdit, SIGNAL(returnPressed()), this, SLOT(endRowsEditing()));
        column += 1;
    }

    startRowEditing(rowToInsert);

    rowCount += 1;
    updateMinHeight();
}

void Table::deleteRow(int row)
{
    if (row == checkedRow) setRowChecked(row, false);

    for (int i = 0, c = ui->gridLayout->columnCount(); i < c; i++) {
        delete ui->gridLayout->itemAtPosition(row, i)->widget();
    }

    rowCount -= 1;
    updateMinHeight();

    emit rowDeleted(row);
}

void Table::setRowChecked(int row, bool checked)
{
    if (checked && row != checkedRow) {
        checkedRow = row;
        repaint();
        emit rowChecked();
    } else if (!checked && row == checkedRow) {
        checkedRow = -1;
        repaint();
        emit rowsUnchecked();
    }
}

void Table::startRowEditing(int row)
{
    endRowsEditing();

    editingRow = row;
    int columns = ui->gridLayout->columnCount();
    // Идем с конца, чтобы первый элемент оказался с курсором
    for (int i = columns - 1; i >= 0; i--) {
        QLineEdit *item = getItemAt(row, i);
        if (item) {
            QPalette pal(item->palette());
            pal.setBrush(QPalette::Base, QColor("#003366"));
            pal.setColor(QPalette::Text, Qt::white);
            item->setPalette(pal);
            item->setEnabled(true);
            item->setFocus();
            item->setCursorPosition(item->text().length());
        }
    }
}
void Table::endRowsEditing()
{
    if (editingRow != -1) {
        int columns = ui->gridLayout->columnCount();
        for (int i = columns - 1; i >= 0; i--) {
            QLineEdit *item = getItemAt(editingRow, i);
            if (item) {
                QPalette pal(item->palette());
                pal.setBrush(QPalette::Base, Qt::transparent);
                pal.setColor(QPalette::Text, Qt::black);
                item->setPalette(pal);
                item->setEnabled(false);
                item->setMinimumWidth(QFontMetrics(item->font()).width(item->text()) + 10);
            }
        }
        editingRow = -1;
    }
}

int Table::getCheckedRow() const
{
    return checkedRow;
}
int Table::getRowCount() const
{
    return rowCount;
}

int Table::getRowHeight() const
{
    return rowHeight;
}

void Table::mouseReleaseEvent(QMouseEvent *e)
{
    try {
        int row = findRow(e->pos());
        setRowChecked(row, true);
    } catch (...) {
       if (checkedRow != -1) setRowChecked(checkedRow, false);
       endRowsEditing();
    }
}

void Table::paintEvent(QPaintEvent *)
{
    if (checkedRow != -1) {
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(checkedRowBrush);
        painter.drawRect(0, ui->gridLayout->cellRect(checkedRow, 0).top(), width(), rowHeight);
    }
}

void Table::updateMinHeight()
{
    setMinimumHeight(rowCount * rowHeight);
}

int Table::findRow(const QPoint &point) const
{
    for (int i = 1, c = ui->gridLayout->rowCount(); i < c; i++) {
        QRect cellRect = ui->gridLayout->cellRect(i, 0);
        QRect rowRect = QRect(0, cellRect.top(), width(), cellRect.height());
        if (rowRect.contains(point)) return i;
    }
    throw "Row rect was not found.";
}

QLineEdit* Table::getItemAt(int row, int column) const
{
    QLayoutItem *item = ui->gridLayout->itemAtPosition(row, column);
    if (item) return static_cast<QLineEdit*>(item->widget());
    else return NULL;
}
