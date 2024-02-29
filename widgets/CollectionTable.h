#pragma once

#include "../Table.h"
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

  void insertRowAfter(Table::row_id id, const QList<QString> &list, int row);
  void appendRow(Table::row_id, const QList<QString> &list);
  bool isStringsEmpty(Table::row_id rowId) const;
  QList<QString> getRow(Table::row_id rowId) const;
  QList<QString> getRow(int row) const;

public slots:
  void startRowEditing(Table::row_id row);
  void endRowsEditing();

protected:
  virtual void keyPressEvent(QKeyEvent *event);

private:
  Table::row_id editingRow;
  QLoggingCategory loggingCategory;
};
