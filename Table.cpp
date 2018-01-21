#include "Table.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>

#include <QDebug>

// ==== PUBLIC ====
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
    setFocusPolicy(Qt::ClickFocus);
}
Table::~Table()
{
    endRowsEditing();
    delete ui;
}
QSize Table::sizeHint() const
{
    return QSize(ui->gridLayout->minimumSize().width() * 1.5, rowHeight * rowCount);
}

void Table::insertRowAfter(const QList<QString> &list, int row)
{
    endRowsEditing();

    QLineEdit *newLineEdit;
    QPalette pal(palette());
    pal.setBrush(QPalette::Base, Qt::transparent);
    pal.setColor(QPalette::Text, Qt::black);
    pal.setColor(QPalette::HighlightedText, Qt::white);
    pal.setColor(QPalette::Highlight, "#249dcd");

    int column = 0;
    int rowToInsert = row + 1;

    // Перемещаем все следующие ряды на 1 вниз (освобождаем место)
    for (int i = ui->gridLayout->rowCount() - 1; i >= rowToInsert; --i) {
        replaceRow(i, i + 1);
    }

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

    rowCount += 1;
    emit rowAdded(rowToInsert);

    startRowEditing(rowToInsert);
}
void Table::appendRow(const QList<QString> &list)
{
    insertRowAfter(list, ui->gridLayout->rowCount() - 1);
}
void Table::deleteRow(int row)
{
    if (row == checkedRow) uncheckRows();

    for (int i = 0, c = ui->gridLayout->columnCount(); i < c; i++) {
        delete ui->gridLayout->itemAtPosition(row, i)->widget();
    }

    rowCount -= 1;
    emit rowDeleted(row);
}

void Table::checkRow(int row)
{
    if (row != checkedRow) {
        endRowsEditing();
        checkedRow = row;
        repaint();
        emit rowChecked(row);
    }
}
void Table::uncheckRows()
{
    if (checkedRow != -1) {
        endRowsEditing();
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
int Table::getRowHeight() const
{
    return rowHeight;
}
const QRect Table::getRowRect(int row) const
{
    if (hasRow(row)) {
        int top = 0;
        for (int i = 0; i < row; i++) if (hasRow(i)) top += rowHeight;
        return QRect(0, top, width(), rowHeight);
    }
    //if (hasRow(row)) return QRect(0, ui->gridLayout->cellRect(row, 0).top(), width(), rowHeight);
    else return QRect(0, 0, 0, 0);
}

// ==== PUBLIC SLOTS ====
void Table::startRowEditing(int row)
{
    endRowsEditing();
    checkRow(row);

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
    emit editingFinished();
    setFocus();
}

// ==== EVENTS ====
void Table::mousePressEvent(QMouseEvent *)
{
    // не нужно передавать виджетам ниже это событие
}
void Table::mouseReleaseEvent(QMouseEvent *e)
{
    try {
        int row = findRow(e->pos());
        checkRow(row);
    } catch (...) {
        uncheckRows();
    }
}
void Table::paintEvent(QPaintEvent *)
{
    if (checkedRow != -1) {
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(checkedRowBrush);
        painter.drawRect(getRowRect(checkedRow));
    }
}
void Table::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_F2:
        if (checkedRow != -1) startRowEditing(checkedRow);
        break;
    case Qt::Key_Delete:
        if (checkedRow != -1) {
            int row = checkedRow;
            deleteRow(checkedRow);
            emit rowDeleted(row);
        }
        break;
    case Qt::Key_Down:
        if (checkedRow == -1) {
            int firstRow = findRowAfter(0);
            if (firstRow != -1) checkRow(firstRow);
        } else {
            int rowAfter = findRowAfter(checkedRow);
            if (rowAfter != -1) {
                uncheckRows();
                checkRow(rowAfter);
            }
        }
        break;
    case Qt::Key_Up:
        if (checkedRow == -1) {
            int lastRow = findRowBefore(ui->gridLayout->rowCount());
            if (lastRow != -1) checkRow(lastRow);
        }
        else {
            int rowBefore = findRowBefore(checkedRow);
            if (rowBefore != -1) {
                uncheckRows();
                checkRow(rowBefore);
            }
        }
        break;
    }
}

// ==== PRIVATE ====
void Table::replaceRow(int from, int to)
{
    int columns = ui->gridLayout->columnCount();
    for (int i = 0; i < columns; i++) {
        QLayoutItem *item = ui->gridLayout->itemAtPosition(from, i);
        if (item) {
            ui->gridLayout->removeItem(item);
            ui->gridLayout->addItem(item, to, i);
        }
    }
}
bool Table::hasRow(int row) const
{
    int columns = ui->gridLayout->columnCount();
    for (int i = 0; i < columns; i++) {
        if (ui->gridLayout->itemAtPosition(row, i)) return true;
    }
    return false;
}
int Table::findRowBefore(int row) const
{
    if (row <= 1) return -1;
    else if (hasRow(row - 1)) return (row - 1);
    else return findRowBefore(row - 1);
}
int Table::findRowAfter(int row) const
{
    if (row >= ui->gridLayout->rowCount()) return -1;
    else if (hasRow(row + 1)) return (row + 1);
    else return findRowAfter(row + 1);
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
