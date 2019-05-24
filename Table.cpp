#include "Table.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>

// ==== PUBLIC ====
Table::Table(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Table),
    checkedRowBrush(QColor("#d5d4d4")),
    checkedRealRow(-1),
    rowHeight(27),
    rowCount(1),
    visibleRealRowCount(1),
    editingRow(-1),
    lastAddedRow(-1)
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
    return QSize(ui->gridLayout->minimumSize().width() * 1.1, rowHeight * visibleRealRowCount);
}

QList<QString> Table::getRealRow(int row) const
{
    QList<QString> list;
    for (int i = 0; i < getColumnCount(); i++) {
        QLineEdit *item = getItemAt(row, i);
        if (item) list << item->text();
        else list << "";
    }
    return list;
}
bool Table::hasRealRow(int row) const
{
    return ui->gridLayout->itemAtPosition(row, 0);
}
bool Table::isStringsEmpty(int row) const
{
    int columns = getColumnCount();
    for (int i = 0; i < columns; i++)
    {
        QLineEdit *item = getItemAt(row, i);
        if (item && !item->text().isEmpty()) return false;
    }
    return true;
}
void Table::setRealRowVisible(int row, bool v)
{
    // Для скрытия была попытка использовать метод setVisible() виджета, но
    // оказалось, что setVisible(true) на скрытом виджете выполняется довольно
    // долго. В случаях, когда в таблице очень много записей, все они скрыты, и
    // их всех нужно сразу показать - это затягивается надолго.
    //
    // Быстро-выполняющимся решением оказалось изменение высоты.

    int columns = getColumnCount();

    if (columns >= 0) {
        auto firstColumn = ui->gridLayout->itemAtPosition(row, 0)->widget();
        if (v) {
            // Если нужно показать, а первая колонка уже показана, можно смело
            // выходить - вся строка уже показана.
            if (firstColumn->height() != 0) return;
            // Показываем первую колонку и считаем, что строка стала видима.
            firstColumn->setFixedHeight(rowHeight);
            visibleRealRowCount += 1;
        } else {
            // Если нужно скрыть, а первая колонка уже скрыта - строка уже скрыта.
            if (!v && firstColumn->height() == 0) return;
            // Скрываем первую колонку и считаем, что строка стала скрыта.
            firstColumn->setFixedHeight(0);
            visibleRealRowCount -= 1;

            if (checkedRealRow == row) uncheckRows();
        }
    }

    for (int i = 1; i < columns; ++i) {
        auto column = ui->gridLayout->itemAtPosition(row, i)->widget();
        column->setFixedHeight(v ? rowHeight : 0);
    }
}

void Table::insertRowAfter(const QList<QString> &list, int row)
{
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
        newLineEdit->setWhatsThis("line-edit");
        ui->gridLayout->addWidget(newLineEdit, rowToInsert, column);
        QObject::connect(newLineEdit, SIGNAL(returnPressed()), this, SLOT(endRowsEditing()));
        column += 1;
    }

    rowCount += 1;
    visibleRealRowCount += 1;
    lastAddedRow = rowToInsert;
    emit rowAdded(rowToInsert);
}
void Table::appendRow(const QList<QString> &list)
{
    insertRowAfter(list, ui->gridLayout->rowCount() - 1);
}
void Table::deleteRow(int row)
{
    if (hasRealRow(row))
    {
        if (row == checkedRealRow) uncheckRows();

        for (int i = 0, c = ui->gridLayout->columnCount(); i < c; i++)
        {
            QLayoutItem *item = ui->gridLayout->itemAtPosition(row, i);
            if (item) delete ui->gridLayout->itemAtPosition(row, i)->widget();
        }

        rowCount -= 1;
        visibleRealRowCount -= 1;
        emit rowDeleted(row);
    }
}
void Table::empty()
{
    int rows = ui->gridLayout->rowCount();
    for (int i = 1; i < rows; i++) deleteRow(i);
}
int Table::getLastAddedRow() const
{
    return lastAddedRow;
}

