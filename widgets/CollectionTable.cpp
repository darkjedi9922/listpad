#include "CollectionTable.h"
#include <QHBoxLayout>
#include "elements/StarButton.h"

CollectionTable::CollectionTable(QWidget *parent) :
  Table(parent),
  editingRow(-1),
  loggingCategory("CollectionTable")
{
  QObject::connect(this, &Table::rowChecked, this, &CollectionTable::endRowsEditing);
  QObject::connect(this, &Table::rowsUnchecked, this, &CollectionTable::endRowsEditing);
  QObject::connect(this, &Table::rowHovered, this, &CollectionTable::onRowHovered);
  QObject::connect(this, &Table::rowUnhovered, this, &CollectionTable::onRowUnhovered);
}
CollectionTable::~CollectionTable()
{
  endRowsEditing();
}

void CollectionTable::insertRowAfter(Table::row_id id, const Core::TableRow &data, int row)
{
  QList<QWidget*> items;

  items.append(new StarButton(this));

  QList<QString> stringColumns = {data.title, data.status, data.rating, data.comment};
  for (QList<QString>::const_iterator it = stringColumns.begin(); it != stringColumns.end(); it++) {
    auto newLineEdit = new LineEdit(*it, this);
    newLineEdit->setEnabled(false);
    newLineEdit->setUpdateGeometryMode(LineEdit::UpdateGeometryMode::TEXT_CHANGED);
    newLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    newLineEdit->setWhatsThis("line-edit");
    items.append(newLineEdit);
    QObject::connect(newLineEdit, SIGNAL(returnPressed()), this, SLOT(endRowsEditing()));
    QObject::connect(newLineEdit, &QLineEdit::cursorPositionChanged, [newLineEdit, this]() {
      if (newLineEdit->isEnabled() && newLineEdit->hasFocus()) {
          emit cellCursorPositionChanged(newLineEdit);
      }
    });
    QObject::connect(newLineEdit, &QLineEdit::textChanged, [id, this]() {
      if (editingRow == id) {
          emit rowTextEdited(id);
      }
    });
  }
  Table::insertRowAfter(id, items, row);
}
void CollectionTable::appendRow(Table::row_id id, const Core::TableRow &data)
{
  insertRowAfter(id, data, getRowCount() - 1);
}
bool CollectionTable::isStringsEmpty(Table::row_id rowId) const
{
  for (auto item : Table::getRow(rowId).mid(1)) {
    if (!static_cast<QLineEdit*>(item)->text().isEmpty()) {
      return false;
    }
  }
  return true;
}
Core::TableRow CollectionTable::getRow(Table::row_id rowId) const
{
  auto values = Table::getRow(rowId);
  return {
    static_cast<QLineEdit*>(values.at(1))->text(),
    static_cast<QLineEdit*>(values.at(2))->text(),
    static_cast<QLineEdit*>(values.at(3))->text(),
    static_cast<QLineEdit*>(values.at(4))->text()
  };
}
Core::TableRow CollectionTable::getRow(int row) const
{
  auto values = Table::getRow(row);
  return {
    static_cast<QLineEdit*>(values.at(1))->text(),
    static_cast<QLineEdit*>(values.at(2))->text(),
    static_cast<QLineEdit*>(values.at(3))->text(),
    static_cast<QLineEdit*>(values.at(4))->text()
  };
}

void CollectionTable::startRowEditing(Table::row_id rowId)
{
  qCInfo(loggingCategory) << "Starting editing row id" << rowId;
  if (rowId != -1) {
    endRowsEditing();
    checkRow(rowId);
    emit editingStarted(rowId);

    editingRow = rowId;
    int columns = 4;
    auto items = Table::getRow(rowId).mid(1);
    for (auto item : items) {
      auto lineEdit = static_cast<QLineEdit*>(item);
      item->setEnabled(true);
    }
    auto firstLineEdit = static_cast<QLineEdit*>(items.at(0));
    firstLineEdit->setFocus();
    firstLineEdit->setCursorPosition(firstLineEdit->text().length());
  }
}
void CollectionTable::endRowsEditing()
{
  if (editingRow != -1) {
    for (auto item : Table::getRow(editingRow).mid(1)) {
      item->setEnabled(false);
    }
    auto row = editingRow;
    editingRow = -1;
    qCInfo(loggingCategory) << "Emitting row editing finished for row id" << row;
    emit editingFinishedById(row);
    setFocus();
  }
}
void CollectionTable::onRowHovered(Table::row_id rowId)
{
  static_cast<StarButton*>(this->getCell(rowId, 0))->showIcon();
}
void CollectionTable::onRowUnhovered(Table::row_id rowId)
{
  static_cast<StarButton *>(this->getCell(rowId, 0))->hideIconIfUnchecked();
}

void CollectionTable::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_F2) {
    if (getCheckedRowId() != -1) {
      startRowEditing(getCheckedRowId());
    }
  } else {
    Table::keyPressEvent(e);
  }
}