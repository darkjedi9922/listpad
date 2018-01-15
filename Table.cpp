#include "Table.h"
#include <QMouseEvent>
#include <QPainter>

Table::Table(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Table),
    checkedRowBrush(QColor("#d5d4d4")),
    checkedRow(-1),
    rowHeight(27),
    rowCount(1)
{
    ui->setupUi(this);
    updateMinHeight();
}

Table::~Table()
{
    delete ui;
}

void Table::addRow(QLabel *name, QLabel *status, QLabel *rating, QLabel *comment)
{
    int row = ui->gridLayout->rowCount();
    ui->gridLayout->addWidget(name, row, 0);
    ui->gridLayout->addWidget(status, row, 1);
    ui->gridLayout->addWidget(rating, row, 2);
    ui->gridLayout->addWidget(comment, row, 3);

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

int Table::getCheckedRow() const
{
    return checkedRow;
}
int Table::getRowCount() const
{
    return rowCount;
}

void Table::mouseReleaseEvent(QMouseEvent *e)
{
    try {
        int row = findRow(e->pos());
        setRowChecked(row, true);
    } catch (...) {
       if (checkedRow != -1) setRowChecked(checkedRow, false);
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