void Table::checkRealRow(int row)
{
    if (checkRealRowWithoutEmit(row)) emit rowChecked(row);
}
void Table::uncheckRows()
{
    if (uncheckRowsWithoutEmit()) emit rowsUnchecked();
}
int Table::getCheckedRow() const
{
    return checkedRealRow;
}

int Table::getEditingRow() const
{
    return editingRow;
}

int Table::getColumnCount() const
{
    return ui->gridLayout->columnCount();
}
int Table::getRowCount() const
{
    return rowCount;
}
int Table::getRowRealCount() const
{
    return ui->gridLayout->rowCount();
}
int Table::getRowHeight() const
{
    return rowHeight;
}
const QRect Table::getRowRect(int row) const
{
    if (hasRealRow(row)) {
        int top = 0;
        for (int i = 0; i < row; i++) if (hasRealRow(i)) top += rowHeight;
        return QRect(0, top, width(), rowHeight);
    }
    //if (hasRealRow(row)) return QRect(0, ui->gridLayout->cellRect(row, 0).top(), width(), rowHeight);
    else return QRect(0, 0, 0, 0);
}

// ==== PUBLIC SLOTS ====
void Table::startRowEditing(int row)
{
    if (row != -1) {
        endRowsEditing();
        checkRealRow(row);

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
        int row = editingRow;
        editingRow = -1;
        emit editingFinished(row);
        setFocus();
    }
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
        checkRealRow(row);
    } catch (...) {
        uncheckRows();
    }
}
void Table::paintEvent(QPaintEvent *)
{
    if (checkedRealRow != -1) {
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(checkedRowBrush);
        painter.drawRect(getRowRect(checkedRealRow));
    }
}
void Table::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_F2:
        if (checkedRealRow != -1) startRowEditing(checkedRealRow);
        break;
    case Qt::Key_Delete:
        if (checkedRealRow != -1) {
            int row = checkedRealRow;
            deleteRow(checkedRealRow);
            emit rowDeleted(row);
        }
        break;
    case Qt::Key_Down:
        if (checkedRealRow == -1) {
            int firstRow = findRowAfter(0);
            if (firstRow != -1) checkRealRow(firstRow);
        } else {
            int rowAfter = findRowAfter(checkedRealRow);
            if (rowAfter != -1) {
                uncheckRowsWithoutEmit();
                checkRealRowWithoutEmit(rowAfter);
                emit rowRechecked(rowAfter);
            }
        }
        break;
    case Qt::Key_Up:
        if (checkedRealRow == -1) {
            int lastRow = findRowBefore(ui->gridLayout->rowCount());
            if (lastRow != -1) checkRealRow(lastRow);
        }
        else {
            int rowBefore = findRowBefore(checkedRealRow);
            if (rowBefore != -1) {
                uncheckRowsWithoutEmit();
                checkRealRowWithoutEmit(rowBefore);
                emit rowRechecked(rowBefore);
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

bool Table::checkRealRowWithoutEmit(int row)
{
    if (row != checkedRealRow) {
        endRowsEditing();
        checkedRealRow = row;
        repaint();
        return true;
    }
    return false;
}

bool Table::uncheckRowsWithoutEmit()
{
    if (checkedRealRow != -1) {
        endRowsEditing();
        checkedRealRow = -1;
        repaint();
        return true;
    }
    return false;
}

int Table::findRowBefore(int row) const
{
    if (row <= 1) return -1;
    else if (hasRealRow(row - 1)) return (row - 1);
    else return findRowBefore(row - 1);
}
int Table::findRowAfter(int row) const
{
    if (row >= ui->gridLayout->rowCount()) return -1;
    else if (hasRealRow(row + 1)) return (row + 1);
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
    if (item && item->widget() && item->widget()->whatsThis() == "line-edit")
        return static_cast<QLineEdit*>(item->widget());
    else return nullptr;
}
