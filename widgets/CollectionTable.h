#pragma once

#include "../Table.h"
#include "../core/SqlData.h"
#include <QLoggingCategory>

/**
 * Collection table wrapper around low-level table widget `Table`
 */
class CollectionTable : public Table
{
  Q_OBJECT

signals:
  void cellCursorPositionChanged(LineEdit *cell);
  void rowTextEdited(Table::row_id rowId);
  void editingStarted(int row);
  void editingFinishedById(Table::row_id rowId);

public:
  CollectionTable(QWidget *parent = nullptr);
  ~CollectionTable();

  void insertRowAfter(Table::row_id id, const Core::TableRow &data, int row);
  void appendRow(Table::row_id, const Core::TableRow &data);
  bool isStringsEmpty(Table::row_id rowId) const;
  Core::TableRow getRow(Table::row_id rowId) const;
  Core::TableRow getRow(int row) const;

public slots:
  void startRowEditing(Table::row_id rowId);
  void endRowsEditing();
  void onRowHovered(Table::row_id rowId);
  void onRowUnhovered(Table::row_id rowId);

protected:
  virtual void keyPressEvent(QKeyEvent *event);

private:
  Table::row_id editingRow;
  QLoggingCategory loggingCategory;
};
