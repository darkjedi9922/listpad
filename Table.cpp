#include "Table.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include "widgets/elements/LineEdit.h"
#include <stdexcept>

using namespace std;

// ==== PUBLIC ====
Table::Table(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Table),
    checkedRowBrush(QColor("#d5d4d4")),
    checkedRow(-1),
    rowCount(1),
    visibleRowCount(1),
    editingRow(-1),
    lastAddedRow(-1),
    deletingRow(-1),
    downArrowClickHandler(this),
    upArrowClickHandler(this),
    loggingCategory("Table")
{
    ui->setupUi(this);
    setFocusPolicy(Qt::ClickFocus);
}
Table::~Table()
{
    endRowsEditing();

    delete ui;
    ui = nullptr;
}
QSize Table::sizeHint() const
{
    auto sizeHint = ui->gridLayout->sizeHint();
    return QSize(int(sizeHint.width() * 1.1), sizeHint.height());
}

QList<QString> Table::getRow(int row) const
{
    QList<QString> list;
    for (int i = 0; i < getColumnCount(); i++) {
        QLineEdit *item = getItemAt(row, i);
        if (item) list << item->text();
        else list << "";
    }
    return list;
}
QList<QString> Table::getRow(row_id rowId) const
{
    return getRow(getRowById(rowId));
}

