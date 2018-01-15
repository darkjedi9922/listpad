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

void Table::mouseReleaseEvent(QMouseEvent *e)
{
    try {
        int row = findRow(e->pos());
        if (checkedRow != row) {
            checkedRow = row;
            repaint();
            emit rowChecked();
        }
    } catch (...) {
        if (checkedRow != -1) {
            checkedRow = -1;
            repaint();
            emit rowsUnchecked();
        }
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