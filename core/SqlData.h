#pragma once

#include "Data.h"
#include <QLoggingCategory>
#include <Qstring>
#include <QtSql>

namespace Core
{
  class SqlData;
  struct TableRow;
}

struct Core::TableRow {
  bool starred;
  QString title;
  QString status;
  QString rating;
  QString comment;
};

/**
 * Manages sql data for the app
 */
class Core::SqlData
{
public:
  SqlData(const QString &filepath, const Core::Data &initialData);
  ~SqlData();

  QSqlQuery execDbQuery(const QString &query);
  void execDbQuery(QSqlQuery &query);

  QSqlQuery getCategories();
  QSqlQuery getTableRows(int tableId);
  void addTable(int id, const QString &name);
  void renameTable(int id, const QString &name);
  void removeTable(int id);
  size_t addTableRow(int tableId, const Core::TableRow &row);
  void updateTableRow(size_t rowId, const Core::TableRow &row);
  void updateTableRowStarred(size_t rowId, bool starred);
  void removeTableRow(size_t rowId);

private:
  void createTables();
  void configureDb();
  void importData(const Core::Data &data);
  void ensureDirExists(const QString &filepath);

  QLoggingCategory loggingCategory;
  QSqlDatabase db;
};