bool Table::hasRow(int row) const
{
    return row >= 0 && row < rowCount;
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
bool Table::isStringsEmpty(row_id rowId) const
{
    return isStringsEmpty(getRowById(rowId));
}
void Table::setRowVisible(int row, bool visible)
{
    // Для скрытия была попытка использовать метод setVisible() виджета, но
    // оказалось, что setVisible(true) на скрытом виджете выполняется довольно
    // долго. В случаях, когда в таблице очень много записей, все они скрыты, и
    // их всех нужно сразу показать - это затягивается надолго.
    //
    // Быстро-выполняющимся решением оказалось изменение высоты.

    int columns = getColumnCount();

    // Работаем только с первой колонкой
    if (columns >= 0) {
        auto firstColumn = ui->gridLayout->itemAtPosition(row, 0)->widget();
        if (visible) {
            // Если нужно показать, а первая колонка уже показана, можно смело
            // выходить - вся строка уже показана.
            if (firstColumn->height() != 0) return;
            // Показываем первую колонку и считаем, что строка стала видима.
            firstColumn->setMaximumHeight(QWIDGETSIZE_MAX);
            visibleRowCount += 1;
        } else {
            // Если нужно скрыть, а первая колонка уже скрыта - строка уже скрыта.
            if (!visible && firstColumn->height() == 0) return;
            // Скрываем первую колонку и считаем, что строка стала скрыта.
            firstColumn->setMaximumHeight(0);
            visibleRowCount -= 1;

            if (checkedRow == row) uncheckRows();
        }
    }

    // Работаем с остальными колонками
    for (int i = 1; i < columns; ++i) {
        auto column = ui->gridLayout->itemAtPosition(row, i)->widget();
        column->setMaximumHeight(visible ? QWIDGETSIZE_MAX : 0);
    }
}

bool Table::isRowVisible(int row) const
{
    if (row == 0) return true;

    int columns = getColumnCount();
    if (columns == 0) return false;

    auto firstColumn = ui->gridLayout->itemAtPosition(row, 0)->widget();
    return firstColumn->height() != 0;
}

void Table::insertRowAfter(row_id id, const QList<QString> &list, int row)
{
    LineEdit *newLineEdit;

    int column = 0;
    int rowToInsert = row + 1;

    // Перемещаем все следующие ряды на 1 вниз (освобождаем место)
    for (int i = ui->gridLayout->rowCount() - 1; i >= rowToInsert; --i) {
        replaceRow(i, i + 1);
    }

    rowsById.insert_or_assign(id, rowToInsert);
    idsByRow.insert_or_assign(rowToInsert, id);
    qCDebug(loggingCategory) << "Inserted row id" << id << "at row" << rowToInsert;
    for (QList<QString>::const_iterator it = list.begin(); it != list.end(); it++) {
        newLineEdit = new LineEdit(*it, this);
        newLineEdit->setEnabled(false);
        newLineEdit->setUpdateGeometryMode(LineEdit::UpdateGeometryMode::TEXT_CHANGED);
        newLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        newLineEdit->setWhatsThis("line-edit");
        ui->gridLayout->addWidget(newLineEdit, rowToInsert, column);
        QObject::connect(newLineEdit, SIGNAL(returnPressed()), this, SLOT(endRowsEditing()));
        QObject::connect(newLineEdit, &QLineEdit::cursorPositionChanged, [newLineEdit, this]() {
            if (newLineEdit->isEnabled() && newLineEdit->hasFocus()) {
                emit cellCursorPositionChanged(newLineEdit);
            }
        });
        QObject::connect(newLineEdit, &QLineEdit::textChanged, [id, this]() {
            auto row = rowsById.at(id);
            if (editingRow == row) {
                emit rowTextEdited(row);
            }
        });
        column += 1;
    }

    rowCount += 1;
    visibleRowCount += 1;
    lastAddedRow = rowToInsert;
    emit rowAdded(rowToInsert);
}
void Table::appendRow(row_id id, const QList<QString> &list)
{
    return insertRowAfter(id, list, rowCount - 1);
}
void Table::deleteRow(row_id rowId)
{
    if (!rowsById.count(rowId)) {
        return;
    }
    deleteRow(rowsById.at(rowId));
}
void Table::deleteRow(int row)
{
    if (hasRow(row) && deletingRow != row)
    {
        qCInfo(loggingCategory) << "Deleting row" << row;
        deletingRow = row;
        if (row == checkedRow) uncheckRows();
        if (isRowVisible(row)) visibleRowCount -= 1;

        qCDebug(loggingCategory) << "Deleting row (2)";
        for (int i = 0, c = ui->gridLayout->columnCount(); i < c; i++)
        {
            QLayoutItem *item = ui->gridLayout->itemAtPosition(row, i);
            if (item) {
                item->widget()->hide();
                // Виджет создавался с родителем - текущим объектом. А при указании
                // parent, родитель сам удаляет (delete) своих детей. Значит нам
                // самим удалять не надо (оно и крашится на любые варианты delete).
                ui->gridLayout->removeItem(item);
            }
        }
        qCDebug(loggingCategory) << "Row id count" << idsByRow.count(row);
        qCDebug(loggingCategory) << "Deleting row" << row << "with id" << idsByRow.at(row);
        rowsById.erase(idsByRow.at(row));
        idsByRow.erase(row);

        // Перемещаем все следующие ряды на 1 вверх (на 1 освобожденное место)
        qCDebug(loggingCategory) << "Deleting row (3)";
        for (int i = row + 1; i < rowCount; ++i) replaceRow(i, i - 1);

        rowCount -= 1;
        deletingRow = -1;
        emit rowDeleted(row);
        qCDebug(loggingCategory) << "Deleting row (4)";
    }
}
void Table::empty()
{
    while (rowCount > 1) deleteRow(rowCount - 1);
}
int Table::getLastAddedRow() const
{
    return lastAddedRow;
}

void Table::checkRow(int row)
{
    if (row == checkedRow) return;
    endRowsEditing();
    checkedRow = row;
    repaint();
    emit rowChecked(row);
}
void Table::uncheckRows()
{
    if (checkedRow == -1) return;
    qCInfo(loggingCategory) << "Unchecking row" << checkedRow;
    endRowsEditing();
    checkedRow = -1;
    repaint();
    rowsUnchecked();
}
int Table::getCheckedRow() const
{
    return checkedRow;
}
Table::row_id Table::getCheckedRowId() const
{
    if (checkedRow == -1) {
        return -1;
    }
    return idsByRow.at(checkedRow);
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

int Table::getVisibleRowCount() const
{
    return visibleRowCount;
}
const QRect Table::getRowRect(int actualRow) const
{
    if (hasRow(actualRow)) {
        auto cellRect = ui->gridLayout->cellRect(actualRow, 0);
        return QRect(0, cellRect.top(), width(), cellRect.height());
    }
    return QRect(0, 0, 0, 0);
}

// ==== PUBLIC SLOTS ====
void Table::startRowEditing(int row)
{
    qCInfo(loggingCategory) << "Starting editing row" << row;
    if (row != -1) {
        endRowsEditing();
        checkRow(row);
        emit editingStarted(row);

        editingRow = row;
        int columns = ui->gridLayout->columnCount();
        for (int i = 0; i < columns; ++i) {
            QLineEdit *item = getItemAt(row, i);
            if (item) {
                item->setEnabled(true);
                if (i == 0) {
                    item->setFocus();
                    item->setCursorPosition(item->text().length());
                }
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
                item->setEnabled(false);
            }
        }
        int row = editingRow;
        editingRow = -1;
        qCInfo(loggingCategory) << "Emitting row editing finished for row" << row;
        emit editingFinishedById(idsByRow.at(row));
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
    case Qt::Key_Down: downArrowClickHandler.handle(); break;
    case Qt::Key_Up: upArrowClickHandler.handle(); break;
    }
}

// ==== PRIVATE ====
void Table::replaceRow(int from, int to)
{
    if (idsByRow.count(from) == 0) {
        return;
    }
    auto rowId = idsByRow.at(from);
    int columns = ui->gridLayout->columnCount();
    for (int i = 0; i < columns; i++) {
        QLayoutItem *item = ui->gridLayout->itemAtPosition(from, i);
        if (item) {
            ui->gridLayout->removeItem(item);
            ui->gridLayout->addItem(item, to, i);
        }
    }
    qCDebug(loggingCategory) << "Erase id of row" << from;
    idsByRow.erase(from);
    idsByRow.insert_or_assign(to, rowId);
    rowsById.insert_or_assign(rowId, to);
}

int Table::toActualRow(int visibleRow) const
{
    // Заголовок таблицы всегда 0.
    if (visibleRow == 0) return 0;

    int visibleCount = 1;
    for (int i = 1; i < rowCount; ++i) {
        if (isRowVisible(i)) ++visibleCount;
        if (visibleRow == visibleCount - 1) return i;
    }

    return -1;
}

int Table::toVisibleRow(int actualRow) const
{
    // Заголовок таблицы всегда 0.
    if (actualRow == 0) return 0;

    int visibleCount = 1;
    for (int i = 1; i <= actualRow; ++i) {
        if (isRowVisible(i)) ++visibleCount;
    }

    return visibleCount - 1;
}
int Table::getRowById(row_id rowId) const
{
    if (!rowsById.count(rowId)) {
        auto error = QString("Row id %1 does not exist").arg(rowId);
        qCCritical(loggingCategory) << error;
        throw std::out_of_range(error.toStdString());
    }
    return rowsById.at(rowId);
}

int Table::findRow(const QPoint &point) const
{
    int actualRow = -1;
    // TODO: for optimization, this can be implemented via binary search
    for (int row = 0; row < ui->gridLayout->rowCount(); ++row) {
        auto firstCellRect = ui->gridLayout->cellRect(row, 0);
        if (firstCellRect.top() <= point.y() && point.y() <= firstCellRect.bottom()) {
            actualRow = row;
            break;
        }
    }
    if (actualRow != -1 && actualRow != 0) return actualRow;
    throw "Row rect was not found.";
}

QLineEdit* Table::getItemAt(int row, int column) const
{
    QLayoutItem *item = ui->gridLayout->itemAtPosition(row, column);
    if (item && item->widget() && item->widget()->whatsThis() == "line-edit")
        return static_cast<QLineEdit*>(item->widget());
    else return nullptr;
}
