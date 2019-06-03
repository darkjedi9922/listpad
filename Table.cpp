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
    return QSize(int(ui->gridLayout->minimumSize().width() * 1.1), rowHeight * visibleRealRowCount);
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
void Table::setRealRowVisible(int row, bool v)
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

    // Работаем с остальными колонками
    for (int i = 1; i < columns; ++i) {
        auto column = ui->gridLayout->itemAtPosition(row, i)->widget();
        column->setFixedHeight(v ? rowHeight : 0);
    }
}

bool Table::isRealRowVisible(int row) const
{
    if (row == 0) return true;

    int columns = getColumnCount();
    if (columns == 0) return false;

    auto firstColumn = ui->gridLayout->itemAtPosition(row, 0)->widget();
    return firstColumn->height() != 0;
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
    insertRowAfter(list, rowCount - 1);
}
void Table::deleteRow(int row)
{
    if (hasRow(row))
    {
        if (row == checkedRealRow) uncheckRows();
        if (isRealRowVisible(row)) visibleRealRowCount -= 1;

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

        // Перемещаем все следующие ряды на 1 вверх (на 1 освобожденное место)
        for (int i = row + 1; i < rowCount; ++i) replaceRow(i, i - 1);

        rowCount -= 1;
        emit rowDeleted(row);
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
    row = toVisibleRow(row);

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
        if (visibleRealRowCount == 1) return;
        if (checkedRealRow == -1) checkRealRow(1);
        else {
            int nextRow = toActualRow(toVisibleRow(checkedRealRow) + 1);
            if (nextRow != -1 && nextRow < visibleRealRowCount) {
                uncheckRowsWithoutEmit();
                checkRealRowWithoutEmit(nextRow);
                emit rowRechecked(nextRow);
            }
        }
        break;
    case Qt::Key_Up:
        if (visibleRealRowCount == 1) return;
        if (checkedRealRow == -1) checkRealRow(rowCount - 1);
        else {
            int prevRow = toActualRow(toVisibleRow(checkedRealRow) - 1);
            if (prevRow > 0) {
                uncheckRowsWithoutEmit();
                checkRealRowWithoutEmit(prevRow);
                emit rowRechecked(prevRow);
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

int Table::toActualRow(int visibleRow) const
{
    // Заголовок таблицы всегда 0.
    if (visibleRow == 0) return 0;

    int visibleCount = 1;
    for (int i = 1; i < rowCount; ++i) {
        if (isRealRowVisible(i)) ++visibleCount;
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
        if (isRealRowVisible(i)) ++visibleCount;
    }

    return visibleCount - 1;
}

int Table::findRow(const QPoint &point) const
{
    int visibleRow = point.y() / rowHeight;
    int actualRow = toActualRow(visibleRow);
    qDebug() << actualRow;
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
